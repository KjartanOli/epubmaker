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

#include <fstream>

#include <libzippp/libzippp.h>

#include "../headers/book.hpp"
#include "../headers/misc.hpp"
#include "../headers/status.hpp"
#include "../headers/fs.hpp"

Book::Book(
	const fs::path& path,
	std::string_view title,
	std::string_view author,
	std::string_view language,
	std::string_view identifier,
	std::string_view publisher,
	std::string_view description,
	const std::vector<std::string>& chapters,
	std::string_view coverFile,
	std::string_view styleDir,
	bool stylesheets,
	std::string_view imgDir,
	bool images,
	std::time_t date
	)
:
	path{path},
	title{title.data()},
	author{author.data()},
	language{language.data()},
	identifier{identifier.data()},
	publisher{publisher.data()},
	description{description.data()},
	chapters{},
	cover{Chapter{coverFile, "Cover"}},
	toc{Chapter{this->path / "tableofcontents.xhtml", "Table of Contents"}},
	stylesheets{},
	images{},
	date{date}
{
	this->chapters.reserve(chapters.size());

	for (const std::string& chapter : chapters)
	{
		this->chapters.emplace_back(this->path / chapter);
	}

	if (stylesheets)
	{
		std::vector<fs::path> stylesheets{get_stylesheets(this->path / styleDir)};
		this->stylesheets.reserve(stylesheets.size());

		for (const fs::path& filename : stylesheets)
		{
			this->stylesheets.emplace_back(filename);
		}
	}

	if (images)
	{
		std::vector<fs::path> images{list_dir(this->path / imgDir)};
		this->images.reserve(images.size());
		for (const fs::path& filename : images)
		{
			this->images.emplace_back(filename);
		}
	}
}

statusCode Book::write(std::string_view filename, bool force, bool cover, bool tableOfContents)
{
	fix_play_order(this->chapters, cover, tableOfContents);

	if (fs::exists(this->path / filename) && !force)
	{
		return OUTFILE_EXISTS;
	}

	libzippp::ZipArchive archive{(this->path / filename).c_str()};
	if (!archive.open(force ? libzippp::ZipArchive::NEW : libzippp::ZipArchive::WRITE))
	{
		return COULD_NOT_OPEN;
	}

	const std::string mimetype{"application/epub+zip"};
	archive.addData("mimetype", mimetype.c_str(), mimetype.size());

	archive.addEntry("OEBPS");
	archive.addEntry("OEBPS/Text");

	archive.addEntry("META-INF");

	std::string container{this->generate_container_file()};
	archive.addData("META-INF/container.xml", container.c_str(), container.length());

	std::string opf{this->generate_opf()};
	archive.addData("OEBPS/content.opf", opf.c_str(), opf.length());

	std::string ncx{this->generate_ncx(cover, tableOfContents)};
	archive.addData("OEBPS/toc.ncx", ncx.c_str(), ncx.length());

	if (cover)
	{
		if (fs::exists(this->cover.filepath))
		{
			archive.addFile("OEBPS/Text/" + this->cover.filename, this->cover.filepath);
		}
		else
		{
			this->generate_cover();
			archive.addFile("OEBPS/Text/" + this->cover.filename, this->path / this->cover.filename);
		}
	}

	if (tableOfContents)
	{
		this->generate_toc(cover);
		archive.addFile("OEBPS/Text/" + this->toc.filename, this->toc.filepath);
	}

	for (const Chapter& chapter : this->chapters)
	{
		archive.addFile("OEBPS/Text/" + chapter.filename, chapter.filepath);
	}

	if (this->stylesheets.size() > 0)
	{
		archive.addEntry("OEBPS/Styles");
		for (const Resource& stylesheet : this->stylesheets)
		{
			archive.addFile("OEBPS/Styles/" + stylesheet.filename, stylesheet.filepath);
		}
	}

	if (this->images.size() > 0)
	{
		archive.addEntry("OEBPS/Images");
		for (const Resource& image : this->images)
		{
			archive.addFile("OEBPS/Images/" + image.filename, image.filepath);
		}
	}

	archive.close();

	return NORMAL;
}

std::string Book::generate_opf() const
{
	std::stringstream opf{};

	opf << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n"
	<< "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"uuid_id\" version=\"2.0\">\n"
	<< "\t<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";
	if (this->title != "")
	{
		opf << "\t\t<dc:title>" << this->title << "</dc:title>\n";
	}
	if (this->language != "")
	{
		opf << "\t\t<dc:language>" << this->language << "</dc:language>\n";
	}
	if (this->identifier != "")
	{
		opf << "\t\t<dc:identifier id=\"uuid_id\">" << this->identifier << "</dc:identifier>\n";
	}
	if (this->author != "")
	{
		opf << "\t\t<dc:creator>" << this->author << "</dc:creator>\n";
	}
	if (this->publisher != "")
	{
		opf << "\t\t<dc:publisher>" << this->publisher << "</dc:publisher>\n";
	}
	if (this->description != "")
	{
		 opf << "\t\t<dc:description>" << this->description << "</dc:description>\n";
	}
	opf << "\t\t<dc:date>" << std::put_time(std::localtime(&(this->date)), "%Y-%m-%d") << "</dc:date>\n"
	<< "\t</metadata>\n\n"
	<< "\t<manifest>\n"
	<< "\t\t<item href=\"toc.ncx\" id=\"toc\" media-type=\"application/x-dtbncx+xml\"/>"
	<< "\t\t<!-- Chapters -->\n"
	<< "\t\t" << this->cover.get_manifest_entry() << '\n'
	<< "\t\t" << this->toc.get_manifest_entry() << '\n';

	for (const Chapter& chapter : this->chapters)
	{
		opf << "\t\t" << chapter.get_manifest_entry() << '\n';
	}

	if (this->stylesheets.size() > 0)
	{
		opf << "\n\n\t\t<!-- Stylesheets -->\n";
		for (const Resource& stylesheet : this->stylesheets)
		{
			opf << "\t\t" << stylesheet.get_manifest_entry() << '\n';
		}
	}

	if (this->images.size() > 0)
	{
		opf << "\n\t\t<!-- Images -->\n";
		for (const Resource& image : this->images)
		{
			opf << "\t\t" << image.get_manifest_entry() << '\n';
		}
	}

	opf << "\t</manifest>\n\n\t<spine toc=\"toc\">\n"
	<< "\t\t" << this->cover.get_spine_entry() << '\n'
	<< "\t\t" << this->toc.get_spine_entry() << '\n';

	for (const Chapter& chapter : this->chapters)
	{
		opf << "\t\t" << chapter.get_spine_entry() << '\n';
	}
	opf << "\t</spine>\n";

	opf << "</package>\n";

	return opf.str();
}

std::string Book::generate_ncx(bool cover, bool tableOfContents) const
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

	if (cover)
	{
		ncx << "\t\t" << this->cover.get_navPoint() << "\n\n";
	}

	if (tableOfContents)
	{
		ncx << "\t\t" << this->toc.get_navPoint() << "\n\n";
	}

	for (const Chapter& chapter : this->chapters)
	{
		ncx << "\t\t" << chapter.get_navPoint() << "\n\n";
	}

	ncx << "\t</navMap>\n</ncx>\n";

	return ncx.str();
}

std::string Book::generate_container_file() const
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

void Book::generate_toc(bool cover) const
{
	std::ofstream toc{this->toc.filepath};

	toc << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
	<< "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
	<< "\t<head>\n\t\t<title>Table of Contents</title>\n\t\t"
	<< "<meta content=\"http://www.w3.org/1999/xhtml; charset=utf-8\" http-equiv=\"Content-Type\"/>\n"
	<< "\t</head>\n\n"
	<< "\t<body>\n"
	<< "\t\t<h1>Table of Contents</h1>\n"
	<< "\t\t<h2>" << this->title << "</h2>\n"
	<< "\t\t<h3>" << this->author << "</h3>\n\t\t<br/><br/>\n\n";
	if (cover)
	{
		toc << "\t\t" << this->cover.get_toc_entry() << "<br/>\n";
	}

	toc << "\t\t" << this->toc.get_toc_entry() << "<br/>\n";

	for (const Chapter& chapter : this->chapters)
	{
		toc << "\t\t" << chapter.get_toc_entry() << "<br/>\n";
	}

	toc << "\t</body>\n</html>\n";
}

void Book::generate_cover() const
{
	std::ofstream cover{this->path / this->cover.filename};

	cover << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
	<< "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
	<< "\t<head>\n\t\t<title>" << this->title << "</title>\n"
	<< "\t\t<meta content=\"http://www.w3.org/1999/xhtml; charset=utf-8\" http-equiv=\"Content-Type\"/>\n"
	<< "\t</head>\n\n\t<body>\n\t\t<h1>" << this->title << "</h1>\n"
	<< "\t\t<h2>By " << this->author << "</h2>\n\t</body>\n</html>\n";
}

