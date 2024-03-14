#include "usage.h"

using namespace prompt;


const set<string> Basic_Type_Keys{ "void", "bool", "char", "short", "int", "long", "long long",
"float", "double", "long double", "istream", "ostream", "string" };        // 基本类型关键字
const set<string> Complex_Type_Keys{ "vector", "list", "deque", "array", "forwarod_list", "set", "multiset",
    "map", "multimap", "unordered_set", "unordered_multiset", "unordered_map", "unordered_multimap",
    "stack", "queue", "priority_queue", "pair" };
const set<string> Modifier_Type_Keys{ "unsigned", "signed", "const", "constexpr", "static" };     // 类型修饰词

const set<string> Class_Keys{ "class", "struct" };       // 自定义类关键字

const set<char> Lbrackets{ '(', '[', '{' };      // 左括号集
const set<char> Rbrackets{ ')', ']', '}' };      // 右括号集


static char ret_rbracket(const char& c)
{
    switch (c) {
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    default:
        throw string("usage.cpp::ret_rbracket: " + mark_char(c) + "不是左括号");
    }
    return ' ';
}

static char ret_lbracket(const char& c)
{
    switch (c) {
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    default:
        throw string("usage.cpp::ret_rbracket: " + mark_char(c) + "不是右括号");
    }
    return ' ';
}

static bool is_modifier_type_key(const string& s)
{
    return Modifier_Type_Keys.count(s);
}

static bool is_container_type_key(const string& s)
{
    return Complex_Type_Keys.count(s);
}

static bool is_basic_type_key(const string& s)
{
    return Basic_Type_Keys.count(s);
}

        // 判断是否是指针或者引用
static bool is_pointer_quote(string temp)
{
    usage::delete_space_pre_suf(temp);
    int len = static_cast<int>(temp.length());
    if (len < 1) return false;
    return (len - 1) == static_cast<int>(temp.rfind('*')) || (len - 1) == static_cast<int>(temp.rfind('&'));
}

// ------------------------------- usage -------------------------------------
namespace usage {
    bool is_lbracket(const char& c)
    {
        return Lbrackets.count(c);
    }

    bool is_rbracket(const char& c)
    {
        return Rbrackets.count(c);
    }

    void delete_space_pre_suf(string& s)
    {
        // suffix
        int len = static_cast<int>(s.length());
        int i = len - 1;
        for (; i >= 0; i--) {
            if (s[i] != ' ') break;
            s.pop_back();
        }
        // prefix
        int count = 0;
        for (int j = 0; j < i; j++) {
            if (s[j] != ' ') break;
            count++;
        }
        s.erase(0, count);
    }

    bool is_identifier(const string& temp)
    {
        string pattern = "^[a-zA-Z_](?:[a-zA-Z0-9_]+)*";
        regex reg(pattern);
        return (regex_match(temp, reg));
    }

    bool is_class_key(const string& s)
    {
        return Class_Keys.count(s);
    }

    bool is_type_key(const string& s)
    {
        return (is_modifier_type_key(s) || is_container_type_key(s) || is_basic_type_key(s));
    }


    void ignore_between_bracket(istream& is)
    {
        char c;
        is >> c;
        if (is.eof()) return;
        if (!usage::is_lbracket(c)) {
            is.putback(c);
            return;
        }
        stack<char> stk;
        stk.push(c);
        char ltarget = c, rtarget = ret_rbracket(c);

        while (!stk.empty()) {
            is >> c;
            if (is.eof()) return;
            if (c == rtarget) stk.pop();
            else if (c == ltarget) stk.emplace(c);
        }
    }

}
