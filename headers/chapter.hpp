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

#ifndef CHAPTER_H
	#define CHAPTER_H

	#include <string>
	#include <string_view>
	#include <vector>

	#include "resource.hpp"
	#include "navPoint.hpp"
	#include "manifestEntry.hpp"
	#include "spineEntry.hpp"

	class Chapter : public Resource
	{
		private:
			const std::string title;
			const SpineEntry spineEntry;
			NavPoint navPoint;

			std::string get_title();
		public:
			Chapter(std::string_view filepath);
			Chapter(std::string_view filepath, std::string_view title);
			std::string get_spine_entry() const;
			std::string get_navPoint() const;
			std::string get_toc_entry() const;

		// If --no-cover and/or --no-toc are passed the playOrder value of all NavPoints
		// will be incorrect because NavPoints for those two files have been initialized
		friend void fix_play_order(std::vector<Chapter>& chapters, bool cover, bool tableOfContents);
	};


	bool is_chapter(std::string_view file);
	std::string verify_chapter_existance
	(
		const std::vector<std::string>& chapters,
		std::string_view path
	);
	std::string get_title(std::string_view filename);

#endif

