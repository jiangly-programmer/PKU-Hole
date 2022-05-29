#ifndef TREEHOLE_HOLECOLLECTION
#define TREEHOLE_HOLECOLLECTION

#include "hole_api.h"
#include "hole.h"
#include "filter.h"

struct HoleCollection {
	vector<Hole> holes;


	explicit HoleCollection(const std::vector<int>& hid) {
		auto res = API.multi_getone(hid);
		for(const auto & x : res) {
			if(x["code"].asInt() == 0) {
				holes.emplace_back(x);
			}
		}
	}
	inline string to_str() const {
		std::string x;
		x += "-------------------------------------\n";
		for(const auto & w : holes) {
			x += w.to_str();
			x += "-------------------------------------\n";
		}
		return x;
	}

	void refreshAll();
	const HoleCollection filter(const Filter &) const;
	const HoleCollection merge(const HoleCollection & rhs) const {
		HoleCollection ret = *this;
		ret.holes.insert(ret.end(), rhs.begin(), rhs.end());
		return ret;
	}
	JSON to_Json() const;

	typedef vector<Hole>::iterator iterator;
	typedef vector<Hole>::const_iterator const_iterator;
	inline iterator begin() {
		return holes.begin();
	}
	inline iterator end() {
		return holes.end();
	}
	inline const_iterator begin() const {
		return holes.cbegin();
	}
	inline const_iterator end() const {
		return holes.cend();
	}
};

#endif