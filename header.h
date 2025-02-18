#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <Windows.h>
#include <algorithm>

using namespace std;

vector<string> KEY(const string& key, const int& rounds);
string AddRoundKey(const string& text, const string& key);
string SubBytes(const string& text);
string ShiftRows(const string& text, const bool& crypt);
string MixColumns(const string& text, const vector<int>& initialization);
