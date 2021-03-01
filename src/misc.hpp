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

#ifndef EXTENSION_H
	#define EXTENSION_H

	#include <string_view>
	#include <vector>

	#include "fs.hpp"

	std::vector<std::string> split(std::string_view string, const char separator = ' ');
	std::string get_mediaType(std::string_view extension);
	std::string add_extension(std::string_view filename, std::string_view extension);
	std::vector<fs::path> list_dir(const fs::path& dirname);
	std::vector<fs::path> get_stylesheets(const fs::path& dirname);

#endif

