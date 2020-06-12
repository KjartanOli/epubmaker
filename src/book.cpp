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

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>
#include <cstdio>

#include <libzippp/libzippp.h>

#include "../headers/book.hpp"
#include "../headers/manifestEntry.hpp"
#include "../headers/spineEntry.hpp"
#include "../headers/navPoint.hpp"
#include "../headers/misc.hpp"
#include "../headers/status.hpp"

#include <iostream>

Book::Book(
	std::string_view path,
	std::string_view title,
	std::string_view author,
	std::string_view language,
	std::string_view identifier,
	std::string publisher,
	std::vector<std::string> chapters,
	std::time_t date
)
:
	path{path.data()},
	title{title.data()},
	author{author.data()},
	language{language.data()},
	identifier{identifier.data()},
	date{date},
	chapters{chapters},
	manifest{},
	spine{},
	navMap{}
{
	for (std::string_view chapter : chapters)
	{
		this->manifest.push_back(ManifestEntry{chapter});
		this->spine.push_back(SpineEntry{chapter});
		this->navMap.push_back(NavPoint{chapter, get_basename(chapter)});
	}

	this->manifest.push_back(ManifestEntry{"toc.ncx"});
}

statusCode Book::write(std::string_view filename, bool force)
{
	if (file_exists(filename))
	{
		if (!force)
		{
			return OUTFILE_EXISTS;
		}
		else
		{
			int err{std::remove(filename.data())};
			if (err)
			{
				return COULD_NOT_REMOVE;
			}
		}
	}

	libzippp::ZipArchive archive{this->path + std::string{filename.data()}};
	if (!archive.open(force ? libzippp::ZipArchive::NEW : libzippp::ZipArchive::WRITE))
	{
		return COULD_NOT_OPEN;
	}

	const std::string mimetype{"application/epub+zip"};
	archive.addData("mimetype", mimetype.c_str(), mimetype.size());

	archive.addEntry("OEBPS");
	archive.addEntry("OEBPS/Text");
	archive.addEntry("OEBPS/Styles");
	archive.addEntry("OEBPS/Images");
	archive.addEntry("META-INF");

	std::string container{this->generate_container_file()};
	archive.addData("META-INF/container.xml", container.c_str(), container.length());

	std::string ncx{this->generate_ncx()};
	archive.addData("OEBPS/toc.ncx", ncx.c_str(), ncx.length());

	std::string opf{this->generate_opf()};
	archive.addData("OEBPS/content.opf", opf.c_str(), opf.length());

	for (const std::string& chapter : chapters)
	{
		archive.addFile("OEBPS/Text/" + chapter, chapter);
	}

	archive.close();

	return NORMAL;
}

std::string Book::generate_opf()
{
	std::stringstream opf{};

	opf << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n"
	<< "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"uuid_id\" version=\"2.0\">\n"
	<< "\t<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"
	<< "\t\t<dc:title>" << this->title << "</dc:title>\n"
	<< "\t\t<dc:language>" << this->language << "</dc:language>\n"
	<< "\t\t<dc:identifier id=\"uuid_id\">" << this->identifier << "</dc:identifier>\n"
	<< "\t\t<dc:creator>" << this->author << "</dc:creator>\n"
	<< "\t\t<dc:publisher>" << this->publisher << "</dc:publisher>\n"
	<< "\t\t<dc:date>" << std::put_time(std::localtime(&(this->date)), "%Y-%m-%d") << "</dc:date>\n"
	<< "\t</metadata>\n"
	<< "\t<manifest>\n";

	for (const ManifestEntry& entry : this->manifest)
	{
		opf << "\t\t" << static_cast<std::string>(entry) << '\n';
	}
	opf << "\t</manifest>\n\t<spine toc=\"toc\">\n";

	for (const SpineEntry& entry : this->spine)
	{
		opf << "\t\t" << static_cast<std::string>(entry) << '\n';
	}
	opf << "\t</spine>\n";

	opf << "</package>\n";

	return opf.str();
}

std::string Book::generate_ncx()
{
	std::stringstream ncx{};

	ncx << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
	<< "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"" << this->language << "\">\n"
	<< "\t<head>\n\t\t<meta content=\"" << this->identifier << "\" name=\"dtb:uid\"/>\n"
	<< "\t\t<meta content=\"2\" name=\"dtb:depth\"/>\n"
	<< "\t\t<meta content=\"0\" name=\"dtb:totalPageCount\"/>\n"
	<< "\t\t<meta content=\"0\" name=\"dtb:maxPageNumber\"/>\n"
	<< "\t</head>\n\t<docTitle><text>" << this->title << "</text></docTitle>\n"
	<< "\t<navMap>\n";

	for (const NavPoint& point : this->navMap)
	{
		ncx << "\t\t" << static_cast<std::string>(point) << '\n';
	}

	ncx << "\t</navMap>\n</ncx>\n";

	return ncx.str();
}

std::string Book::generate_container_file()
{
	std::stringstream container{};

	container << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\n"
	<< "\t<rootfiles>\n"
	<< "\t\t<rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/>\n"
	<< "\t</rootfiles>\n"
	<< "</container>\n";

	return container.str();
}

