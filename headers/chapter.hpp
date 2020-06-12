#ifndef CHAPTER_H
	#define CHAPTER_H

	#include <string>
	#include <string_view>
	#include <vector>

	bool is_chapter(std::string_view file);
	std::string verify_chapter_existance
	(
		const std::vector<std::string>& chapters,
		std::string_view path
	);
	std::string read_chapter_order(std::ifstream& order, std::vector<std::string>& chapters);
	std::string get_title(std::string_view filename);

#endif

