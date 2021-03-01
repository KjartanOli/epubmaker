/*
 * Copyright (C) 2020, 2021  Ágústsson, Kjartan Óli <kjartanoli@protonmail.com>
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

#ifndef NAV_POINT_H
	#define NAV_POINT_H

	#include <string>
	#include <vector>

	#include "fs.hpp"

	class Chapter;

	class NavPoint
	{
		inline static unsigned int s_playOrder{1};

		private:
			unsigned int playOrder;
			std::string source;
			std::string id;
			std::string text;

		public:
			NavPoint(const fs::path& filepath, std::string_view title);
			operator std::string() const;

		// If --no-cover and/or --no-toc are passed the playOrder value of all NavPoints
		// will be incorrect because NavPoints for those two files have been initialized
		friend void fix_play_order(std::vector<Chapter>& chapters, bool cover, bool tableOfContents);
	};
#endif

