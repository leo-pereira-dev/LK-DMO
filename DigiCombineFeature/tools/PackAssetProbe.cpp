#include "stdafx.h"
#include "CsFilePackSystem.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace
{
	void PrintUsage()
	{
		std::cout << "Usage: PackAssetProbe <pack-base-path> <asset-path> [output-file]\n"
				  << "Example: PackAssetProbe C:\\DMO\\Data\\Pack01 Random_box\\bg.png out.png\n";
	}
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		PrintUsage();
		return 2;
	}

	const std::string packBasePath = argv[1];
	const std::string assetPath = argv[2];

	std::list<std::string> packs;
	packs.push_back(packBasePath);

	if (!CsFPS::CsFPSystem::Initialize(true, packs, false))
	{
		std::cerr << "init-failed," << packBasePath << "," << assetPath << "\n";
		return 1;
	}

	std::vector<unsigned char> data;
	const size_t size = CsFPS::CsFPSystem::GetFileData(0, assetPath.c_str(), data);
	if (size == 0 || data.empty())
	{
		std::cout << "missing," << packBasePath << "," << assetPath << "\n";
		CsFPS::CsFPSystem::Destroy();
		return 3;
	}

	std::cout << "found," << packBasePath << "," << assetPath << "," << size << "\n";

	if (argc >= 4)
	{
		const std::filesystem::path outputPath = argv[3];
		std::filesystem::create_directories(outputPath.parent_path());

		std::ofstream out(outputPath, std::ios::binary);
		out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
		if (!out)
		{
			std::cerr << "write-failed," << outputPath.string() << "\n";
			CsFPS::CsFPSystem::Destroy();
			return 4;
		}
	}

	CsFPS::CsFPSystem::Destroy();
	return 0;
}
