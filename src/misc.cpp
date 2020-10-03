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
#include <map>
#include <regex>

#include "../headers/misc.hpp"
#include "../headers/status.hpp"
#include "../headers/filter.hpp"
#include "../headers/mediaType.hpp"
#include "../headers/fs.hpp"

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

std::vector<fs::path> list_dir(const fs::path& dirname)
{
	std::vector<fs::path> results{};

	for (auto& file : fs::directory_iterator(dirname))
	{
		results.push_back(file.path());
	}

	return results;
}

std::vector<fs::path> get_stylesheets(const fs::path& dirname)
{
	return filter<fs::path>(list_dir(dirname), [](const fs::path& file){return file.extension() == ".css";});
}

void version()
{
	static const short major{1};
	static const short minor{1};
	static const short hotfix{3};

	std::cout << "Epubmaker " << major << '.' << minor << '.' << hotfix << '\n'
	<< "This program comes with ABSOLUTELY NO WARRANTY.\n"
	<< "This is free software, and you are welcome to redistribute it\n";
}
