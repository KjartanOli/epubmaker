#include <string>
#include <string_view>

#include "../headers/navPoint.hpp"
#include "../headers/misc.hpp"
#include "../headers/chapter.hpp"

NavPoint::NavPoint(std::string_view source, std::string_view text)
: playOrder{NavPoint::s_playOrder++},
	source{source.data()},
	id{get_basename(source)},
	text{get_title(source)}
{}

NavPoint::operator std::string() const
{
	return "<navPoint id=\"" + this->id + "\" playOrder=\"" + std::to_string(this->playOrder) + "\">\n\t\t\t<navLabel><text>" + this->text + "</text></navLabel>\n\t\t\t<content src=\"Text/" + this->source + "\"/>\n\t\t</navPoint>";
}

