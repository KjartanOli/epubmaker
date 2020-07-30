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

#include <fstream>
#include <string_view>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex>
#include <filesystem>

#include "../headers/chapter.hpp"
#include "../headers/misc.hpp"

namespace fs = std::filesystem;

bool is_chapter(std::string_view filepath)
{
	static const std::vector<std::string_view> extensions{"html", "xhtml"};

	std::string_view extension{get_extension(filepath)};

	for (std::string_view ext : extensions)
	{
		if (extension == ext)
		{
			return true;
		}
	}

	return false;
}

std::string verify_chapter_existance(
	const std::vector<std::string>& chapters,
	std::string_view path
)
{
	for (const std::string& chapter : chapters)
	{
		std::string filepath{std::string{path.data()} + chapter};
		if (!fs::exists(filepath))
		{
			return filepath;
		}
	}

	return "";
}

Chapter::Chapter(std::string_view filename)
:
	Resource{filename},
	title{this->get_title()},
	spineEntry{strip_path(filename)},
	navPoint{filename, this->title}
{}

Chapter::Chapter(std::string_view filepath, std::string_view title)
:
	Resource{filepath},
	title{title},
	spineEntry{strip_path(filepath)},
	navPoint{filepath, this->title}
{}

std::string Chapter::get_title()
{
	const static std::regex titleRegex{"(.*)<title>(.+)</title>"};

	std::ifstream chapter{this->filepath};

	std::string line{};
	while (chapter)
	{
		std::getline(chapter, line);
		if (std::regex_search(line, titleRegex))
		{
			return std::regex_replace(line, titleRegex, "$2");
		}
	}

	return get_basename(this->filename);

}

std::string Chapter::get_navPoint() const
{
	return static_cast<std::string>(this->navPoint);
}

std::string Chapter::get_spine_entry() const
{
	return static_cast<std::string>(this->spineEntry);
}

std::string Chapter::get_toc_entry() const
{
	return "<a href=\"../Text/" + this->filename + "\">" + this->title + "</a>";
}

