#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <deque>
#include <set>
#include <typeinfo>
using namespace std;
#define readNewLine(in, line, ss)\
getline(in, line); \
ss.clear();  ss.str(line);

extern regex atomReg, cnctReg, paraReg, allReg;