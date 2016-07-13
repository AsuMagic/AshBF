#include "fileutils.hpp"

#include <fstream>

std::string read_file(std::string& filename)
{
	std::ifstream sourcefile(filename, std::ios::in | std::ios::binary | std::ios::ate);

	auto size = sourcefile.tellg();
	sourcefile.seekg(0, std::ios::beg); // Seek to the beginning of the file

	std::string source(size, ' ');
	sourcefile.read(&source[0], size);

	return source;
}