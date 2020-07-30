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
#include <unistd.h>
#include <map>
#include <iostream>
#include <filesystem>

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

const std::string_view orderFile{"order.txt"};
const short minArgs{2};

void help(std::ostream& out = std::cerr);
void version();

namespace fs = std::filesystem;

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
			std::cerr << "Option " << args.requires_argument << " requires an argument\n";
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
		std::cerr << "Directory " << args.path << " Does not exist\n";
		return DIR_DOES_NOT_EXIST;
	}

	if (!fs::exists(fs::path{args.path + std::string{orderFile.data()}}))
	{
		std::cerr << "Directory " << args.path << " Does not contain a " << orderFile << " file\n";
		return NO_ORDER_FILE;
	}

	std::ifstream order{args.path + std::string{orderFile.data()}};
	std::vector<std::string> chapters{};

	if (std::string chapter{read_chapter_order(order, chapters)}; chapter != "")
	{
		std::cerr << "Invalid extension for chapter: " << chapter << " in " << args.path <<  orderFile << '\n'
		<< "Please make sure that it only contains .html and/or .xhtml files\n";

		return INVALID_CHAPTER_FILE;
	}

	if (std::string chapter{verify_chapter_existance(chapters, args.path)}; chapter != "")
	{
			std::cerr << "Chapter " << chapter << " in " <<  args.path << orderFile << " does not exist\n";
			return CHAPTER_DOES_NOT_EXIST;
	}

	if (args.stylesheets)
	{
		if (!fs::exists(args.path + args.styleDir))
		{
			std::cerr << args.path << " does not contain a directory " << args.styleDir << '\n';
			return STYLEDIR_DOES_NOT_EXIST;
		}
	}

	if (args.images)
	{
		 if (!fs::exists(args.path + args.imgDir))
		 {
		 	std::cerr << args.path << " does not contain a directory " << args.imgDir << '\n';
			return IMGDIR_DOES_NOT_EXIST;
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
		chapters,
		args.coverFile,
		args.styleDir,
		args.stylesheets,
		args.imgDir,
		args.images
	};

	statusCode status{book.write(add_extension(args.outfile, ".epub"), args.force, args.cover, args.toc)};

	switch (status)
	{
		case OUTFILE_EXISTS:
			std::cerr << "Target file " << args.path << args.outfile << " already exists and --force was not specified\n";
			return OUTFILE_EXISTS;

			case NORMAL:
			return NORMAL;
			break;

		default:
			// This path should never be executed but not having it would cause a warning.
			std::cerr << "Unknown error\n";
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
	<< indent << indent << "-i, --identifier=IDENTIFIER      Set the identifier of the document\n"
	<< indent << indent << "-l, --language=LANGUAGE          Set the language of the document\n"
	<< indent << indent << "-p, --publisher=PUBLISHER        Set the publisher of the document\n"

	<< '\n' << indent << "Input control:\n"
	<< indent << indent <<  "-c, --cover=COVERFILE            Supply the path for a custom cover for the document. If this argument is skipped\n"
	<< "                                     the file DIRNAME/cover.xhtml is used if it exists, else a cover is generated automatically\n"
	<< indent << indent << "--img-dir=IMGDIR                 Specify a directory in which to search for images."
	<< " If this argument is skipped\n"
	<< "                                     the default of DIRNAME/Images is used\n"
	<< indent << indent << "--no-cover                       Do not generate a cover for the document\n"
	<< indent << indent << "--no-toc                         Do not generate a table of contents\n"
	<< indent << indent << "--no-style                       Do not add any stylesheets to the document\n"
	<< indent << indent << "--no-images                      Do not add any images to the document\n"
	<< indent << indent << "--style-dir STYLEDIR             Specify a directory in which to search for stylesheets. If this argument is skipped\n"
	<< "                                     the default of DIRNAME/Styles is used\n"
	<< '\n' << indent << "Output control:\n"
	<< indent << indent << "-f, --force                      Overwrite the output file if it already exists\n"
	<< indent << indent << "-o, --outfile=OUTFILE            Set the file to write output to. If this argument is skipped\n"
	<< "                                     output is written to DIRNAME/book.epub\n"

	<< '\n' << indent << "Miscellaneous:\n"
	<< indent << indent << "-h, --help                       Print this help and exit\n"
	<< indent << indent << "-V, --version                    Print version information and exit\n"
	<< '\n';

	out << "Error codes:\n"
	<< indent << NOT_ENOUGH_ARGUMENTS << "\tNot enough arguments were provided\n"
	<< indent << DIR_DOES_NOT_EXIST << "\tThe target directory does not exist\n"
	<< indent << NO_ORDER_FILE << "\tNo " << orderFile << " was found in the target directory\n"
	<< indent << INVALID_OPTION << "\tAn unrecognised option was passed\n"
	<< indent << ARG_REQUIRED << "\tAn option that requires an argument was not passed one\n"
	<< indent << INVALID_CHAPTER_FILE << "\tA file listed in " << orderFile << " does not have a valid extension\n"
	<< indent << CHAPTER_DOES_NOT_EXIST << "\tA file listed in " << orderFile << " does not exist\n"
	<< indent << STYLEDIR_DOES_NOT_EXIST << "\tA directory for stylesheets does not exist and --no-style was not specified\n"
	<< indent << IMGDIR_DOES_NOT_EXIST << "\tA directory for images does not exist and --no-images wan not specified\n"
	<< indent << OUTFILE_EXISTS << "\tThe target file already exists and --force was not specified\n"
	<< indent << COULD_NOT_OPEN << "\tFailed to open output file\n"
	<< indent << UNKOWN_ERROR << "\tAn unknown error occurred, this should never occur\n"
	<< '\n';
}

