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

#include <string_view>

#include "../headers/resource.hpp"
#include "../headers/manifestEntry.hpp"
#include "../headers/misc.hpp"

Resource::Resource(std::string_view filepath)
:
	filepath{filepath},
	filename{strip_path(filepath)},
	manifestEntry{ManifestEntry{this->filename}}
{}

std::string Resource::get_manifest_entry() const
{
	return static_cast<std::string>(this->manifestEntry);
}

