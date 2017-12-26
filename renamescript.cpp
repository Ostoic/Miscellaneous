// RenameScript.cpp : A short C++ "script" to change file extensions from .inl to .hxx.
// This is mostly just to get some practice with std::filesystem.

#include <experimental\filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		int number = 0;
		std::cout << argv[1] << '\n';
		fs::path p(argv[1]);

		for (auto entry : fs::recursive_directory_iterator(p))
		{
			if (!fs::is_directory(entry))
			{
				const auto& oldPath = entry.path();
				if (oldPath.extension().string() == ".inl")
				{
					fs::path newPath = oldPath;
					newPath.replace_extension(".hxx");

					std::cout << "Found inline file.\n";
					std::cout << "Renaming " << oldPath << " to " << newPath << "...\n";
					fs::rename(oldPath, newPath);
					number++;
				}
			}
		}

		std::cout << "Number of files renamed: " << number << '\n';
	}
    return 0;
}

