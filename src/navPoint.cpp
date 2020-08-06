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

#include <string>
#include <string_view>

#include "../headers/navPoint.hpp"
#include "../headers/misc.hpp"
#include "../headers/chapter.hpp"

NavPoint::NavPoint(std::string_view source, std::string_view title)
: playOrder{NavPoint::s_playOrder++},
	source{strip_path(source)},
	id{strip_path(source)},
	text{title}
{}

NavPoint::operator std::string() const
{
	return "<navPoint id=\"" + this->id + "\" class=\"chapter\" playOrder=\"" +
		std::to_string(this->playOrder) + "\">\n\t\t\t<navLabel><text>" +
		this->text + "</text></navLabel>\n\t\t\t<content src=\"Text/" +
		this->source + "\"/>\n\t\t</navPoint>";
}

// If --no-cover and/or --no-toc are passed the playOrder value of all NavPoints
// will be incorrect because NavPoints for those two files have been initialized
void fix_play_order(std::vector<Chapter>& chapters, bool cover, bool tableOfContents)
{
	int modifier{2 - cover - tableOfContents};

	for (Chapter& chapter : chapters)
	{
		chapter.navPoint.playOrder -= modifier;
	}
}
