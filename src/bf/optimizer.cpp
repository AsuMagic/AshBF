#include "bf.hpp"

#include <vector>
#include <array>
#include <functional>
#include <algorithm>
#include "../vecutils.hpp"

// @TODO add code analysis for optimizations, improve the sequence-based optimizer or remove it
// @TODO have a way to profile the execution of the brainfuck program and make it possible to analyze it (with an external tool eventually)
namespace bf
{
	void Brainfuck::optimize(const size_t passes)
	{		
		typedef std::vector<Instruction> ivec;
		static std::array<OptimizationSequence, 3> optimizers
		{{
			// [-] or [+] then set 0
			OptimizationSequence{{bfLoopBegin, bfAdd, bfLoopEnd}, [](const ivec&) -> ivec {
				return {{bfSet, 0}};
			}},

			// + or - and then set -> set
			OptimizationSequence{{bfAdd, bfSet}, [](const ivec& v) -> ivec { return {{bfSet, v[1].argument}}; }},

			// [>] and [<]
			OptimizationSequence{{bfLoopBegin, bfShift, bfLoopEnd}, [](const ivec& v) -> ivec {
				if (v[1].argument == -1 || v[1].argument == 1)
					return {{bfLoopUntilZero, v[1].argument}};
				return v;
			}},
		}};

		for (size_t p = 0;; ++p)
		{
			if (p >= passes)
			{
				warnout(optimizeinfo) << "Maximal optimization pass reached. Consider increasing -optimizepasses.\n";
				return;
			}
			
			bool useful_pass = false;
			
			// Merge stackable ops
			for (size_t i = 0; i < program.size(); ++i)
			{
				if (!instructions[program[i].opcode].stackable)
					continue;
				
				size_t j = i;
				while (++j < program.size() && program[i].opcode == program[j].opcode)
					program[i].argument += program[j].argument;
				
				program.erase(begin(program) + i + 1, begin(program) + j);
			}
			
			// Sequence based optimizer
			for (auto& optimizer : optimizers)
			{
				for (size_t i = 0; i < program.size(); ++i)
				{
					if (std::equal(begin(program) + i, begin(program) + i + optimizer.seq.size(), begin(optimizer.seq), end(optimizer.seq)))
					{
						ivec extract(program.begin() + i, program.begin() + i + optimizer.seq.size());
						ivec optimized = optimizer.callback(extract);
						
						if (!std::equal(begin(extract), end(extract), begin(optimized), end(optimized)))
						{
							useful_pass = true;
							replace_subvector_smaller(program, begin(program) + i, begin(program) + i + extract.size(), optimized);
						}	
					}
				}
			}

			if (!useful_pass)
				break;
		}

		program.shrink_to_fit();
	}
}
