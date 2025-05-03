#pragma once
#include <random>

namespace Engine::Random
{
	inline std::random_device SEEDER;
	inline std::mt19937 ENGINE{ SEEDER() };

	inline size_t Int(size_t min, size_t max)
	{
		std::uniform_int_distribution Get{ min, max };
		return Get(ENGINE);
	}

	inline bool Chance(int chance)
	{
		if (chance > 100 || chance < 0)
		{
			std::cout << "Chance percentage is incorrect" << std::endl;
			return false;
		}

		return chance > Int(0, 100);
	}
}