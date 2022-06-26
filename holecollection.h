#ifndef TREEHOLE_HOLECOLLECTION
#define TREEHOLE_HOLECOLLECTION

#include "hole_api.h"
#include "hole.h"
#include "filter.h"

struct HoleCollection {
	vector<Hole> holes;

	explicit HoleCollection()
	{
	}
	 explicit HoleCollection(const std::vector<int>& hid) {
		auto res = API.multi_getone(hid);
		// for (auto h:hid) std::cerr<<h<<' '; std::cerr<<'\n';
		for(const auto & x : res) {
			// std::cerr << JSON_to_string(x) << '\n';
			if(x["code"].asInt() == 0) {
				holes.emplace_back(x, HoleJSON::GETONE);
				if (holes.back().pid == 0) {
					std::cerr << JSON_to_string(x) << std::endl;
					assert(0);
				}
			}
		}
	}
	HoleCollection(const HoleCollection& hid) : holes(hid.holes) {
	}

	static const HoleCollection from_search_result(const JSON & x) {
		std::cerr << "enter HoleCollection::from_search_result()" << std::endl;
		// std::cerr << JSON_to_string(x) << std::endl;
		HoleCollection ret;
		for(int i = 0;i < (int)x["data"].size();i++) {
			ret.holes.emplace_back(x["data"][i], HoleJSON::GETLIST);
		}
		std::cerr << "exit HoleCollection::from_search_result()" << std::endl;
		return ret;
	}
	static const HoleCollection from_getlist_result(const JSON & x) {
		HoleCollection ret;
		for(int i = 0;i < (int)x["data"].size();i++) {
			ret.holes.emplace_back(x["data"][i], HoleJSON::GETLIST);
		}
		return ret;
	}
	void sort_by_pid() {
		std::sort(holes.begin(), holes.end(), [](const Hole& a, const Hole& b) {
			return a.pid < b.pid;
		});
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

	void updateAll() {
		vector<int>pid;
		for(const auto & w : holes) {
			pid.push_back(w.pid);
		}
		auto res = API.multi_getcomment(pid);
		for (int i = 0; i < (int)res.size(); i++) {
			if (res[i]["code"].asInt() == 0) {
				for (int j = 0; j < (int)res[i]["data"].size(); j++) {
					holes[i].reply[j] = Text(res[i]["data"][j]["text"].asString());
				}
			}
		}
	}
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