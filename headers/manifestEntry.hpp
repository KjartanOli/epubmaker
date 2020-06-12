#ifndef MANIFEST_ENTRY_H
	#define MANIFEST_ENTRY_H

	#include <string>
#include <string_view>

	class ManifestEntry
	{
		private:
			std::string href;
			std::string id;
			std::string mediaType;

		public:
			ManifestEntry(std::string_view filename);
			operator std::string() const;
	};
#endif

