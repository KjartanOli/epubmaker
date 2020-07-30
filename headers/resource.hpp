#ifndef RESOURCE_H
	#define RESOURCE_H

	#include <string>

	#include "manifestEntry.hpp"

	class Resource
	{
		public:
			const std::string filepath;
			const std::string filename;

		protected:
			const ManifestEntry manifestEntry;

		public:
			Resource(std::string_view filepath);
			std::string get_manifest_entry() const;

	};
#endif
