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
            :_name{ }, _funs{ }
        {
            
        }

        Class::Class(const stream::Identifier& n, const vector<vector<Function>>& funs)
            :_name{ n }, _funs{ funs }
        {
            if (funs.size() != 3) 
                throw exception::Excep_dev("Class_fun::Class_fun", _LINE + 
                    "argument 'vector<vector<Function>*> funs.size' must be 3");
            if (n.empty() && (!funs[0].empty() || !funs[1].empty() || !funs[2].empty())) 
                throw exception::Excep_dev("Class_fun::Class_fun", _LINE + "class name don't empty");
        }

        static vector<Function> get_function(Lex& lex)
        {
            vector<Function> res;
            Function f;
            while (true) {
                f.get(lex, true);
                if (f.empty()) {
                    if ((lex.peek().val == "private" || lex.peek().val == "public" ||
                        lex.peek().val == "protected" || lex.peek().kind == '}')) break;
                    lex.ignore();
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
        static vector<vector<Function>> get_functions(Lex& lex, bool is_class)
        {
            vector<Function> pub, pro, pri;
            vector<vector<Function>> res;
            auto token = lex.get();
            while (token.kind != '}' && !token.empty()) {
                if (token.val == "public") {	// 1. public
                    if (lex.peek().kind != ':')
                        throw exception::Excep_syntax(lex.hpath().str(), lex.line(), "after"
                             + mark_string(token.val) + "lack of" + mark_char(':'));
                    token = lex.get();
                    unite(pub, get_function(lex));
                }
                else if (token.val == "protected") {		// 2. protected
                    if (lex.peek().kind != ':')
                        throw exception::Excep_syntax(lex.hpath().str(), lex.line(), "after"
                             + mark_string(token.val) + "lack of" + mark_char(':'));
                    token = lex.get();
                    unite(pro, get_function(lex));
                }
                else if (token.val == "private") {	  // 3. private
                    if (lex.peek().kind != ':')
                        throw exception::Excep_syntax(lex.hpath().str(), lex.line(), "after"
                             + mark_string(token.val) + "lack of" + mark_char(':'));
                    token = lex.get();
                    unite(pri, get_function(lex));
                }
                else {		// 4. 默认 (struct-public class-private)
                    lex.putback(token);
                    if (is_class) unite(pri, get_function(lex));
                    else unite(pub, get_function(lex));
                }
                token = lex.get();
            }
            res.emplace_back(pub);
            res.emplace_back(pro);
            res.emplace_back(pri);
            return res;
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

            token = lex.get();	// 2. 类名 
            if (token.kind == Lexer_kind::TYPE_KIND && usage::is_identifier(token.val))		
                _name = stream::Identifier{token.val};
            else throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after" + mark_string(class_key) +
                "lack of identifier(class-name), " + mark_string(token.val) + "isn't a legal class-name"};
            
            if (lex.peek().kind != '{')     
                throw exception::Excep_syntax{lex.hpath().str(), lex.line(), "after" +  
                    mark_string(class_key + _name.str()) + "lack of" + mark_char('{')};
            token = lex.get();

            if (class_key == "struct") _funs = get_functions(lex, false);
            else if (class_key == "class") _funs = get_functions(lex, true);

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
                    res += i->str_class(_name, count);
                    if (i != _funs[0].end() - 1) res += "\n";
                }
            }
            if (!_funs[1].empty()) {        // protected
                res += "\n" + indentation + "// protected\n";
                for (auto i = _funs[1].begin(); i != _funs[1].end(); i++) {
                    res += i->str_class(_name, count);
                    if (i != _funs[1].end() - 1) res += "\n";
                }
            }
            if (!_funs[2].empty()) {
                res += "\n" + indentation + "// private\n";
                for (auto i = _funs[2].begin(); i != _funs[2].end(); i++) {
                    res += i->str_class(_name, count);
                    if (i != _funs[2].end() - 1) res += "\n";
                }
            }
            return res;
        }

        bool Class::empty() const
        {
            return _name.empty();
        }

        void Class::_clear()
        {
            *this = Class{};
        }
    }
}