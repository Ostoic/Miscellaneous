// LoC.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <string>

namespace fs = std::experimental::filesystem;

std::size_t count_lines(const fs::path& filePath)
{
	std::wifstream file(filePath);
	std::size_t numberOfLines = 0;

	while (file)	
	{
		++numberOfLines;

		// Skip to the next line
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return numberOfLines;
}

bool is_cpp_extension(const std::wstring& extension) noexcept
{
	constexpr std::array<const wchar_t*, 6> extensions = { L".cpp", L".cc", L".c", L".hpp", L".hxx", L".h" };
	return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
}

bool contained_in_cwd(const fs::path& path)
{
	for (auto entry : fs::recursive_directory_iterator(fs::current_path()))
		if (fs::equivalent(entry.path(), path))
			return true;
	
	return false;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Invalid parameters.\n";
		std::cerr << "Usage: loc <folder path>\n";
		return 0;
	}

	fs::path folderPath = argv[1];

	if (folderPath.is_relative())
		folderPath = fs::absolute(folderPath);

	if (!fs::is_directory(folderPath) || !fs::exists(folderPath))
	{
		std::cerr << "Given path is not a folder\n";
		return 0;
	}

	std::size_t totalLines = 0;

	for (auto entry : fs::recursive_directory_iterator(folderPath))
		if (!fs::is_directory(entry) && is_cpp_extension(entry.path().extension().wstring()))
			totalLines += count_lines(entry) - 1;

	std::wcout << "There are " << totalLines << " total lines of code in " << folderPath.wstring() << ".\n";
    return 0;
}

