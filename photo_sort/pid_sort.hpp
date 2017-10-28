#pragma once

#include <filesystem>
#include <string>

namespace photo_sort
{
	struct pid_compare { bool operator()(const std::experimental::filesystem::directory_entry a, const std::experimental::filesystem::directory_entry b) const; };
	struct pid_builder { std::string operator()(const std::experimental::filesystem::path&, const std::experimental::filesystem::path&) const; };
}

