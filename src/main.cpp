/*
 * Copyright (C) 2020  Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
 * Author: Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
 *
 * This file is a part of epubmaker
 * epubmaker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * epubmaker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <iostream>

#include "../headers/filter.hpp"
#include "../headers/chapter.hpp"
#include "../headers/status.hpp"
#include "../headers/args.hpp"
#include "../headers/misc.hpp"
#include "../headers/manifestEntry.hpp"
#include "../headers/spineEntry.hpp"
#include "../headers/navPoint.hpp"
#include "../headers/book.hpp"
#include "../headers/order.hpp"
#include "../headers/defaults.hpp"
#include "../headers/fs.hpp"

const std::string_view orderFile{"order.txt"};
const short minArgs{2};

void help(std::ostream& out = std::cerr);
void version();

int main(int argc, char** argv)
{
	if (argc < minArgs)
	{
		help();
		return NOT_ENOUGH_ARGUMENTS;
	}

	arguments args{};

	switch (parse_args(args, argc - 1, argv+1))
	{
		case INVALID_OPTION:
			std::cerr << "Unknown option " << args.unknownOption << '\n';
			help();
			return INVALID_OPTION;

		case ARG_REQUIRED:
			std::cerr << "Option " << args.requiresArgument << " requires an argument\n";
			help();
			return ARG_REQUIRED;

		// parse_args can only return NORMAL, INVALID_OPTION, or ARG_REQUIRED
		default:
			break;
	}

	if (args.help)
	{
		help(std::cout);
		return 0;
	}

	if (args.version)
	{
		version();
		return 0;
	}

	if (!fs::exists(args.path))
	{
		std::cerr << "Directory " << args.path << " Does not exist\n\n";
		return DIR_DOES_NOT_EXIST;
	}

	if (!fs::exists(fs::path{args.path / std::string{orderFile.data()}}))
	{
		std::cerr << "Directory " << args.path << " Does not contain a " << orderFile << " file\n\n";

		return NO_ORDER_FILE;
	}

	std::ifstream order{args.path / orderFile};
	std::vector<std::string> chapters{};

	if (std::string chapter{read_chapter_order(order, chapters)}; chapter != "")
	{
		std::cerr << "Invalid extension for chapter: " << chapter << " in " << args.path / orderFile << '\n'
		<< "Please make sure that it only contains .html and/or .xhtml files\n\n";

		return INVALID_CHAPTER_FILE;
	}

	if (std::string chapter{verify_chapter_existance(chapters, args.path)}; chapter != "")
	{
		std::cerr << "Chapter " << chapter << " in " <<  args.path / orderFile << " does not exist\n\n";

		return CHAPTER_DOES_NOT_EXIST;
	}

	// Error if --no-style and a custom styledir are both specified
	if (!args.stylesheets && args.styleDirs.size() > 1)
	{
		std::cerr << "Incompatable options --no-style and --style-dir " << args.styleDirs[1] << "\n\n";

		return INCOMPATABLE_OPTIONS;
	}

	// Error if --no-images and a custom imgdir are both specified
	if (!args.images && args.imgDirs.size() > 1)
	{
		std::cerr << "Incompatable options --no-images and --img-dir " << args.imgDirs[1] << "\n\n";

		return INCOMPATABLE_OPTIONS;
	}

	if (args.stylesheets)
	{
		for (std::string_view styledir : args.styleDirs)
		{
			if (!fs::exists(args.path / styledir))
			{
				// Only error if a custom styledir was specifed
				if (styledir != defaults::styleDir)
				{
					std::cerr << args.path << " does not contain a directory " << styledir << "\n\n";

					return STYLEDIR_DOES_NOT_EXIST;
				}
				// If the default STYLEDIR is the only one asume --no-style
				else if (args.styleDirs.size() == 1)
				{
					args.stylesheets = false;
				}
				// If there are other styledirs just remove the default
				else
				{
					args.styleDirs.pop_front();
				}
			}
		}
	}

	if (args.images)
	{
		for (std::string_view imgDir : args.imgDirs)
		{
			if (!fs::exists(args.path / imgDir))
			{
				// Only error if a custom imgdir was specifed else just asume --no-images
				if (imgDir != defaults::imgDir)
				{
					std::cerr << args.path << " does not contain a directory " << imgDir << "\n\n";

					return IMGDIR_DOES_NOT_EXIST;
				}
				else if (args.imgDirs.size() == 1)
				{
					args.images = false;
				}
				else
				{
					args.imgDirs.pop_front();
				}
			}
		}
	}

	Book book
	{
		args.path,
		args.title,
		args.author,
		args.language,
		args.identifier,
		args.publisher,
		args.description,
		chapters,
		args.coverFile,
		args.styleDirs,
		args.stylesheets,
		args.imgDirs,
		args.images,
	};

	statusCode status{book.write(add_extension(args.outfile, ".epub"), args.force, args.cover, args.toc)};

	switch (status)
	{
		case OUTFILE_EXISTS:
			std::cerr << "Target file " << args.path << args.outfile << " already exists and --force was not specified\n\n";

			return OUTFILE_EXISTS;

		case COULD_NOT_OPEN:
			std::cerr << "Could not open file " << args.path / args.outfile << " for writing\n";
			return COULD_NOT_OPEN;

		case NORMAL:
			return NORMAL;
			break;

		default:
			// This path should never be executed but not having it would cause a warning.
			std::cerr << "Unknown error\n\n";

			help();
			return UNKOWN_ERROR;
	}

	return NORMAL;
}

void help(std::ostream& out)
{
	static std::string_view indent{"  "};

	version();

	out << "\nUsage: epubmaker [OPTIONS] DIRNAME\n"
	<< "Create an epub file from a list of xhtml files\n"
	<< "Example: epubmaker -a \"Kjartan Óli Ágústsson\" -t Test -p \"Example Ltd.\" -l en -i IDENTIFIER .\n\n"
	<< "Options:\n"
	<< indent << "Metadata:\n"
	<< indent << indent << "-a, --author=AUTHOR              Set the author of the document\n"
	<< indent << indent << "-t, --title=TITLE                Set the title of the document\n"
	<< indent << indent << "-d, --date=DATE                  Set the publication date, if this argument is skipped the current date and time are used\n"
	<< indent << indent << "-D, --description=DESCRIPTION    Set the description of the document\n"
	<< indent << indent << "-i, --identifier=IDENTIFIER      Set the identifier of the document\n"
	<< indent << indent << "-l, --language=LANGUAGE          Set the language of the document\n"
	<< indent << indent << "-p, --publisher=PUBLISHER        Set the publisher of the document\n"

	<< '\n' << indent << "Input control:\n"
	<< indent << indent <<  "-c, --cover=COVERFILE            Supply the path for a custom cover for the document. If this argument is skipped\n"
	<< "                                     the file DIRNAME/cover.xhtml is used if it exists, else a cover is generated automatically\n"
	<< indent << indent << "--img-dir=IMGDIR                 Specify an additional directory in which to search for images.\n"
		<< indent << indent << "--no-cover                       Do not generate a cover for the document\n"
	<< indent << indent << "--no-toc                         Do not generate a table of contents\n"
	<< indent << indent << "--no-style                       Do not add any stylesheets to the document\n"
	<< indent << indent << "--no-images                      Do not add any images to the document\n"
	<< indent << indent << "--style-dir STYLEDIR             Specify an additional directory in which to search for stylesheets.\n"
	<< indent << "Output control:\n"
	<< indent << indent << "-f, --force                      Overwrite the output file if it already exists\n"
	<< indent << indent << "-o, --outfile=OUTFILE            Set the file to write output to. If this argument is skipped\n"
	<< "                                     output is written to DIRNAME/book.epub\n"

	<< '\n' << indent << "Miscellaneous:\n"
	<< indent << indent << "-h, --help                       Print this help and exit\n"
	<< indent << indent << "-V, --version                    Print version information and exit\n"
	<< '\n';

	out << "Error codes:\n"
	<< "    " << NOT_ENOUGH_ARGUMENTS << "    Not enough arguments were provided\n"
	<< "    " << DIR_DOES_NOT_EXIST << "    The target directory does not exist\n"
	<< "    " << NO_ORDER_FILE << "    No " << orderFile << " was found in the target directory\n"
	<< "    " << INVALID_OPTION << "    An unrecognised option was passed\n"
	<< "    " << INCOMPATABLE_OPTIONS << "    Two or more options that are incompatable were passed\n"
	<< "    " << ARG_REQUIRED << "    An option that requires an argument was not passed one\n"
	<< "    " << INVALID_CHAPTER_FILE << "    A file listed in " << orderFile << " does not have a valid extension\n"
	<< "    " << CHAPTER_DOES_NOT_EXIST << "    A file listed in " << orderFile << " does not exist\n"
	<< "    " << STYLEDIR_DOES_NOT_EXIST << "    A user specified directory for stylesheets does not exist\n"
	<< "   " << IMGDIR_DOES_NOT_EXIST << "    A user specified directory for images does not exist\n"
	<< "   " << OUTFILE_EXISTS << "    The target file already exists and --force was not specified\n"
	<< "   " << COULD_NOT_OPEN << "    Failed to open output file\n"
	<< "   " << UNKOWN_ERROR << "    An unknown error occurred, this should never occur\n"
	<< '\n';
}

