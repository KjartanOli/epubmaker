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

#ifndef STATUS_H
	#define STATUS_H

	enum statusCode
	{
		NORMAL,
		NOT_ENOUGH_ARGUMENTS,
		DIR_DOES_NOT_EXIST,
		NO_ORDER_FILE,
		INVALID_OPTION,
		INCOMPATABLE_OPTIONS,
		ARG_REQUIRED,
		INVALID_CHAPTER_FILE,
		CHAPTER_DOES_NOT_EXIST,
		STYLEDIR_DOES_NOT_EXIST,
		IMGDIR_DOES_NOT_EXIST,
		OUTFILE_EXISTS,
		COULD_NOT_OPEN,
		COVER_IMAGE_DOES_NOT_EXIST,
		UNKOWN_ERROR,
	};

#endif
