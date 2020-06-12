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

#include "../headers/misc.hpp"
#include "../headers/status.hpp"

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

bool dir_exists(std::string_view path)
{
	struct stat st{};

	return (((stat(path.data(), &st) == 0)) && ((st.st_mode & S_IFMT) == S_IFDIR));
}

bool file_exists(std::string_view filename)
{
	struct stat st{};
	return (stat(filename.data(), &st) == 0);
}

bool file_exists(const std::string& path, const std::string& filename)
{
	struct stat st{};
	return (stat(std::string{path + '/' + filename}.c_str(), &st) == 0);
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

// get the name of a file without its extension
std::string get_basename(std::string_view filename)
{
	return split(filename, '.')[0];
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

void version()
{
	static const short major{1};
	static const short minor{0};
	static const short hotfix{0};

	std::cout << "EpubMaker " << major << '.' << minor << '.' << hotfix << '\n';
}
