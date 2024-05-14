#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "output.h"

using namespace std;

// ---------- color ---------------
static const string RESET = "\033[0m";
static const string RED = "\033[31m";
static const string GREEN = "\033[32m";
static const string YELLOW = "\033[33m";
static const string BLUE = "\033[34m";
static const string WHITE = "\033[37m";

static const string Print_Str_Sta = "  ";
static const string Print_Str_End = "  ";
static const string Enter_Str = YELLOW + "-- " + GREEN + "enter " + RESET;
static const string Result_Str = GREEN + "--> " + RESET;
static const string Warn_Str = YELLOW + "~~ " + RESET;
static const string Error_Str = RED + "~~ " + RESET;


// // 删除异常中字符串的头部 (A::B: ...)
// static void delete_head(string& s)
// {
// 	auto index = s.find("::");
// 	auto len = s.length();
// 	if (index == string::npos || index == len - 1) return;

// 	index = s.find(':', index + 2);
// 	if (s[index + 1] == ' ') index++;
// 	s.replace(0, index + 1, "");
// }


// 返回颜色
static string ret_color(int flag) 
{
    string color;
    switch(flag) {
        case 1:
        {
            color = GREEN;
            break;
        }
        case 0:
        {
            color = YELLOW;
            break;
        }
        case -1:
        {
            color = RED;
            break;
        }
        default:
            return "";
    }
    return color;
}

static vector<pair<size_t, size_t>> ret_indexs(const string& s)
{
    vector<pair<size_t, size_t>> res;
    pair<size_t, size_t> temp;
    size_t index = 0;
    while (true) {
        index = (s.find('"', index) == string::npos) ? -1 : s.find('"', index);
        if (index == -1 || index == s.length() - 1) break;
        temp.first = index;

        index = (s.find('"', index + 1) == string::npos) ? -1 : s.find('"', index + 1);
        if (index == -1) break;
        temp.second = index;
        res.emplace_back(temp);
        index++;
    }
    return res;
}

static string color_string(const string& s, const string& color, const string& target = WHITE)
{
    vector<pair<size_t, size_t>> indexs = ret_indexs(s);
    if (indexs.empty()) {
        return color + s;
    }
    string res = color + s.substr(0, indexs[0].first);
    // "123" 567 "9"   0,4  8,10

    for (size_t i = 0; i < indexs.size(); i++) {
        res += target + "\"";
        res += s.substr(indexs[i].first + 1, indexs[i].second - indexs[i].first - 1) + "\"" + color;
        if (i + 1 < indexs.size())      // 加上双引号外的字符
            res += s.substr(indexs[i].second + 1, indexs[i + 1].first - indexs[i].second - 1);
    }
    res += s.substr(indexs.back().second + 1);
    return res;
}

namespace output{  
    void prompt()
    {
        
    }

    void print(const string& s)
    {
        cout << Print_Str_Sta << s << Print_Str_End << endl;
    }

    void print_enter(const string& s)
    {
        cout << Enter_Str + color_string(s, GREEN) << ": " + RESET;
    }

    void print_result(const string& s, bool is_line)
    {
        cout << Result_Str << color_string(s, GREEN) + RESET;
        if (is_line) cout << endl;
    }

    void print_warn(string s, bool is_line)
    {
        // if (is_user) delete_head(s);
        cout << Warn_Str << color_string(s, YELLOW) + RESET;
        if (is_line) cout << endl;
    }

    void print_error(string s, bool is_line)
    {
        // if (is_user) delete_head(s);
        cerr << Error_Str << color_string(s, RED) + RESET;
        if (is_line) cout << endl;
    }

    void print_indentation(ostream& os, int count)
    {
        for (int i = 0; i < count; i++) os << "\t";
    }


    string mark_string(const string& s, const string& c)
    {
        return (" " + c + s + c + " ");
    }

    string mark_char(char s, char c)
    {
        string res = " ";
        res += c;
        res += s;
        res += c;
        res += " ";
        return res;
    }
}