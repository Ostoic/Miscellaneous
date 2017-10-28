// Organize iPod Pictures.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "photo_sort\pid_sort.hpp"
#include "photo_sort\date_sort.hpp"

namespace fs = std::experimental::filesystem;

template <typename Comparison, typename Builder>
void organize_by_ids(const fs::path& directory, Comparison compare, Builder build)
{
	auto end = fs::directory_iterator();

 	const auto min = *std::min_element(fs::directory_iterator(directory), end, compare);
	const auto max = *std::max_element(fs::directory_iterator(directory), end, compare);

	/// Rename the directory to have the format:
	/// IMGS mmmm-MMMM,
	/// where mmmm denotes the minimum ipod photo id,
	/// and MMMM denotes the maximum ipod photo id.
	fs::path new_name = directory;
	new_name.replace_filename(build(min, max));

	try
	{
		fs::rename(directory, new_name);
		std::cout << "Renamed directory " << directory << " to " << new_name << '\n';
	}
	catch (std::exception& e)
	{
		std::cerr << "Unable to rename directory: " << e.what() << "\n";
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Invalid parameters\n";
		return 0;
	}

	fs::path p(argv[1]);

	std::cout << "Checking directory " << p << '\n';
	if (fs::exists(p) && fs::is_directory(p))
	{
		for (auto dir : fs::directory_iterator(p))
		{
			if (fs::is_directory(dir.path()))
			{
				organize_by_ids(dir, photo_sort::date_compare(), photo_sort::date_builder());
				std::cout << "Done.\n\n";
			}
		}
	}
	else
		std::cerr << "Directory " << p << " does not exist, or it is not a directory!";

    return 0;
}

