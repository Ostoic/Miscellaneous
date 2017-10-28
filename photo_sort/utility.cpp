#include "stdafx.h"
#include "utility.hpp"

namespace utility
{
	namespace fs = std::experimental::filesystem;
	bool matches_regex(const fs::path& p, const std::regex& regex)
	{
		const auto searchString = p.filename().string();

		// Make sure the filename matches the given pattern
		return std::regex_search(searchString, regex);
	}
}