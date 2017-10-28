#pragma once

#include <filesystem>
#include <string>

namespace photo_sort
{
	struct date_compare { bool operator()(const std::experimental::filesystem::directory_entry&, const std::experimental::filesystem::directory_entry&) const; };
	struct date_builder { std::string operator()(const std::experimental::filesystem::path&, const std::experimental::filesystem::path&) const; };
}
