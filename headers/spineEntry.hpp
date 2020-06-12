#ifndef SPINE_ENTRY_H
	#define SPINE_ENTRY_H

	#include <string>
#include <string_view>

	class SpineEntry
	{
		private:
			std::string idref;
			bool primary;

		public:
			SpineEntry(std::string_view id, bool primary = true);
			operator std::string() const;
	};
#endif

