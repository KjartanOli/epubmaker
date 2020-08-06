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

#include <ostream>
#include <string>
#include <string_view>
#include <map>

#include "../headers/manifestEntry.hpp"
#include "../headers/misc.hpp"
#include "../headers/resourceType.hpp"

ManifestEntry::ManifestEntry(std::string_view filename)
:
	mediaType{get_mediaType(get_extension(filename))},
	id{filename}
{
	static std::map<std::string, resourceType> resourceTypeMap
	{
		{"application/xhhml+xhml", CHAPTER},
		{"image/png", IMAGE},
		{"image/jpeg", IMAGE},
		{"text/css", STYLESHEET}
	};

	switch (resourceTypeMap[this->mediaType])
	{
		case CHAPTER:
			this->href = "Text/" + std::string{filename};
			break;

		case IMAGE:
			this->href = "Images/"  + std::string{filename};
			break;

		case STYLESHEET:
			this->href = "Styles/" + std::string{filename};
			break;
	}
/*
	if (this->mediaType == "application/xhtml+xml")
	{
		this->href = ("Text/" + std::string{filename});
	}

	if (filename == "toc.ncx")
	{
		this->href = "toc.ncx";
	}
*/
}

ManifestEntry::operator std::string() const
{
	return "<item href=\"" +  this->href + "\" id=\"" + this->id + "\" media-type=\"" + this->mediaType + "\"/>";
}

