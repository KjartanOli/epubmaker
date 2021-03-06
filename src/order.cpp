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

#include <filesystem>
#include <string>
#include <string_view>
#include <fstream>

#include "order.hpp"
#include "chapter.hpp"
#include "fs.hpp"

fs::path read_chapter_order(std::ifstream& order, std::vector<fs::path>& chapters)
{
	fs::path chapter{};

	while (order >> chapter)
	{
		if (!is_chapter(chapter))
		{
			return chapter;
		}
		chapters.push_back(chapter);
	}

	return "";
}
