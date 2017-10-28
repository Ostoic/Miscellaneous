#pragma once

#include <filesystem>
#include <regex>

namespace utility
{
	bool matches_regex(const std::experimental::filesystem::path& p, const std::regex& regex = std::regex("IMG_[0-9]{4}\\."));
}