#ifndef NAV_POINT_H
	#define NAV_POINT_H

	#include <string>
	#include <string_view>

	class NavPoint
	{
		inline static unsigned int s_playOrder{1};

		private:
			unsigned int playOrder;
			std::string source;
			std::string id;
			std::string text;

		public:
			NavPoint(std::string_view source, std::string_view text);
			operator std::string() const;
	};
#endif

