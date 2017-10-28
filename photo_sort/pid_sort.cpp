#include "stdafx.h"
#include "pid_sort.hpp"
#include "utility.hpp"

namespace photo_sort
{
	namespace fs = std::experimental::filesystem;

	unsigned int get_id(const fs::path& p)
	{
		unsigned int id = 0;

		std::smatch matches;
		std::regex regex("IMG_[0-9]{4}\\.");
		const auto searchString = p.filename().string();

		// Make sure the filename matches the given pattern
		if (std::regex_search(searchString, matches, regex))
		{
			std::stringstream idString(static_cast<std::string>(matches[0]).substr(4, 9));
			idString >> id;
		}

		return id;
	}

	std::string pid_builder::operator()(const std::experimental::filesystem::path& first, const std::experimental::filesystem::path& last) const
	{
		return std::to_string(get_id(first)) + " - " + std::to_string(get_id(last));
	}

	bool pid_compare::operator()(const fs::directory_entry a, const fs::directory_entry b) const
	{
		return (utility::matches_regex(a) && utility::matches_regex(b)) && (get_id(a) < get_id(b));
	}
}