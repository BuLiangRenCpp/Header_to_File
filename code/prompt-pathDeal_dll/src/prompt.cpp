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


namespace prompt{  
    void prompt()
    {
        print("指令格式请见 https://gitee.com/buliangrencpp/Header_to_File");
        print("注: 1. 目录或者路径必须是已经存在的");
        print("    2. 文件路径是基于 windows 平台的");
        print("    3. encoding = GB2312");
        cout << endl;
    }

    void print(const string& s)
    {
        cout << Print_Str_Sta << s << Print_Str_End << endl;
    }

    void print_enter(const string& s)
    {
        cout << Enter_Str << GREEN + s << ": " + RESET;
    }

    void print_result(const string& s, bool is_line)
    {
        cout << Result_Str << GREEN + s + RESET;
        if (is_line) cout << endl;
    }

    void print_warn(string s, bool is_user, bool is_line)
    {
        if (is_user) delete_head(s);
        cout << Warn_Str << YELLOW + s + RESET;
        if (is_line) cout << endl;
    }

    void print_error(string s, bool is_user, bool is_line)
    {
        if (is_user) delete_head(s);
        cerr << Error_Str << RED + s + RESET;
        if (is_line) cout << endl;
    }


    string mark_string(const string& s, const string& c, int flag)
    {
        string color = ret_color(flag);
        if (color == "") throw string("prompt::mark_string: flag参数只能取 1、0、-1");
        return (" " + c + BLUE + s + color + c + " ");
    }

    string mark_char(char s, char c, int flag)
    {
        string color = ret_color(flag);
        if (color == "") throw string("prompt::mark_char: flag参数只能取 1、0、-1");
        if (s == '\"') c = '\'';
        string res = " ";
        res += c;
        res += BLUE;
        res += s;
        res += color;
        res += c;
        res += ' ';
        return res;
    }
}