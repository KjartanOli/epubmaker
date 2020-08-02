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

#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <regex>
#include <filesystem>

#include "../headers/misc.hpp"
#include "../headers/status.hpp"
#include "../headers/filter.hpp"
#include "../headers/mediaType.hpp"

namespace fs = std::filesystem;

std::string get_extension(std::string_view filename)
{
	std::vector<std::string> vec{split(filename, '.')};

	size_t length{vec.size()};
	if (length > 1)
	{
		return vec[length - 1];
	}
	return "";
}
std::vector<std::string> split(std::string_view string, const char separator)
{
	std::vector<std::string> results{};
	std::string tmp{};

	for (const char c : string)
	{
		if (c != separator)
		{
			tmp += c;
		}
		else
		{
			results.push_back(tmp);
			tmp = "";
		}
	}
	results.push_back(tmp);

	return results;
}

// get the name of a file without its extension or path
std::string get_basename(std::string_view filename)
{
	return split(strip_path(filename), '.')[0];
}

std::string strip_path(std::string_view filename)
{
	static const std::regex path{"(.+/)+(.+)"};
	return std::regex_replace(filename.data(), path, "$2");
}

std::string get_mediaType(std::string_view extension)
{
	static std::map<std::string_view, mediaType> mediaTypeMap
	{
		{"html", XHTML},
		{"xhtml", XHTML},
		{"png", PNG},
		{"css", CSS},
		{"jpg", JPEG},
		{"ncx", NCX},
	};

	switch (mediaTypeMap[extension])
	{
		case XHTML:
			return "application/xhtml+xml";

		case PNG:
			return "image/png";

		case JPEG:
			return "image/jpeg";

		case CSS:
			return "text/css";

		case NCX:
			return "application/x-dtbncx+xml";
	};

	// TODO pick a media-type to return as default.
	return "should not happen";
}

// add the provided file extension to the file's name only if it is not already there
std::string add_extension(std::string_view filename, std::string_view extension)
{
	std::regex ext{".+" + std::string{extension.data()} + '$'};

	if (std::regex_match(filename.data(), ext))
	{
		return filename.data();
	}
	else
	{
		return std::string{filename.data()} + std::string{extension.data()};
	}
}

std::vector<std::string> list_dir(std::string_view dirname)
{
	std::vector<std::string> results{};

	for (auto& file : fs::directory_iterator(dirname))
	{
		results.push_back(file.path());
	}

	return results;
}

std::vector<std::string> get_stylesheets(std::string_view dirname)
{
	return filter<std::string>(list_dir(dirname), [](std::string_view filename){return split(strip_path(filename), '.')[1] == "css";});
}

void version()
{
	static const short major{1};
	static const short minor{1};
	static const short hotfix{2};

	std::cout << "Epubmaker " << major << '.' << minor << '.' << hotfix << '\n'
	<< "This program comes with ABSOLUTELY NO WARRANTY.\n"
	<< "This is free software, and you are welcome to redistribute it\n";
}
