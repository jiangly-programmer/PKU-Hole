#ifndef TREEHOLE_VERSION

#define TREEHOLE_VERSION "NO_CACHEFILE_TERMINAL_VERSION_1_0_0"

#include "headers.h"
#include "basic_class.h"
#include "hole.h"
#include "hole_api.h"
#include "filter.h"
#include "holecollection.h"

string trim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
	return s;
}

struct HoleSearcher {
	// 如果时间较短（或者 enforce_all ），那么可以分批次返回所有满足要求的树洞。
	// 否则，只有在正文中至少出现一个关键词的洞才会被找到。
	Filter filter;
	TimeFilter TFilter;
	bool enforce_all;
	int R; // 如果必须抓取所有树洞，那么在一开始就把编号起始位置二分出来
	explicit HoleSearcher(const Filter&, const TimeFilter&, bool);
	const HoleCollection getNext();
};
struct TagManager {
	map<string, HoleCollection> tags;
	const HoleCollection getTag(const string &tag) {return tags[tag];}
	void addHole(const Hole &h, const string &tag) {tags[tag].holes.push_back(h);}
	void updateTag(const string &tag) {tags[tag].updateAll();}
	TagManager() {
		FILE *F = fopen("TagManagerCache", "r");
		if (F != NULL) {
			string tag;
			while (233) {
				tag = "";
				char ch;
				while (isspace(ch = fgetc(F))) ;
				while (!isspace(ch)) tag += ch, ch = fgetc(F);
				// std::cerr << "tag: " << tag << '\n';
				if (tag == "@") break;
				int num; fscanf(F, "%d", &num);
				// std::cerr << "num: " << num << '\n';
				HoleCollection hc;
				for (int i = 0;i < num;i++) {
					string s = "";
					char ch;
					while (isspace(ch = fgetc(F))) ;
					while (!isspace(ch)) s += ch, ch = fgetc(F);
					// std::cerr << "hole: " << s << '\n';
					Hole h(string_to_JSON(s), HoleJSON::HOLETOJSON);
					hc.holes.push_back(h);
				}
				tags[tag] = hc;
			}
		}
		fclose(F);
	}
	~TagManager() {
		FILE *F = fopen("TagManagerCache", "w");
		for (auto [tag,hc] : tags) {
			fprintf(F, "%s\n", tag.c_str());
			fprintf(F, "%d\n", hc.holes.size());
			for (auto h : hc.holes) {
				fprintf(F, "%s\n", JSON_to_string(h.to_JSON()).c_str());
			}
		}
		fprintf(F, "@\n");
		fclose(F);
	}
}tagManager;

struct TerminalInterater {
	void main() {
		std::cout << "TreeHoleHelper\n";
		std::cout << "version : " TREEHOLE_VERSION "\n" << std::flush;
		enum class State {
			Browse, Search, Tag
		}state;
		int page; // Browse
		string tag; // Tag
		HoleSearcher hs(ALLOK, T_ALLOK, 0); // Search
		HoleCollection hc; // current result
		state = State::Browse;
		page = 1;
		hc = HoleCollection::from_getlist_result(API.getlist(page));
		auto output=[](string s) {
			FILE *F = fopen("tmp.out", "w");
			fprintf(F, "%s", s.c_str());
			fclose(F);
		};
		// output(hc.to_str());
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
			} else if (cmd == "next") {
				if (state == State::Browse) {
					page++;
					hc = HoleCollection::from_getlist_result(API.getlist(page));
					output(hc.to_str());
				} else if (state == State::Search) {
					hc = hs.getNext();
					output(hc.to_str());
				} else if (state == State::Tag) {
					printf("current state is Tag\n");
					printf("`next` not allowed\n");
				}
			} else if (cmd == "browse") {
				state = State::Browse;
				page = 1;
				hc = HoleCollection::from_getlist_result(API.getlist(page));
				output(hc.to_str());
			} else if (cmd == "search") {
				state = State::Search;
				printf("input keywords and types, end by @\n");
				string keyword;
				static std::vector<std::pair<string, int>> keywords;
				while (233) {
					int type;
					std::cin >> keyword;
					if (keyword == "@") break;
					std::cerr << "keyword : " << keyword << std::endl;
					assert(!std::cin.bad());
					assert(!std::cerr.bad());
					std::cin >> type;
					std::cerr << "type : " << type << std::endl;
					keywords.emplace_back(keyword, type);
				}
				Filter filter = getFilter(keywords);
				printf("input time range, Year Month Day Hour Minute\n");
				int year, month, day, hour, minute;
				std::cin >> year >> month >> day >> hour >> minute;
				// turn into time_t using std::tm
				tm t;
				t.tm_year = year - 1900;
				t.tm_mon = month - 1;
				t.tm_mday = day;
				t.tm_hour = hour;
				t.tm_min = minute;
				t.tm_sec = 0;
				time_t start = mktime(&t);
				hs = HoleSearcher(filter, TimeFilter(0, start), 0);
				hc = hs.getNext();
				output(hc.to_str());
			} else if (cmd == "tag") {
				state = State::Tag;
				printf("input tag name\n");
				std::cin >> tag;
				hc = tagManager.getTag(tag);
				output(hc.to_str());
			} else if (cmd == "tagadd") {
				int pid;
				printf("input tag name and pid\n");
				std::cin >> tag >> pid;
				tagManager.addHole(Hole(API.getone(pid), HoleJSON::GETONE), tag);
			} else if (cmd == "tagdel") {
				int pid;
				printf("input tag name and pid\n");
				std::cin >> tag >> pid;
				tagManager.tags[tag].holes.erase(std::remove_if(tagManager.tags[tag].holes.begin(), tagManager.tags[tag].holes.end(), [pid](Hole h) {
					return h.pid == pid;
				}), tagManager.tags[tag].holes.end());
			} else {
				printf("unknown command\n");
			}
		}
	}
}terminalInterater;

#endif
