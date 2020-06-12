#ifndef BOOK_H
	#define BOOK_H

	#include <string>
	#include <string_view>
	#include <vector>
	#include <chrono>

	#include "manifestEntry.hpp"
	#include "spineEntry.hpp"
	#include "status.hpp"
	#include "navPoint.hpp"

	class Book
	{
		private:
			std::string path;
			std::string title;
			std::string author;
			std::string language;
			std::string identifier;
			std::string publisher;
			std::time_t date;
			std::vector<std::string> chapters;
			std::vector<ManifestEntry> manifest;
			std::vector<SpineEntry> spine;
			std::vector<NavPoint> navMap;

			// Utilities to generate the various structure files
			std::string generate_opf();
			std::string generate_ncx();
			std::string generate_container_file();

		public:
			Book(
				std::string_view path,
				std::string_view title,
				std::string_view author,
				std::string_view language,
				std::string_view identifier,
				std::string publisher,
				const std::vector<std::string> chapters,
				std::time_t date
					= std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
			);
			statusCode write(std::string_view filename, bool force);
	};
#endif

