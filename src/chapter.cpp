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

#include <filesystem>
#include <fstream>
#include <string_view>
#include <string>
#include <vector>
#include <regex>

#include "../headers/chapter.hpp"
#include "../headers/misc.hpp"
#include "../headers/fs.hpp"

bool is_chapter(const fs::path& filepath)
{
	static const std::vector<std::string_view> extensions{".html", ".xhtml"};

	std::string_view extension{filepath.extension().c_str()};

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
	std::vector<std::string>& chapters,
	const fs::path& path
)
{
	for (fs::path chapter : chapters)
	{
		// check if the chapter has a parent path or is just a filename
		bool parentPath{chapter.has_parent_path()};

		if (!chapter.is_absolute())
		{
			chapter = path / chapter;
		}

		if (!fs::exists(chapter))
		{
			if (!parentPath)
			{
				chapter = chapter.parent_path() / "Text" / chapter.filename();
				if (!fs::exists(chapter))
				{
					return chapter.filename();
				}
			}
			else
			{
				return chapter;
			}
		}
	}

	return "";
}

Chapter::Chapter(const fs::path& filepath)
:
	Resource{filepath},
	title{this->get_title()},
	spineEntry{filepath.filename().c_str()},
	navPoint{filename, this->title}
{}

Chapter::Chapter(const fs::path& filepath, std::string_view title)
:
	Resource{filepath},
	title{title},
	spineEntry{filepath.filename().c_str()},
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

	return this->filename;

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

