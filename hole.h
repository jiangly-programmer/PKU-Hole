#ifndef TREEHOLE_hole
#define TREEHOLE_hole

#include "hole_api.h"
#include "basic_class.h"

struct Hole {
	int pid, likenum, replynum;
	time_t update_time, hole_time;
	Text main;
	vector<Text> reply;
	Image img;
	int alertLevel; // what's this?
	inline std::string to_str() const {
		std::string ret;
		auto get = [](time_t x) {
			std::tm* now = std::localtime(&x);
			char date[20];
			sprintf(date, "%04d/%02d/%02d %02d:%02d:%02d", now -> tm_year + 1900, now -> tm_mon + 1, now -> tm_mday, now -> tm_hour, now -> tm_min, now -> tm_sec);
			return std::string(date);
		};
		ret += "#" + std::to_string(pid) + " | " + get(hole_time) + "\n";
		ret += "last update time : " + get(update_time) + "\n";
		ret += "likes : " + std::to_string(likenum) + " | replys : " + std::to_string(replynum) + "\n";
		ret += main.s;
		ret += "\n";
		if(img.url != "") {
			ret += "image : " + config["url"].asString() + "images/" + img.url;
		}
		ret += "\n";
		return ret;
	}

	explicit Hole(int _pid, int _likenum, int _replynum, time_t _hole_time, time_t _update_time, const Text &_main, const vector<Text>&_reply, const Image &_img) :
		pid(_pid), likenum(_likenum), replynum(_replynum), hole_time(_hole_time), update_time(_update_time), main(_main), reply(_reply), img(_img), alertLevel(-1)
	{
	}

	explicit Hole(const JSON & x) :
	pid(atoi(x["data"]["pid"].asString())),
	likenum(atoi(x["data"]["likenum"].asString())),
	replynum(atoi(x["data"]["reply"].asString())),
	hole_time(atoi(x["data"]["timestamp"].asString())),
	update_time(atoi(x["timestamp"].asString())),
	main(x["data"]["text"].asString()),
	reply(),
	img(x["data"]["url"].asString()),
	alertLevel(-1)
	{
	}
	bool operator < (Hole &a) const {
		return pid < a.pid;
	}
	// virtual void refresh();
	virtual JSON to_Json() const {
		// 他妈的老子先不测了
		JSON ret;
		ret["pid"] = std::to_string(pid);
		ret["data"]["likenum"] = std::to_string(likenum);
		ret["data"]["reply"] = std::to_string(replynum);
		ret["data"]["timestamp"] = std::to_string(hole_time);
		ret["data"]["text"] = main.s;
		ret["data"]["url"] = img.url;
		ret["timestamp"] = std::to_string(update_time);
		return ret;
	}
};

/*
struct Hole_level_1 : Hole {
};

struct Hole_level_2 : Hole {
};

struct Hole_level_3 : Hole {
};
*/

#endif
