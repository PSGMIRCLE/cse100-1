#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include "HCTree.hpp"

// creates the header used for the uncompress program with the following format:
/*
	# HCT
	char freq char freq ... char freq
	# END
*/
void makeHeader(std::ostream &out, HCTree hct, std::vector<int> freqs)
{
	out << "# HCT" << std::endl;
	for (size_t i = 0; i < freqs.size(); ++i) {
		if (freqs[i]) {
			out << i << " " << freqs[i] << " ";
		}
	}
	out << std::endl << "# END" << std::endl;
}

int main(int argc, char* argv[])
{
	// read the file into a buffer
	std::ifstream ifs(argv[1], std::ios::binary);
	std::string buffer((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));

	// count the occurences of each char
	std::vector<int> freqs(256, 0);
	for (size_t i = 0; i < buffer.size(); ++i) {
		byte b = buffer[i];
		++freqs[b];
	}

	// build the HCTree
	HCTree hct;
	hct.build(freqs);

	// output the compressed file
	std::ofstream ofs(argv[2], std::ios::binary);
	BitOutputStream bos(ofs);
	makeHeader(ofs, hct, freqs);
	for (size_t i = 0; i < buffer.size(); ++i) {
		hct.encode(buffer[i], bos);
	}

	return 0;
}
