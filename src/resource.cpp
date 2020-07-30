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

