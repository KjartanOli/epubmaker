#ifndef ORDER_H
	#define ORDER_H

	#include <string>
	#include <vector>
	#include <fstream>

	#include "chapter.hpp"

	std::string read_chapter_order(std::ifstream& order, std::vector<std::string>& chapters);
#endif

