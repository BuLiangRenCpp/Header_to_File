#include "Class.h"
#include "Excep_dev.h"
#include "Excep_syntax.h"
#include "usage_lex.h"
#include "output.h"

using namespace std;
using namespace output;

namespace htf {
    using namespace lex;

    namespace parse {
        Class::Class()
            :_name{ }, _funs{ }, __vars{ }
        {
            
        }

        Class::Class(const stream::Identifier& n, const vector<vector<Function>>& funs)
            :_name{ n }, _funs{ funs }, __vars{ }
        {
            if (funs.size() != 3) 
                throw exception::Excep_dev("Class_fun::Class_fun", _LINE + 
                    "argument 'vector<vector<Function>*> funs.size' must be 3");
            if (n.empty() && (!funs[0].empty() || !funs[1].empty() || !funs[2].empty())) 
                throw exception::Excep_dev("Class_fun::Class_fun", _LINE + "class name don't empty");
        }

        vector<Function> Class::_get_function(Lex& lex)
        {
            vector<Function> res;
            Function f;
            while (true) {
                f.get(lex, true);
                if (f.empty()) {
                    if (!f.fail())     // * 先判断是否是定义成员变量语句
                        __vars.emplace_back(f.var());
                    else if ((lex.peek().val == "private" || lex.peek().val == "public" ||
                        lex.peek().val == "protected" || lex.peek().kind == '}')) break;
                    else lex.ignore();      // ! 错误恢复
                    continue;
                }
                else res.emplace_back(f);
            }
            return res;
        }

        static void unite(vector<Function>& ts, const vector<Function>& t)
        {
            for (const auto& x : t) 
                ts.emplace_back(x);
        }

        // (pub, pro, pri)
        // is_class = true: class关键字定义的类
        // 读取 {} 中的函数，留下类结尾的 ';'   --->   因为有的可能有变量名
        void Class::_get_functions(Lex& lex, bool is_class)
        {
            vector<Function> pub, pro, pri;
            _funs.clear();
            auto token = lex.get();
            while (token.kind != '}' && !token.empty()) {
                if (token.val == "public") {	// 1. public
                    if (lex.peek().kind != ':')
                        throw exception::Excep_syntax(lex.hpath().str(), lex.line(), "after"
                             + mark_string(token.val) + "lack of" + mark_char(':'));
                    token = lex.get();
                    unite(pub, _get_function(lex));
                }
                else if (token.val == "protected") {		// 2. protected
                    if (lex.peek().kind != ':')
                        throw exception::Excep_syntax(lex.hpath().str(), lex.line(), "after"
                             + mark_string(token.val) + "lack of" + mark_char(':'));
                    token = lex.get();
                    unite(pro, _get_function(lex));
                }
                else if (token.val == "private") {	  // 3. private
                    if (lex.peek().kind != ':')
                        throw exception::Excep_syntax(lex.hpath().str(), lex.line(), "after"
                             + mark_string(token.val) + "lack of" + mark_char(':'));
                    token = lex.get();
                    unite(pri, _get_function(lex));
                }
                else {		// 4. 默认 (struct-public class-private)
                    lex.putback(token);
                    if (is_class) unite(pri, _get_function(lex));
                    else unite(pub, _get_function(lex));
                }
                token = lex.get();
            }
            _funs.emplace_back(pub);
            _funs.emplace_back(pro);
            _funs.emplace_back(pri);
        }

        Class Class::get(Lex& lex)
        {
            _clear();
            auto token = lex.get();		// 1. class、struct
            if (!usage::is_class_key(token.val)) {
                if (!token.empty()) lex.putback(token);
                return *this;
            }
            string class_key = token.val;

            token = lex.get();	// 2. 类名  (一定是 type --> 由 lex 处理)
            if (token.kind == Lexer_kind::TYPE_KIND && usage::is_identifier(token.val))		
                _name = stream::Identifier{token.val};
            else throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after" + mark_string(class_key) +
                "lack of identifier(class-name), " + mark_string(token.val) + "isn't a legal class-name"};
            
            if (lex.peek().kind != '{')     
                throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after" +  
                    mark_string(class_key + _name.str()) + "lack of" + mark_char('{')};
            token = lex.get();

            if (class_key == "struct") _get_functions(lex, false);
            else if (class_key == "class") _get_functions(lex, true);

            // 4. 剩下 ';'，可能会有变量名，直接忽略到 ';'
            if (lex.peek().kind != ';' && lex.peek().kind != Lexer_kind::IDENTIFIER_KIND)
                throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after" +
                    mark_string(class_key + " " + _name.str() + "{}") + "lack of" + mark_char(';')};
            lex.ignore();
            return *this;
        }

        string Class::str(unsigned int count) const
        {
            if (this->empty()) return "";
            string res;
            string indentation(count, '\t');
            if (!_funs[0].empty()) {        // public
                res += indentation + "// public\n";
                for (auto i = _funs[0].begin(); i != _funs[0].end(); i++) {
                    res += i->str_class(_name, count, __vars);
                    if (i != _funs[0].end() - 1) res += "\n";
                }
            }
            if (!_funs[1].empty()) {        // protected
                res += "\n" + indentation + "// protected\n";
                for (auto i = _funs[1].begin(); i != _funs[1].end(); i++) {
                    res += i->str_class(_name, count, __vars);
                    if (i != _funs[1].end() - 1) res += "\n";
                }
            }
            if (!_funs[2].empty()) {
                res += "\n" + indentation + "// private\n";
                for (auto i = _funs[2].begin(); i != _funs[2].end(); i++) {
                    res += i->str_class(_name, count, __vars);
                    if (i != _funs[2].end() - 1) res += "\n";
                }
            }
            return res;
        }

        // 进判断是否有函数
        bool Class::empty() const
        {
            bool res = true;
            for (const auto& t : _funs) {
                if (!t.empty()) {
                    res = false;
                    break;
                }
            }
            return res;
        }

        void Class::_clear()
        {
            *this = Class{};
        }
    }
}