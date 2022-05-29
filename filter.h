#ifndef TREEHOLE_FILTER
#define TREEHOLE_FILTER

#include "hole.h"

struct Filter {
	virtual bool operator () (const Hole &) const;
};

struct KeywordFilter : Filter {
	string word;

	int search_range;
	// 0x01 : main
	// 0x02 : reply

	KeywordFilter(const string &, int);
	virtual bool operator () (const Hole &) const;
};

struct TimeFilter : Filter {
	size_t time_min, time_max;

	TimeFilter(size_t, size_t);
	virtual bool operator () (const Hole &) const;
};

struct Advanced_Filter : Filter {
	enum KeywordFilterType {And, Or, Not} type;
	Filter * left_son, * right_son;

	virtual bool operator () (const Hole &) const;
};

#endif