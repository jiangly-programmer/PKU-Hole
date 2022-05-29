#ifndef TREEHOLE_BASIC_CLASS
#define TREEHOLE_BASIC_CLASS

#include "headers.h"
#include "jsoncpp/json.h"

typedef Json::Value JSON;
inline int atoi(const std::string & s) {
	return atoi(s.c_str());
}

struct Image {
	string url;
	inline explicit Image(const string & x) : url(x) {
	}
	enum ImageType {Image_Cat, Image_Dog, Image_Person, Image_Scenery, Image_Text, Image_Else} type;
};

struct Text {
	string s;
	set<string> words;
	explicit Text(const string&_s) : s(_s) {
	}
	bool haveWord(const string&_s) const;
};

Json::Reader reader;
Json::FastWriter writer;

inline JSON string_to_JSON(const string & s) {
	JSON ret;
	reader.parse(s, ret);
	return ret;
}
inline string JSON_to_string(const JSON & s) {
	return writer.write(s);
}
inline JSON readJSON(const string & s) {
	FILE * file = fopen(s.c_str(), "r");
	assert(file);
	static const int size = 1 << 20;
	static char buffer[size];
	int L = fread(buffer, 1, size, file);
	fclose(file);
	assert(L < size);
	return string_to_JSON(std::string(buffer, buffer + L));
}
#endif