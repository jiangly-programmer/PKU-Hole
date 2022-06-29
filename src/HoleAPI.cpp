#include "HoleAPI.h"
#include <codecvt>
#include <locale>
#include "curl/curl.h"

HoleAPI API;

string geturl(string website,
              string action,
              vector<std::pair<string, int>> param,
              string param2 = "") {
  string res = website + "?";
  res += "action=" + action;
  for (auto p : param) {
    res += "&" + p.first + "=" + std::to_string(p.second);
  }
  res += param2;
  res += "&PKUHelperAPI=3.0&jsapiver=201027113050-459248";
  res += "&user_token=" + CONFIG::token;
  return res;
}

size_t easy_callback(void* data, size_t size, size_t nmemb, void* userp) {
  for (int i = 0; i < (int)nmemb; i++) {
    ((std::string*)userp)->push_back(((char*)data)[i]);
  }
  return nmemb;
}

CURL* my_easy_init(string url, string& dst) {
  dst.clear();
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, easy_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&dst);
  return curl;
}

const string error = "{\"code\":-1}";

int hex_to_int(int c) {
  return (c >= 'a' ? c - 'a' + 10 : c - '0');
}

char int_to_hex(int w) {
  return w >= 10 ? 'a' + w - 10 : '0' + w;
}

string utf8_to_url(const string& s) {
  string res;
  for (int i = 0; i < (int)s.size(); i++) {
    unsigned w = s[i];
    res += '%', res += int_to_hex((w & 0xf0) >> 4), res += int_to_hex(w & 0x0f);
  }
  return res;
}

string unicode_to_utf8(const string& s) {
#define hex4(i)                                                 \
  ((hex_to_int(s[i + 2]) << 12) | (hex_to_int(s[i + 3]) << 8) | \
   (hex_to_int(s[i + 4]) << 4) | (hex_to_int(s[i + 5])))
  string res;
  for (int i = 0; i < (int)s.size(); i++) {
    if (s[i] != '\\') {
      res.push_back(s[i]);
      continue;
    }
    if (s[i + 1] != 'u') {
      res.push_back(s[i]);
      res.push_back(s[i + 1]);
      // static map<char, char> pb = {
      // 	{'n', '\n'},
      // 	{'/', '/'},
      // 	{'\\', '\\'},
      // 	{'\'', '\''},
      // 	{'\"', '\"'},
      // 	{'t', '\t'},
      // 	{'r', '\r'},
      // };
      // if (pb.count(s[i + 1])) {
      // 	res.push_back(pb[s[i + 1]]);
      // } else {
      // 	std::cerr << "unknown escape sequence \\" << s[i + 1] << '\n',
      // assert(0);
      // }
      i++;
      continue;
    }
    unsigned u16char = hex4(i);
    i += 5;
    if (u16char <= 0x007f) {
      res.push_back(u16char);
    } else if (u16char <= 0x07ff) {
      res.push_back((char)(((u16char >> 6) & 0x1F) | 0xC0));
      res.push_back((char)((u16char & 0x3F) | 0x80));
    } else if (u16char >= 0xd800 && u16char <= 0xdbff) {
      uint32_t highSur = u16char;
      uint32_t lowSur = hex4(i + 1);
      i += 6;
      uint32_t codePoint = highSur - 0xD800;
      codePoint <<= 10;
      codePoint |= lowSur - 0xDC00;
      codePoint += 0x10000;
      res.push_back((char)((codePoint >> 18) | 0xF0));
      res.push_back((char)(((codePoint >> 12) & 0x3F) | 0x80));
      res.push_back((char)(((codePoint >> 06) & 0x3F) | 0x80));
      res.push_back((char)((codePoint & 0x3F) | 0x80));
    } else {
      res.push_back((char)(((u16char >> 12) & 0x0F) | 0xE0));
      res.push_back((char)(((u16char >> 6) & 0x3F) | 0x80));
      res.push_back((char)((u16char & 0x3F) | 0x80));
    }
  }
  return res;
#undef hex4
}

JSON HoleAPI::getjson(const std::string& url) const {
  std::cerr << "enter getjson\n";
  string tmp;
  auto curl = my_easy_init(url, tmp);
  CURLcode res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "%s failed: %s\n", url.c_str(), curl_easy_strerror(res));
    return string_to_JSON(error);
  }
  std::cerr << "unicode_to_utf8\n";
  auto res1 = unicode_to_utf8(tmp);
  std::cerr << "string_to_JSON\n";
  auto res2 = string_to_JSON(res1);
  std::cerr << "exit getjson\n";
  return res2;
  // return string_to_JSON(unicode_to_utf8(tmp));
}

JSON HoleAPI::getone(int pid) const {
  return getjson(geturl(CONFIG::url, "getone", {{"pid", pid}}));
}

JSON HoleAPI::getcomment(int pid) const {
  return getjson(geturl(CONFIG::url, "getcomment", {{"pid", pid}}));
}

JSON HoleAPI::getlist(int p) const {
  return getjson(geturl(CONFIG::url, "getlist", {{"p", p}}));
}

JSON HoleAPI::getattention() const {
  return getjson(geturl(CONFIG::url, "getattention", {}));
}

JSON HoleAPI::search(string word, int page) const {
  return getjson(geturl(CONFIG::url, "search", {{"page", page}},
                        "&keywords=" + utf8_to_url(word) + "&pagesize=50"));
}

vector<JSON> HoleAPI::multi_getone(const vector<int>& pid) const {
  if (!pid.size())
    return vector<JSON>();
  CURLM* multi = curl_multi_init();
  vector<CURL*> curl;
  curl.resize(pid.size());
  vector<string> res;
  res.resize(pid.size());
  for (int i = 0; i < (int)pid.size(); i++) {
    curl[i] =
        my_easy_init(geturl(CONFIG::url, "getone", {{"pid", pid[i]}}), res[i]);
    curl_multi_add_handle(multi, curl[i]);
  }
  for (int _ = 1; _ <= 3; _++) {
    clock_t _start = clock();
    int running;
    do {
      curl_multi_perform(multi, &running);
    } while (running && clock() - _start < 5 * CLOCKS_PER_SEC);
    for (auto c : curl) {
      curl_multi_remove_handle(multi, c);
    }
    for (int i = 0; i < (int)pid.size(); i++)
      if (res[i].find("503 Service Temporarily Unavailable") != string::npos) {
        res[i].clear();
        std::cerr << "503 Service Temporarily Unavailable\n";
        curl_multi_add_handle(multi, curl[i]);
      }
    Sleep(1000);
  }
  curl_multi_cleanup(multi);
  for (auto c : curl) {
    curl_easy_cleanup(c);
  }
  vector<JSON> RES;
  for (auto s : res)
    if (s.size() &&
        s.find("503 Service Temporarily Unavailable") == string::npos) {
      RES.push_back(string_to_JSON(unicode_to_utf8(s)));
    } else {
      RES.push_back(string_to_JSON(error));
      std::cerr << s << '\n';
    }
  return RES;
}

vector<JSON> HoleAPI::multi_getcomment(const vector<int>& pid) const {
  if (!pid.size())
    return vector<JSON>();
  CURLM* multi = curl_multi_init();
  vector<CURL*> curl;
  curl.resize(pid.size());
  vector<string> res;
  res.resize(pid.size());
  clock_t _start = clock();
  int running;
  for (int i = 0; i < (int)pid.size(); i++) {
    curl[i] = my_easy_init(geturl(CONFIG::url, "getcomment", {{"pid", pid[i]}}),
                           res[i]);
    curl_multi_add_handle(multi, curl[i]);
  }
  for (int _ = 1; _ <= 3; _++) {
    clock_t _start = clock();
    int running;
    do {
      curl_multi_perform(multi, &running);
      Sleep(100);
    } while (running && clock() - _start < 5 * CLOCKS_PER_SEC);
    for (auto c : curl) {
      curl_multi_remove_handle(multi, c);
    }
    for (int i = 0; i < (int)pid.size(); i++)
      if (res[i].find("503 Service Temporarily Unavailable") != string::npos) {
        res[i].clear();
        curl_multi_add_handle(multi, curl[i]);
      }
  }
  for (auto c : curl) {
    curl_multi_remove_handle(multi, c);
  }
  curl_multi_cleanup(multi);
  for (auto c : curl) {
    curl_easy_cleanup(c);
  }
  vector<JSON> RES;
  for (auto s : res)
    if (s.size() &&
        s.find("503 Service Temporarily Unavailable") != string::npos) {
      RES.push_back(string_to_JSON(unicode_to_utf8(s)));
    } else {
      RES.push_back(string_to_JSON(error));
    }
  return RES;
}

void test_hole_api() {
  std::fstream fout("tmp.out");
  vector<int> V = {};
  for (int i = 2856415; i >= 2856385; i--) {
    V.push_back(i);
  }

  auto res = API.multi_getone(V);

  for (auto js : res) {
    fout << JSON_to_string(js) << '\n';
  }
}
