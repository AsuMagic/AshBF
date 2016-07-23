#include "bf.hpp"

#include <cassert>

namespace bf
{
	void Brainfuck::link()
	{
		std::vector<uint16_t> jumps;
		for (size_t i = 0; i < program.size(); ++i)
		{
			switch (program[i].opcode)
			{
				case bfLoopBegin:
					program[i].opcode = static_cast<uint8_t>(bfJmpZero);
					jumps.push_back(i);
				break;

				case bfLoopEnd:
					assert(!jumps.empty());
					program[i].opcode = static_cast<uint8_t>(bfJmpNotZero);
					program[jumps.back()].argument = i + 1;
					program[i].argument = jumps.back() + 1;
					jumps.pop_back();
				break;
			}
		}

		if (extended_level >= 2)
		{
			while(!jumps.empty())
			{
				program[jumps.back()].opcode = static_cast<uint8_t>(bfLoopBegin);

				assert(initializer_loopends--);

				if (warnings)
					puts("Warning : Found an orphan loop begin ('[') that will be ignored at runtime, because a matching loop end (']') was found in the memory initializer area, but not in the code area. (Ex. Type II)");

				jumps.pop_back();
			}
		}

		assert(jumps.empty());
	}
}
