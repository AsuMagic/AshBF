#include "bf.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

namespace bf
{
	void Brainfuck::compile(const std::string &fname)
	{
		std::ifstream file{fname, std::ios::binary};

		if (!file)
			throw std::runtime_error(std::string{"Could not load file '"} + fname + '\'');

		static const std::array<BFOp, 8> instruction_list =
		{{
			{'+', bfAdd, 1},
			{'-', bfAdd, -1},
			{'>', bfShift, 1},
			{'<', bfShift, -1},
			{'.', bfCharOut},
			{',', bfCharIn},
			{'[', bfLoopBegin},
			{']', bfLoopEnd}
		}};

		char current;
		file.get(current);

		while (file.good())
		{
			auto it = std::find_if(begin(instruction_list), end(instruction_list), [current](const BFOp& other) { return current == other.match; });

			if (it != end(instruction_list))
				program.emplace_back(static_cast<uint8_t>(it->base_opcode), it->default_arg);

			file.get(current);
		}

		program.emplace_back(bfEnd, 0);
	}
}
