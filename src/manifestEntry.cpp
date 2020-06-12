#include <ostream>
#include <string>
#include <string_view>

#include "../headers/manifestEntry.hpp"
#include "../headers/misc.hpp"

#include <iostream>

ManifestEntry::ManifestEntry(std::string_view filename)
:
	id{get_basename(filename)},
	mediaType{get_mediaType(get_extension(filename))}
{
	if (this->mediaType == "application/xhtml+xml")
	{
		this->href = ("Text/" + std::string{filename});
	}

	if (filename == "toc.ncx")
	{
		this->href = "toc.ncx";
	}
}

ManifestEntry::operator std::string() const
{
	return "<item href=\"" +  this->href + "\" id=\"" + this->id + "\" media-type=\"" + this->mediaType + "\"/>";
}

