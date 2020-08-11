#/*
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

#ifndef RESOURCE_H
	#define RESOURCE_H

	#include <string>

	#include "manifestEntry.hpp"
	#include "fs.hpp"

	class Resource
	{
		public:
			const fs::path filepath;
			const std::string filename;

		protected:
			const ManifestEntry manifestEntry;

		public:
			Resource(const fs::path& filepath);
			std::string get_manifest_entry() const;

	};
#endif
