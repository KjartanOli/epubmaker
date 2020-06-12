#include <fstream>
#include <limits>
#include <string_view>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex>
#include <limits>

#include "../headers/chapter.hpp"
#include "../headers/misc.hpp"

#include <iostream>

bool is_chapter(std::string_view filename)
{
	static const std::vector<std::string_view> extensions{"html", "xhtml"};

	std::string_view extension{get_extension(filename)};

	for (std::string_view ext : extensions)
	{
		if (extension == ext)
		{
			return true;
		}
	}

	return false;
}

std::string verify_chapter_existance(
	const std::vector<std::string>& chapters,
	std::string_view path
)
{
	for (std::string_view chapter : chapters)
	{
		if (!file_exists(path.data(), chapter.data()))
		{
			return std::string{chapter.data()};
		}
	}

	return "";
}

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

std::string get_title(std::string_view filename)
{
	const static std::regex titleRegex{"(.*)<title>(.+)</title>"};

	std::ifstream chapter{filename.data()};

	std::string line{};
	while (chapter)
	{
		std::getline(chapter, line);
		if (std::regex_search(line, titleRegex))
		{
			return std::regex_replace(line, titleRegex, "$2");
		}
	}

	return "";
}

