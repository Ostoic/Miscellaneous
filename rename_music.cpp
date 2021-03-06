// RenameMusic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <regex>
#include <filesystem>
#include <sstream>
#include <iostream>

namespace fs = std::experimental::filesystem;

struct song_t
{
	int volume_number;
	int song_number;
	fs::path path;
};

int main()
{
	const fs::path music_folder = R"(C:\Users\dinne\source\repos\RenameMusic\RenameMusic\Analord)";

	std::vector<song_t> songs;

	for (auto& file : fs::directory_iterator(music_folder))
	{
		if (!fs::is_regular_file(file))
			continue;

		std::smatch matches;
		const std::regex pattern{"[a-zA-Z -]*([0-9][0-9]?) - ([0-9][0-9]?)[a-zA-Z -]*.*\.mp3"};
		const auto filename = file.path().filename().string();

		if (std::regex_search(filename, matches, pattern))
		{
			int volume_number, song_number;
			std::stringstream{ matches[1] } >> volume_number;
			std::stringstream{ matches[2] } >> song_number;

			songs.push_back({
				volume_number, song_number, 
				std::move(file) 
			});
		}
	}

	std::sort(songs.begin(), songs.end(), [](const song_t& lhs, const song_t& rhs)
	{
		return lhs.volume_number < rhs.volume_number;
	});

	std::uint32_t i = 1;
	for (auto& song : songs)
	{
		std::stringstream stream; 
		stream << i++ << ".mp3";

		fs::path new_filename = song.path;
		new_filename.replace_filename(stream.str());
		fs::rename(song.path, new_filename);
	}
	std::cout << "Songs count = " << songs.size() << '\n';
    return 0;
}

