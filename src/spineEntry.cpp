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

#include "../headers/spineEntry.hpp"
#include "../headers/misc.hpp"

SpineEntry::SpineEntry(std::string_view filename, bool primary)
: idref{get_basename(filename)}, primary{primary}
{}

SpineEntry::operator std::string() const
{
	return "<itemref idref=\"" + this->idref + '"' + (this->primary ? "" : "linear=\"no\"") + "/>";
}

