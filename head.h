#ifndef TREEHOLE_VERSION

#define TREEHOLE_VERSION "NO_CACHEFILE_TERMINAL_VERSION_1_0_0"

#include "headers.h"
#include "basic_class.h"
#include "hole.h"
#include "hole_api.h"
#include "filter.h"
#include "holecollection.h"

struct HoleSearcher {
	Filter filter;
	int pid;
	explicit HoleSearcher(const Filter&, int pid);
	const HoleCollection getNext();
};
struct TagManager {
	map<string, HoleCollection> tags;
	const HoleCollection getTag(const string &tag);
	void addHole(const Hole &, const string &tag);
};

struct TerminalInterater {
	void main() {
		std::cout << "TreeHoleHelper\n";
		std::cout << "version : " TREEHOLE_VERSION "\n" << std::flush;
		while (true) {
			std::cout << ">>>";
			std::string cmd;
			std::cin >> cmd;
			if (cmd == "help") {
				std::cout <<
				          "谁来写一下啊\n"
				          "谁来写一下啊\n";

			} else if (cmd == "quit") {
				break;
			}
		}
	}
};

#endif
