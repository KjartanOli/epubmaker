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

#ifndef EXTENSION_H
	#define EXTENSION_H

	#include <string_view>
	#include <vector>

	std::string get_extension(std::string_view filename);
	bool dir_exists(std::string_view path);
	bool file_exists(std::string_view filename);
	bool file_exists(const std::string& path, const std::string& filename);
	std::vector<std::string> split(std::string_view string, const char separator = ' ');
	std::string get_basename(std::string_view filename);
	std::string get_mediaType(std::string_view extension);

	enum mediaType
	{
		XHTML,
		CSS,
		JPEG,
		PNG,
		NCX,
	};
#endif

