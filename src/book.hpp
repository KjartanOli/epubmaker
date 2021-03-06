/*
 * Copyright (C) 2020, 2021 Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
 * Author: Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
 *
 * This file is a part of Epubmaker
 * Epubmaker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Epubmaker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOOK_H
#define BOOK_H

#include <deque>
#include <string_view>
#include <string>
#include <vector>
#include <chrono>

#include "resource.hpp"
#include "status.hpp"
#include "chapter.hpp"
#include "fs.hpp"

class Book
{
private:
	fs::path path;
	std::string title;
	std::string author;
	std::string language;
	std::string identifier;
	std::string publisher;
	std::string description;
	std::vector<Chapter> chapters;
	Chapter cover;
	Chapter toc;
	std::vector<Resource> stylesheets;
	std::vector<Resource> images;
	std::string date;

	// Utilities to generate the various structure files
	std::string generate_opf(bool cover, bool tableOfContents) const;
	std::string generate_ncx(bool cover, bool tableOfContents) const;
	std::string generate_container_file() const;
	void  generate_toc(bool cover) const;
	void generate_cover() const;

public:
	Book(
		const fs::path& path,
		std::string_view title,
		std::string_view author,
		std::string_view language,
		std::string_view identifier,
		std::string_view publisher,
		std::string_view description,
		const std::vector<fs::path>& chapters,
		std::string_view coverFile,
		std::deque<fs::path>& styleDirs,
		bool stylesheets,
		std::deque<fs::path>& imgDirs,
		bool images,
		std::string_view date
		);
	statusCode write(std::string_view filename, bool force, bool cover, bool tableOfContents);
};
#endif

