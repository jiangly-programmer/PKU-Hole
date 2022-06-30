#pragma once

#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <set>

using std::map;
using std::string;
using std::unordered_set;
using std::vector;

template <class T>
using set = std::unordered_set<T>;
template <class T>
using ordered_set = std::set<T>;