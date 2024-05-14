#include "Function.h"
#include "output.h"
#include "Excep_dev.h"
#include "Excep_syntax.h"

using namespace std;
using namespace output;

namespace htf {
    using namespace lex;

    namespace parse {        
        Function::Function()        // 原本不能为空函数的，但是有的情况需要
            :_type{ }, _name{ }, _args{ }, _is_const{ false }, _is_construct{ false }
        {
            
        }
        
        Function::Function(const Type& t, const stream::Identifier& n, const std::vector<Fun_arg>& args, bool is_const, bool is_construct)
            :_type{ t }, _name{ n }, _args{ args }, _is_const{ is_const }, _is_construct{ is_construct }, _is_define{ false }
        {
            if (_is_construct) {    // 1. 析构函数
                if (_is_const)
                    throw exception::Excep_dev{"Function::Function(...)", _LINE + "析构函数不能设为常量函数"};
                if (!_args.empty())
                    throw exception::Excep_dev{"Function::Function(...)", _LINE + "析构函数不能有参数"};
                if (!_name.empty()) 
                    throw exception::Excep_dev{"Function::Function(...)", _LINE + "析构函数函数名应为空"};
            }
            else if (_is_const) {   // 2. 常量函数
                if (_name.empty())     // # 构造函数
                    throw exception::Excep_dev{"Function::Function(...)", _LINE + "构造函数不能设为常量函数"};
            }
            else {      // 3. 一般函数
                if (_name.empty())
                    throw exception::Excep_dev{"Function::Function(...)", _LINE + "缺少函数名"};
            }
        }

        bool Function::empty() const
        {
            return _type.empty() && _name.empty() && 
                _args.empty() && _is_const == false && _is_construct == false;
        }

        bool Function::fail() const
        {
            return this->empty() && !_is_define;
        }

        Function Function::get(Lex& lex, bool is_class)
        {
            return (is_class) ? _get_class_fun(lex) : _get_fun(lex);
        }

        string Function::str(unsigned int count) const
        {
            if (this->empty()) return "";
            if (_is_const || _is_construct)
                throw exception::Excep_dev("Function::str", _LINE + "此函数是特殊函数(类的构造函数或者析构函数)，请使用" +
                     mark_string("str_class()") + "调用");
            string indentation(count, '\t');
            string res = indentation + _type_name() + _ret_args() + "\n";
            res += indentation + "{" + "\n\n" + indentation + "}\n";
            return res;
        }

        string Function::str_class(const stream::Identifier& class_name, unsigned int count) const
        {
            if (this->empty()) return "";
            if (class_name.empty())
                throw exception::Excep_dev{"Function::str_class", _LINE + "class_name don't empty"};
            string indentation(count, '\t');
            string modifier = (_is_construct) ? "~" : "";
            string res = indentation;
            if (_name.empty()) res += class_name.str() + "::" + modifier + _type.str();
            else res += _type.str() + " " + class_name.str() + "::" + _name.str();
            res += _ret_args();
            if (_is_const) res += " const"; 
            res += "\n" + indentation + "{" + "\n\n" + indentation + "}\n";
            return res;
        }

        Function Function::_get_class_fun(Lex& lex)
        {
            _clear();
            if (lex.peek().kind == '~') {       // 析构函数
                lex.get();
                _is_construct = true;
                if (!_get_noname_fun(lex)) { 
                    _clear();
                    return *this;
                } else {
                    if (!_args.empty()) 
                        throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "析构函数" +   
                            mark_string(_type_name() + _ret_args()) + "参数列表应该为空"};
                }
            } 
            else if (lex.peek().kind == Lexer_kind::TYPE_KIND) {
                auto lexer = lex.get();
                if (lex.peek().kind == '(') {       // 构造函数
                    lex.putback(lexer);
                    if (!_get_noname_fun(lex)) { 
                        _clear();
                        return *this;
                    } 
                } else {
                    lex.putback(lexer);
                    return _get_fun(lex, true);   
                }
            }
            return *this;
        }

        Function Function::_get_fun(Lex& lex, bool is_class)
        {
            _clear();
            if (lex.peek().kind == Lexer_kind::TYPE_KIND) {
                _type = Type{ lex.get() };
                if (lex.peek().kind != Lexer_kind::IDENTIFIER_KIND) 
                    throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after function" +   
                        mark_string(_type_name())+ "lack of identifier"};
                _name = stream::Identifier{ lex.get().val };
                if (!_get_args(lex)) {      // * 定义变量 
                    _clear();
                    return *this;
                };
                if (lex.peek().val == "const") {
                    lex.get();
                    if (is_class) _is_const = true;
                    else throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "normal-function" +  
                        mark_string(_str()) + "don't set const function"};
                }
                if (lex.peek().kind == '{') {       // * 定义语句
                    _clear();
                    _is_define = true;
                    lex.ignore_between_bracket();
                }
                else if (lex.peek().kind != ';') 
                    throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after function" +  
                        mark_string(_str()) + "lack of" + mark_char(';')};
            }
            return *this;
        }

        bool Function::_get_noname_fun(Lex& lex)
        {
            if (lex.peek().kind == Lexer_kind::TYPE_KIND) {
                _type = Type{ lex.get() };
                if (lex.peek().kind != '(') 
                    throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after" + mark_string(_type_name()) + 
                        "must be" + mark_char('(')};
                if (!_get_args(lex)) return false;
                if (lex.eof()) return false;
                
                if (lex.peek().kind == '{') {       // * 定义语句
                    _clear();
                    _is_define = true;
                    lex.ignore_between_bracket();
                }
                else if (lex.peek().kind == '=')     // A() = delete;
                    lex.ignore();
                else if (lex.peek().kind != ';') 
                    throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after function" +  
                        mark_string(_str()) + "lack of" + mark_char(';')};
                else lex.ignore();
                return true;
            }   
            return false;
        }

        bool Function::_get_args(Lex& lex)
        {
            if (lex.peek().kind == '(') {
                lex.get();
                bool found = false;
                while (!lex.eof()) {      
                    if (lex.peek().kind == ')') {    // (1) 避免 空参数列表
                        lex.get();
                        found = true;
                        break;
                    }
                    // ********** get a arg **************
                    // 1. type
                    auto type = lex.get();
                    if (type.kind != Lexer_kind::TYPE_KIND) {
                        throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "in function" + 
                            mark_string(_type_name() + "(...)") + "," + mark_string(type.val) + "isn't type"};
                    }
                    // 2. arg_name
                    stream::Identifier name;
                    if (lex.peek().kind == Lexer_kind::IDENTIFIER_KIND) {
                        name = stream::Identifier{ lex.get().val };
                    }
                    _args.emplace_back(Fun_arg{ type, name });
                    // 3. 忽略默认值
                    while (!lex.eof()) {
                        if (lex.peek().kind == ')') break;      // 在 (1) 除才 break 外层循环 
                        if (lex.peek().kind == ',') {
                            lex.get();
                            break;
                        }
                        lex.get();
                    }
                }
                if (!found) 
                    throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after function" + 
                        mark_string(_type_name() + "(") + "lack of" + mark_char(')')};
                return true;
            }
            return false;
        }

        string Function::_type_name() const
        {
            string s = (_is_construct) ? "~" : "";
            if (!_name.empty()) s = " " + _name.str();
            return _type.str() + s;
        }

        string Function::_ret_args() const
        {
            string res = "(";
            auto size = _args.size();
            for (int i = 0; i < size; i++) {		// par1, par2, pa3...
                res += _args[i].str();
                if (i < size - 1) res += ", ";
            }
            return res + ")";
        }

        string Function::_str() const
        {
            return _type_name() + _ret_args();
        }

        void Function::_clear()
        {
            *this = Function{};
        }
    }
}