#include <string>
#include <string_view>
#include <fstream>

#include "../headers/order.hpp"
#include "../headers/chapter.hpp"

std::string read_chapter_order(std::ifstream& order, std::vector<std::string>& chapters)
{
	std::string chapter{};

	while (order >> chapter)
	{
		if (!is_chapter(chapter))
		{
			return chapter;
		}
		chapters.push_back(chapter);
	}

	return "";
}
