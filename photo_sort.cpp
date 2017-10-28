#include "stdafx.h"
#include "photo_sort\pid_sort.hpp"
#include "photo_sort\date_sort.hpp"

#include <boost\program_options.hpp>

namespace fs = std::experimental::filesystem;
namespace po = boost::program_options;

template <typename Comparison, typename Builder>
void rename_dir(const fs::path& directory, Comparison compare, Builder build)
{
	auto end = fs::directory_iterator();

 	const auto min = *std::min_element(fs::directory_iterator(directory), end, compare);
	const auto max = *std::max_element(fs::directory_iterator(directory), end, compare);

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

template <typename Comparator, typename Builder>
void run(std::string&& input_folder, Comparator compare, Builder builder)
{
	fs::path p(std::move(input_folder));

	std::cout << "Checking directory " << p << '\n';
	if (fs::exists(p) && fs::is_directory(p))
	{
		for (auto dir : fs::directory_iterator(p))
		{
			if (fs::is_directory(dir.path()))
			{
				rename_dir(dir, compare, builder);
				std::cout << "Done.\n\n";
			}
		}
	}
	else
		std::cerr << "Directory " << p << " does not exist, or it is not a directory!";
}

int main(int argc, char* argv[])
{
	try
	{
		po::options_description description("Allowed options");
		description.add_options()
			("help", "produce help message")
			("date", "sort photos by date")
			("pid", "sort photos by their photo ids")
			("input-folder", "the folder to rename");

		po::positional_options_description pod;
		pod.add("input-folder", -1);

		po::variables_map vm;
		po::store(
			po::command_line_parser(argc, argv).
			options(description).positional(pod).run(), vm);

		po::notify(vm);

		std::string input_folder;

		// No options specified
		if (vm.size() == 0 || vm.count("help"))
		{
			std::cout << description << '\n';
			return 0;
		}
		
		// Specify input folder
		if (vm.count("input-folder"))
			input_folder = vm["input-folder"].as<std::string>();
		else
		{
			std::cerr << "Error: Input folder required\n";
			return 0;
		}

		// Sort by photo date
		if (vm.count("date"))
		{
			if (vm.count("pid"))
			{
				std::cerr << "Error: two different sort methods specified\n";
				return 0;
			}

			run(std::move(input_folder), photo_sort::date_compare(), photo_sort::date_builder());
		}

		// Sort by photo id
		else
		{
			if (vm.count("date"))
			{
				std::cerr << "Error: two different sort methods specified\n";
				return 0;
			}

			run(std::move(input_folder), photo_sort::pid_compare(), photo_sort::pid_builder());
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

    return 0;
}

