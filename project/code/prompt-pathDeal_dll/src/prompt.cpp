#define PROMPT_DLL
#include "prompt.h"

// ---------- color ---------------
static const string RESET = "\033[0m";
static const string RED = "\033[31m";
static const string GREEN = "\033[32m";
static const string YELLOW = "\033[33m";
static const string BLUE = "\033[34m";
static const string WHITE = "\033[37m";

static const string Print_Str_Sta = "---------------------- ";
static const string Print_Str_End = " ---------------------";
static const string Enter_Str = YELLOW + "-- " + GREEN + "enter " + RESET;
static const string Result_Str = GREEN + "--> " + RESET;
static const string Warn_Str = YELLOW + "~~ " + RESET;
static const string Error_Str = RED + "~~ " + RESET;


// 删除异常中字符串的头部 (A::B: ...)
static void delete_head(string& s)
{
	auto index = s.find("::");
	auto len = s.length();
	if (index == string::npos || index == len - 1) return;

	index = s.find(':', index + 2);
	if (s[index + 1] == ' ') index++;
	s.replace(0, index + 1, "");
}


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

// 返回 s 的双引号的下标
static pair<int, int> ret_index(const string& s)
{
    int fi = (s.find('"') == string::npos) ? -1 : s.find('"');
    int se = (s.rfind('"') == string::npos) ? -1 : s.rfind('"');
    return make_pair(fi, se);
}


namespace prompt{  
    void prompt()
    {
        print("指令格式请见 https://gitee.com/buliangrencpp/Header_to_File");
        print("注: 目录或者路径必须是已经存在的");
        cout << endl;
    }

    void print(const string& s)
    {
        cout << Print_Str_Sta << s << Print_Str_End << endl;
    }

    void print_enter(const string& s)
    {
        pair<int, int> indexs = ret_index(s);
        string res = GREEN;
        if (indexs.first == -1 || indexs.second == -1) res += s;
        else {
            res += s.substr(0, indexs.first + 1);
            res += BLUE + s.substr(indexs.first + 1, indexs.second - indexs.first - 1) + GREEN + s.substr(indexs.second);
        } // 0 1 2 3
        cout << Enter_Str + res << ": " + RESET;
    }

    void print_result(const string& s, bool is_line)
    {
        pair<int, int> indexs = ret_index(s);
        string res = GREEN;
        if (indexs.first == -1 || indexs.second == -1) res += s;
        else {
            res += s.substr(0, indexs.first + 1);
            res += BLUE + s.substr(indexs.first + 1, indexs.second - indexs.first - 1) + GREEN + s.substr(indexs.second);
        }
        cout << Result_Str << res + RESET;
        if (is_line) cout << endl;
    }

    void print_warn(string s, bool is_user, bool is_line)
    {
        if (is_user) delete_head(s);
        pair<int, int> indexs = ret_index(s);
        string res = YELLOW;
        if (indexs.first == -1 || indexs.second == -1) res += s;
        else {
            res += s.substr(0, indexs.first + 1);
            res += BLUE + s.substr(indexs.first + 1, indexs.second - indexs.first - 1) + YELLOW + s.substr(indexs.second);
        }
        cout << Warn_Str << res + RESET;
        if (is_line) cout << endl;
    }

    void print_error(string s, bool is_user, bool is_line)
    {
        if (is_user) delete_head(s);
        pair<int, int> indexs = ret_index(s);
        string res = RED;
        if (indexs.first == -1 || indexs.second == -1) res += s;
        else {
            res += s.substr(0, indexs.first + 1);
            res += BLUE + s.substr(indexs.first + 1, indexs.second - indexs.first - 1) + RED + s.substr(indexs.second);
        }
        cerr << Error_Str << res + RESET;
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
        if (s == '\"') c = '\'';
        string res = " ";
        res += c;
        res += s;
        res += c;
        res += ' ';
        return res;
    }
}