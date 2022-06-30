#pragma once

#include "HoleAPI.h"
#include "utility.h"

namespace HoleJSON {
struct JSON_format {
  virtual int get_pid(const JSON& x) const = 0;
  virtual int get_likenum(const JSON& x) const = 0;
  virtual int get_replynum(const JSON& x) const = 0;
  virtual time_t get_update_time(const JSON& x) const = 0;
  virtual time_t get_hole_time(const JSON& x) const = 0;
  virtual Text get_main(const JSON& x) const = 0;
  virtual const std::vector<Text> get_reply(const JSON& x) const = 0;
  virtual const Image get_img(const JSON& x) const = 0;
};
struct type1 : JSON_format {
  virtual int get_pid(const JSON& x) const {
    return std::stoi(x["data"]["pid"].asString());
  }
  virtual int get_likenum(const JSON& x) const {
    return std::stoi(x["data"]["likenum"].asString());
  }
  virtual int get_replynum(const JSON& x) const {
    return std::stoi(x["data"]["reply"].asString());
  }
  virtual time_t get_update_time(const JSON& x) const {
    return std::stoi(x["timestamp"].asString());
  }
  virtual time_t get_hole_time(const JSON& x) const {
    return std::stoi(x["data"]["timestamp"].asString());
  }
  virtual Text get_main(const JSON& x) const {
    return Text(x["data"]["text"].asString());
  }
  virtual const std::vector<Text> get_reply(const JSON& x) const {
    return std::vector<Text>();
  }
  virtual const Image get_img(const JSON& x) const {
    return Image(x["data"]["url"].asString());
  }
};
struct type2 : JSON_format {
  virtual int get_pid(const JSON& x) const {
    return std::stoi(x["pid"].asString());
  }
  virtual int get_likenum(const JSON& x) const {
    return std::stoi(x["likenum"].asString());
  }
  virtual int get_replynum(const JSON& x) const {
    return std::stoi(x["reply"].asString());
  }
  virtual time_t get_update_time(const JSON& x) const { return time(0); }
  virtual time_t get_hole_time(const JSON& x) const {
    return std::stoi(x["timestamp"].asString());
  }
  virtual Text get_main(const JSON& x) const {
    return Text(x["text"].asString());
  }
  virtual const std::vector<Text> get_reply(const JSON& x) const {
    return std::vector<Text>();
  }
  virtual const Image get_img(const JSON& x) const {
    return Image(x["url"].asString());
  }
};
struct type3 : JSON_format {
  virtual int get_pid(const JSON& x) const {
    return std::stoi(x["pid"].asString());
  }
  virtual int get_likenum(const JSON& x) const {
    return std::stoi(x["data"]["likenum"].asString());
  }
  virtual int get_replynum(const JSON& x) const {
    return std::stoi(x["data"]["reply"].asString());
  }
  virtual time_t get_update_time(const JSON& x) const {
    return std::stoi(x["timestamp"].asString());
  }
  virtual time_t get_hole_time(const JSON& x) const {
    return std::stoi(x["data"]["timestamp"].asString());
  }
  virtual Text get_main(const JSON& x) const {
    return Text(x["data"]["text"].asString());
  }
  virtual const std::vector<Text> get_reply(const JSON& x) const {
    return std::vector<Text>();
  }
  virtual const Image get_img(const JSON& x) const {
    return Image(x["data"]["url"].asString());
  }
};

const type1 GETONE;
const type2 GETLIST;
const type3 HOLETOJSON;
}  // namespace HoleJSON
struct Hole {
  int pid, likenum, replynum;
  time_t update_time, hole_time;
  Text main;
  vector<Text> reply;
  Image img;
  std::string to_str() const {
    std::string ret;
    auto get = [](time_t x) {
      std::tm* now = std::localtime(&x);
      char date[20];
      sprintf(date, "%04d/%02d/%02d %02d:%02d:%02d", now->tm_year + 1900,
              now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min,
              now->tm_sec);
      return std::string(date);
    };
    ret += "#" + std::to_string(pid) + " | " + get(hole_time) + "\n";
    ret += "last update time : " + get(update_time) + "\n";
    ret += "likes : " + std::to_string(likenum) +
           " | replys : " + std::to_string(replynum) + "\n";
    ret += main.s;
    ret += "\n";
    if (img.url != "") {
      ret += "image : " + config["url"].asString() + "images/" + img.url;
    }
    ret += "\n";
    return ret;
  }

  explicit Hole(int _pid,
                int _likenum,
                int _replynum,
                time_t _hole_time,
                time_t _update_time,
                const Text& _main,
                const vector<Text>& _reply,
                const Image& _img)
      : pid(_pid),
        likenum(_likenum),
        replynum(_replynum),
        hole_time(_hole_time),
        update_time(_update_time),
        main(_main),
        reply(_reply),
        img(_img) {}

  Hole(const Hole& x)
      : pid(x.pid),
        likenum(x.likenum),
        replynum(x.replynum),
        hole_time(x.hole_time),
        update_time(x.update_time),
        main(x.main),
        reply(x.reply),
        img(x.img) {}

  explicit Hole(const JSON& x, const HoleJSON::JSON_format& r)
      : pid(r.get_pid(x)),
        likenum(r.get_likenum(x)),
        replynum(r.get_replynum(x)),
        hole_time(r.get_hole_time(x)),
        update_time(r.get_update_time(x)),
        main(r.get_main(x)),
        reply(r.get_reply(x)),
        img(r.get_img(x)) {}
  bool operator<(const Hole& a) const { return pid < a.pid; }
  virtual JSON to_JSON() const {
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
