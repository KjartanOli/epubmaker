#include <string>
#include <string_view>

#include "../headers/spineEntry.hpp"
#include "../headers/misc.hpp"

SpineEntry::SpineEntry(std::string_view id, bool primary)
: idref{get_basename(id)}, primary{primary}
{}

SpineEntry::operator std::string() const
{
	return "<itemref idref=\"" + this->idref + '"' + (this->primary ? "" : "linear=\"no\"") + "/>";
}

