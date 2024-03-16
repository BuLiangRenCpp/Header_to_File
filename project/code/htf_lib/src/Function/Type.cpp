#include "Type.h"

using namespace prompt;
using namespace Token_kind;


// ------------------------------------- Type类 ------------------------------------------------

// ----------------- pubilc ---------------------

Type::Type()
    : _type{ "" }
{

}

Type::Type(const string& s)
    : _type{ s }
{

}

string Type::str() const
{
    return _type;
}

bool Type::empty() const
{
    return _type.empty();
}


// ------------------------- 重载 ------------------------------

istream& operator>>(istream& is, Type& t)
{
    string type;
    Token token;
    bool is_container = false;      // 是否是容器类型
    stack<char> stk;        // 保存 '<'

    token = TS.get(is);
    while (token.kind != NULL_KIND) {
        switch (token.kind) {
        case TYPE_KIND: {
            string temp = (type.empty() || type.back() == '<' || type.back() == ' ') ? "" : " ";        // 输出格式化type
            type += temp + token.val;
            break;
        }
        case '*': case '&': case '<': case '>': case ',': {
            if (type.empty()) {
                TS.putback(token);
                t = Type{};
                return is;
            }
            if (token.kind == '<') {
                is_container = true;
                stk.emplace(token.kind);
            }
            else if (token.kind == '>') {
                if (stk.empty())        // 语法错误
                    throw string("Type.cpp::operator>>: 语法错误 -> " + mark_string(type) + "缺少" + mark_char('<') + "或者出现未知类型");
                stk.pop();
            }
            if (token.kind == ',') {        // < > 之内才允许 ,
                if (!is_container) {
                    TS.putback(token);
                    t = Type{ type };
                    return is;
                }
            }
            type += token.kind;
            if (token.kind == ',') type += " ";         // 格式化
            break;
        }
        default: {
            if (!stk.empty())
                throw string("Type.cpp::operator>>: 语法错误 -> " + mark_string(type) + "缺少" + mark_char('>') + "或者出现未知类型");
            TS.putback(token);
            t = Type{ type };
            return is;
        }
        }
        // -- 自定义类型一定在 < 或者 , 之后
        if (TS.peek(is).kind == IDENTIFIER_KIND) {        // < > 中有自定义类型
            if (is_container && !stk.empty()) TS.push_type(TS.peek(is).val);
        }
        token = TS.get(is);
    }
    if (!stk.empty()) 
        throw string("Type.cpp::operator>>: 语法错误 -> " + mark_string(type) + "缺少" + mark_char('>') + "或者出现未知类型");
    t = Type{ type };
    return is;
}

ostream& operator<<(ostream& os, const Type& t)
{
    os << t.str();
    return os;
}