#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include <string>
#include <vector>
#include <stack>
#include <set>
#include <unordered_map>

#include <regex>


using namespace std;


// *************** 由于在程序中使用了noskipws可能有时会读取空格，因此采用此方式读取字符 ***********************
namespace my_std{
    inline void getchar(istream& is, char& c)
    {
        while (is >> c) {
            if (!isspace(c)) return; 
        }
    }
}