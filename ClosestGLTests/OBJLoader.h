#pragma once
#include <tuple>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

namespace ClosestGLTests::Tools
{
	template<typename TVertex>
	std::tuple<std::vector<TVertex>, std::vector<size_t>> LoadModelOBJ(const char* path)
	{
		std::ifstream in(path);
		std::tuple<std::vector<TVertex>, std::vector<size_t>> ret;

		if (in.fail()) std::terminate();

		while (!in.eof())
		{
			std::string line;
			std::getline(in, line);

			std::stringstream sr(line);

			std::string mark;
			sr >> mark;

			if (mark == "v")
			{
				TVertex vert;
				sr >> vert.SVPosition.x;
				sr >> vert.SVPosition.y;
				sr >> vert.SVPosition.z;
				vert.SVPosition.w = 1;

				std::get<0>(ret).push_back(vert);
			}

			if (mark == "f")
			{
				std::string f;
				for (int i = 0; i < 3; ++i)
				{
					sr >> f;
					auto pos = f.find('/');
					if (pos != std::string::npos)
						f = f.substr(0, pos);
					auto index = std::atoi(f.c_str());
					std::get<1>(ret).push_back(index - 1);
				}
			}
		}

		return ret;
	}
}
