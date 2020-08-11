/*
 * Copyright (C) 2020  Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
 * Author: Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
 *
 * This file is a part of EpubMaker
 * EpubMaker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARGS_H
	#define ARGS_H

	#include <string_view>
	#include <string>
	#include <vector>

	#include "status.hpp"
	#include "defaults.hpp"
	#include "fs.hpp"

	struct arguments
	{
		std::string author;
		std::string title;
		std::string date;
		std::string outfile{defaults::outfile};
		std::string language;
		std::string identifier;
		std::string publisher;
		std::string unknownOption;
		std::string requiresArgument;
		std::string description;
		fs::path path;
		std::string styleDir{defaults::styledir};
		std::string imgDir{defaults::imagedir};
		std::string coverFile{defaults::coverfile};

		bool help{false};
		bool version{false};
		bool force{false};
		bool stylesheets{true};
		bool cover{true};
		bool toc{true};
		bool images{true};
	};

	enum argumentValues
	{
		NOTOPTION,
		AUTHOR,
		DATE,
		HELP,
		TITLE,
		LANGUAGE,
		IDENTIFIER,
		PUBLISHER,
		DESCRIPTION,
		VERSION,
		OUTFILE,
		FORCE,
		STYLEDIR,
		NOSTYLE,
		IMGDIR,
		COVER,
		NOCOVER,
		NOIMAGES,
		NOTOC,
	};

	statusCode parse_args(arguments& args, int argc, char* argv[]);
	bool is_argument(std::string_view argument);
#endif
