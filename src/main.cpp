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

#include "../headers/filter.hpp"
#include "../headers/chapter.hpp"
#include "../headers/status.hpp"
#include "../headers/args.hpp"
#include "../headers/misc.hpp"
#include "../headers/manifestEntry.hpp"
#include "../headers/spineEntry.hpp"
#include "../headers/navPoint.hpp"
#include "../headers/book.hpp"

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

	if (!dir_exists(args.path))
	{
		std::cerr << "Directory " << args.path << " Does not exist\n";
		return DIR_DOES_NOT_EXIST;
	}

	if (!file_exists(args.path.data(), orderFile.data()))
	{
		std::cerr << "Directory " << args.path << " Does not contain a " << orderFile << " file\n";
		return NO_ORDER_FILE;
	}

	std::ifstream order{args.path + orderFile.data()};
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
			return CHAPTER_DOES_NOT_EXSIST;
	}

	Book book{args.path, args.title, args.author, args.language, args.identifier, args.publisher, chapters};
	statusCode status{book.write(args.outfile + ".epub", args.force)};

	switch (status)
	{
		case OUTFILE_EXISTS:
			std::cerr << "Target file " << args.outfile << " already exists and --force was not specified\n";
			return OUTFILE_EXISTS;

		case COULD_NOT_REMOVE:
			std::cerr << "Could not remove " << args.outfile << '\n';
			return COULD_NOT_REMOVE;

		case NORMAL:
			break;

		default:
			// This path should never be executed but not having it would cause a warning.
			std::cerr << "Unkown error\n";
			return UNKOWN_ERROR;
	}

	return NORMAL;
}

void help(std::ostream& out)
{
	// TODO write usage examples and option explainations

	out << "Erorr codes\n\t" << NOT_ENOUGH_ARGUMENTS << "\tNot enough arguments vere provided\n"
	<< '\t' << DIR_DOES_NOT_EXIST << "\tThe target directory does not exist\n"
	<< '\t' << NO_ORDER_FILE << "\tNo " << orderFile << " was found in the target directory\n"
	<< '\t' << INVALID_OPTION << "\tAn unrecognized option was passed\n"
	<< '\t' << ARG_REQUIRED << "\tAn option that requiers an argument was not passed one\n"
	<< '\t' << INVALID_CHAPTER_FILE << "\tA file listed in " << orderFile << " does not have a valid extension\n"
	<< '\t' << CHAPTER_DOES_NOT_EXSIST << "\tA file listed in " << orderFile << " does not exist\n"
	<< '\t' << OUTFILE_EXISTS << "\tThe target file already exists and --force was not specified\n"
	<< '\t' << COULD_NOT_REMOVE << "\tCould not remove the target file when running with --force\n"
	<< '\t' << UNKOWN_ERROR << "\tAn unknown error occured, this should never occur\n";
}
