#ifndef EXTENSION_H
	#define EXTENSION_H

	#include <string_view>
	#include <vector>

	std::string get_extension(std::string_view filename);
	bool dir_exists(std::string_view path);
	bool file_exists(std::string_view filename);
	bool file_exists(const std::string& path, const std::string& filename);
	std::vector<std::string> split(std::string_view string, const char separator = ' ');
	std::string get_basename(std::string_view filename);
	std::string get_mediaType(std::string_view extension);

	enum mediaType
	{
		XHTML,
		CSS,
		JPEG,
		PNG,
		NCX,
	};
#endif

