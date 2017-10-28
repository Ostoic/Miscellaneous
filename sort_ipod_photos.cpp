#include <filesystem>
#include <iostream>
#include <sstream>
#include <regex>

namespace fs = std::experimental::filesystem;

bool matches_regex(const fs::path& p)
{
	std::regex regex("IMG_[0-9]{4}\.");
	const auto searchString = p.filename().string();

	// Make sure the filename matches the given pattern
	return std::regex_search(searchString, regex);
}

/// Get the ipod photo id 
/// Ex: IMG_7333 has id 7333
unsigned int get_id(const fs::path& p)
{
	unsigned int id = 0;

	std::smatch matches;
	std::regex regex("IMG_[0-9]{4}\.");
	const auto searchString = p.filename().string();

	// Make sure the filename matches the given pattern
	if (std::regex_search(searchString, matches, regex))
	{
		std::stringstream idString(static_cast<std::string>(matches[0]).substr(4, 9));
		idString >> id;
	}

	return id;
}

/// Build directory string
std::string build_iPod_dir(const fs::path& first, const fs::path& last)
{
	return std::to_string(get_id(first)) + " - " + std::to_string(get_id(last));
}

void organize_by_ids(const fs::path& directory)
{
	auto end = fs::directory_iterator();

	auto comparison = [](const auto a, const auto b)
	{
		return (matches_regex(a) && matches_regex(b)) && (get_id(a) < get_id(b));
	};

	const auto min = *std::min_element(fs::directory_iterator(directory), end, comparison);
	const auto max = *std::max_element(fs::directory_iterator(directory), end, comparison);

	/// Rename the directory to have the format:
	/// IMGS mmmm-MMMM,
	/// where mmmm denotes the minimum ipod photo id,
	/// and MMMM denotes the maximum ipod photo id.
	fs::path new_name = directory;
	new_name.replace_filename(build_iPod_dir(min, max));

	try
	{
		fs::rename(directory, new_name);
		std::cout << "Renamed directory to " << new_name << '\n';
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
		/// Traverse given directory
		for (auto dir : fs::directory_iterator(p))
		{
			/// Sort ipod photo directories by lowest - highest ipod photo id
			if (fs::is_directory(dir.path()))
			{
				std::cout << "Renaming directory: " << dir << " by ipod photo ids...\n";
				organize_by_ids(dir);
				std::cout << "Done.\n\n";
			}
		}
	}
	else
		std::cerr << "Directory " << p << " does not exist, or it is not a directory!";

    return 0;
}

