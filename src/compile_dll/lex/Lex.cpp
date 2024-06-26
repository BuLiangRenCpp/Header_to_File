#include "Lex.h"
#include "usage_lex.h"
#include "output.h"
#include "Excep_dev.h"
#include "Excep_syntax.h"

using namespace std;
using namespace output;

const string DLL_API_Str = "DLL_API";

namespace htf {
    using namespace excep;
    namespace lex {
        Lex::Lex(istream& is, const path_deal::Hpath& hpath, const std::vector<std::string>& basic_types, const std::vector<std::string>& containers)
            :_ts{ is }, _buffer{ }, _hpath{ hpath }, _basic_types{ basic_types }, _containers{ containers }
        {

        }

        bool Lex::eof()
        {
            return (this->peek().empty());
        }

        void Lex::putback(const Lexer& t)
        {
			if (t.empty()) 
				throw Excep_dev("Token_stream::putback", _LINE + "don't putback a empty Token");
			_buffer.putback(t);
        }

        void Lex::ignore(const Lexer& t)
        {
			if (this->eof())  
				return;
			if (_buffer.empty()) this->peek();	// * 会将下一个token提到buffer中，因此只需要判断 buffer 即可 
			if (_buffer.get() == t) {	 // * get -> 将 eof 提到 buffer 中的 token 删除
				return;
			}
			this->ignore(t);
        }

        void Lex::ignore_between_bracket()
        {
            if (usage::is_lbracket(this->peek().kind)) {
                char token_ch = this->get().kind;
                char target_ch = usage::ret_rbracket(token_ch);
                if (this->peek().kind == target_ch) {       // * 避免 _buffer.peek 中就是 右括号
                    this->get();
                    return;
                }
                _buffer.clear();
                _ts.putback( stream::Token{ token_ch } );
                _ts.ignore_between_bracket();
            }
        }

        Lexer Lex::get()
        {
            if (!_buffer.empty()) {
                // ! *******************************
                if (_buffer.peek().val == DLL_API_Str) {
                    _buffer.get();
                    return this->get();
                }
                // ! *******************************
                return _buffer.get();
            }
            // ! *******************************
            if (_ts.peek().val == DLL_API_Str) {
                _ts.get();
                return this->get();
            }
            // ! *******************************

            // ********** 忽略预处理指令、类型别名、template、namespace A = xx; ********
            if (_ts.peek().kind == '#') {
                _ts.ignore_line();
                return this->get();
            }
            else if (_ts.peek().val == "using" || _ts.peek().val == "typedef") {
                _deal_define_type();
                return this->get();
            }
            else if (_ts.peek().val == "template") {
                _deal_template();
                return this->get();
            } 
            else if (_ts.peek().val == "namespace") {
                _ts.get();
                auto name = _get_namespace(false);
                if (_ts.peek().kind == '=') {        // 处理 namespace A = xx;
                    _ts.ignore();
                    return this->get();
                }
                return name;
            }
            // *****************************************************************

            auto token = _ts.get();
            if (token.val == "inline") return this->get();
            if (token.val == "operator") {      // * operator 重载运算符函数名
                token.val += _get_operator().val;
                return Lexer{ Lexer_kind::IDENTIFIER_KIND, token };
            }
            if (token.empty()) return Lexer{};

            // **** const 处理：如果其后跟 type 则为 type，否则为 identifier ****
            Lexer lexer{ token };
            Lexer lexer_namespace;
            bool is_const_type = false;
            if (token.val == "const") {
                lexer_namespace = _get_namespace();     // 可能后面是namespace 
                if (lexer_namespace.empty() && !_is_type(_ts.peek())) return lexer;
                else lexer.val += " " + lexer_namespace.val;
                is_const_type = true;
            } else {
                _ts.putback(token);
                lexer = Lexer{};
            }        
            
            // * 函数定义语句一定以标识符开头            
            // * 作用域开头的看做 type
            if (!is_const_type) lexer_namespace = _get_namespace();             
            stream::Token peek = _ts.peek();
            if (!_is_type(peek)) {    // * 1. not type
                if (!lexer_namespace.empty()) {     // ! 看做标识符
                    peek = _ts.get();
                    if (peek.kind != Lexer_kind::IDENTIFIER_KIND)
                        throw Excep_syntax{_hpath.str(), _ts.line(), "after namespace" + 
                            mark(lexer_namespace.val) + "lack of identifier"};
                    peek.val = lexer_namespace.val + peek.val;
                    return Lexer{ peek };
                }
                if (peek.val == "class" || peek.val == "struct") {
                    peek = _ts.get();
                    _deal_class(false);
                    return peek;
                }
                return Lexer{ _ts.get() };
            } else {    // * 2. type  (可能以作用域开头)
                if (!is_const_type) lexer.val += lexer_namespace.val;
                lexer.kind = Lexer_kind::TYPE_KIND;     // 避免为 empty
                // 1. 多个 basic_type
                if (_is_basic_type(peek)) 
                    lexer.val += _get_basic_type().val;
                else 
                    lexer.val += _get_container().val;
                return lexer;
            }
            return Lexer{};
        }

		Lexer Lex::peek()
		{
			if (_buffer.empty_peek()) 
                _buffer.push_peek(this->get());
			return _buffer.peek();
		}

        line_t Lex::line() const
        {
            return _ts.line();
        }

        vector<string> Lex::basic_types() const
        {
            return _basic_types;
        }

        vector<string> Lex::containers() const
        {
            return _containers;
        }

        path_deal::Hpath Lex::hpath() const
        {
            return _hpath;
        }

        bool Lex::_is_basic_type(const stream::Token& t) const
        {
            return t.kind == Lexer_kind::IDENTIFIER_KIND && 
                (usage::is_basic_type_key(t.val) || my_std::is_in(t.val, _basic_types));
        }

        bool Lex::_is_container(const stream::Token& t) const
        {
            return t.kind == Lexer_kind::IDENTIFIER_KIND && 
                (usage::is_container_key(t.val) || my_std::is_in(t.val, _containers));
        }

        bool Lex::_is_type(const stream::Token& t) const
        {
            return _is_basic_type(t) || _is_container(t);
        }
        
        Lexer Lex::_get_namespace(bool is_type)
        {
            if (_ts.peek().kind != Lexer_kind::IDENTIFIER_KIND) {
                if (!is_type) 
                    throw Excep_syntax{_hpath.str(), _ts.line(), mark(_ts.peek().val) + 
                        "isn't a legal namespace"};
                return Lexer{};
            }
            else {
                stream::Token token = _ts.get();
                Lexer lexer;
                // 读取 'A::A:: ... A::'
                while (_ts.peek().kind == Lexer_kind::DCHAR_KIND && _ts.peek().val == "::") {
                    lexer.val += token.val;
                    lexer.val += _ts.get().val;
                    token = _ts.get();
                    if (_is_type(token) || token.kind != Lexer_kind::IDENTIFIER_KIND) 
                        break;
                }
                if (is_type) _ts.putback(token);
                else {
                    lexer.val += token.val;
                    if (token.kind != Lexer_kind::IDENTIFIER_KIND) 
                        throw Excep_syntax{_hpath.str(), _ts.line(), mark(lexer.val) + 
                            "isn't a legal namespace"};
                }
                if (!lexer.empty()) lexer.kind = Lexer_kind::NAMESPACE_KIND;
                return lexer;
            }
            return Lexer{};
        }

        Lexer Lex::_get_basic_type()
        {
            Lexer lexer;
            if (_is_basic_type(_ts.peek())) {       // ! 不采用 putback，避免各个函数之间 _ts.putback 冲突
                auto token = _ts.get();
                lexer.kind = Lexer_kind::TYPE_KIND;
                lexer.val += token.val;
                // ******** 多个basic_type、指针、引用、数组 ***********      
                stack<char> stk;
                while (!_ts.eof()) {
                    token = _ts.peek();
                    if (_is_basic_type(token)) 
                        lexer.val += " " + token.val;
                    else if (token.kind == '&' || token.kind == '*') 
                        lexer.val += token.val;
                    else if (token.kind == '[') {
                        stk.push('[');
                        lexer.val += token.val;
                    }
                    else if (token.kind == ']') {
                        if (stk.empty()) 
                            throw Excep_syntax{_hpath.str(), _ts.line(), "after array" + mark(lexer.val) + 
                                "lack of" + mark('[')};
                        stk.pop();
                        lexer.val += token.val;
                    }
                    else break;
                    _ts.get();
                }
                if (!stk.empty()) 
                    throw Excep_syntax{_hpath.str(), _ts.line(), "after array" + mark(lexer.val) + 
                                "lack of" + mark(']')};
            }
            if (lexer.empty()) 
                throw Excep_dev{"Lex::_get_basic_type()", _LINE + "empty basic type"};
            return lexer; 
        }

        Lexer Lex::_get_container()
        {
            Lexer lexer;
            if (_is_container(_ts.peek())) {
                auto token = _ts.get();
                lexer.kind = Lexer_kind::TYPE_KIND;
                lexer.val += token.val;
                if (_ts.peek().kind != '<') 
                    throw Excep_syntax{_hpath.str(), _ts.line(), "after template" + mark(token.val) 
                        + "lack of" + mark('<')};

                token = _ts.get();
                stack<char> stk;    // 用于判断 <> 是否合法 
                stk.push(token.kind);
                lexer.val += token.val;
                // 1. 读取 <> 之间的部分
                while (!_ts.eof() && !stk.empty()) {
                    token = _ts.get();
                    switch (token.kind) {
                    case Lexer_kind::IDENTIFIER_KIND:
                    {
                        _ts.putback(token);
                        Lexer lexer_namespace = _get_namespace();       // * 注意可能有 namespace 
                        if (!lexer_namespace.empty()) token = _ts.peek();
                        if (_is_container(token)) 
                            lexer.val += lexer_namespace.val + _get_container().val;
                        else if (_is_basic_type(token)) 
                            lexer.val += lexer_namespace.val + _get_basic_type().val;
                        else {
                            if (!stk.empty())
                                throw Excep_syntax{_hpath.str(), _ts.line(), "in container" + mark(lexer.val + ">") 
                                    + ", type" + mark(token.val) + "don't be declared"};
                        }
                        break;
                    }
                    case '<': 
                    {
                        stk.push('<');
                        lexer.val += token.val;
                        break;
                    }
                    case '>': 
                    {
                        if (stk.empty()) 
                            throw Excep_syntax{_hpath.str(), _ts.line(), "container" + 
                                mark(lexer.val) + "lack of" + mark('>')};
                        stk.pop();
                        lexer.val += token.val;
                        break;
                    }
                    case ',':
                    {
                        if (stk.empty()) {  // * < > 之内才允许 ','
                            _ts.putback(token);
                            return lexer;
                        }
                        lexer.val += token.val + " ";
                        break;
                    }
                    default:
                    {
                        _ts.putback(token);
                        return lexer;
                    }
                    }
                }
            }
            // 2. 读取 <> 之后的部分 (可能有 & *)
            while (_ts.peek().kind == '&' || _ts.peek().kind == '*') 
                lexer.val += _ts.get().val;
            if (lexer.empty()) 
                throw Excep_dev{"Lex::_get_container()", _LINE + "empty contanier"};
            return lexer;          
        }

        void Lex::_deal_define_type()
        {
            stream::Token token = _ts.get();
            if (token.val == "typedef") {       // typedef  old_type  new_type ;
                if (usage::is_class_key(_ts.peek().val)) {  // typedef struct { } new_type ;
                    _ts.ignore_until();
                    _ts.ignore_between_bracket();
                }
                while (!_ts.eof()) {
                    if (_ts.peek().kind == ';') {
                        if (!my_std::is_in(token.val, _basic_types)) _basic_types.emplace_back(token.val);
                        _ts.get();
                        break;
                    }
                    token = _ts.get();
                }
            }
            else if (token.val == "using") {
                if (_ts.peek().val == "namespace")    // using namespace xx;
                    _ts.ignore();     
                else {
                    token = _ts.get();
                    if (!token.empty() && token.kind == stream::Token_kind::IDENTIFIER_KIND 
                        && _ts.peek() == '=')    // using new_type = type; 
                            if (!my_std::is_in(token.val, _basic_types)) _basic_types.emplace_back(token.val);
                    _ts.ignore();
                }
            }
            else throw Excep_dev{"Lex::_deal_define_type()", _LINE + mark(token.val) + "isn't typedef or using"};
        }

        void Lex::_deal_template()
        {
            stream::Token token = _ts.get();
            if (token.val != "template")
                throw Excep_dev{"Lex::_deal_template", _LINE + mark(token.val) + "isn't" + mark("template")};
            if (_ts.peek().kind != '<') 
                throw Excep_syntax{_hpath.str(), _ts.line(), "after" + mark("template") + 
                    "lack of" + mark('<')};
            _ts.ignore_between_bracket();
            if (usage::is_class_key(_ts.peek().val)) {      // 1. 模板类
                _deal_class();
                _ts.ignore_between_bracket();
                _ts.ignore();
            } else {        // 2. 模板函数  
                _ts.ignore(stream::Token{ ')' });       // * 避免是定义语句
                if (_ts.peek().kind == ';') _ts.ignore();
                else _ts.ignore_between_bracket();
            }
        }

        // 将 class 加入自定义类型，处理掉 class |...| A |...| {} 中 |...| 之间的部分
        void Lex::_deal_class(bool is_template)
        {
            stream::Token token = _ts.get();
            while (!_ts.eof()) { // class (struct) A {  }; | class DLL_API A : public AA { }; | class A;
                if (_ts.peek().kind == ':' || _ts.peek().kind == '{' || _ts.peek().kind == ';') {
                    if (is_template) {
                        if (!my_std::is_in(token.val, _containers)) _containers.emplace_back(token.val);
                    } else {
                        if (!my_std::is_in(token.val, _basic_types)) _basic_types.emplace_back(token.val);
                    } 
                    break;
                }
                token = _ts.get();
            }
            // 忽略掉 |: xxx|  { }
            if (!_ts.eof() && _ts.peek().kind != ';')    // 避免 class A ;
                _ts.ignore_until(stream::Token{ '{' });
            // * 当不是模版时，说明当前 class 不能忽略
            // * 在调用此函数前，'class' 已经被读取在临时变量中(get()函数的)了，
            // *    而现在 token 保存的是 'class' 后的标识符，get() 函数需要返回的是 'class'，所以此标识符需要被 putback
            if (is_template == false) _ts.putback(token);       
        }

        stream::Token Lex::_get_operator()
        {
            stream::Token res{};
            auto peek = _ts.peek();
            // 1. new delete
            if (peek.val == "new" || peek.val == "delete") {    
                res.val += " " + _ts.get().val;
                if (_ts.peek().kind == '[') {
                    res.val += _ts.get().val;
                    if (_ts.peek().kind == ']') res.val += _ts.get().val;
                    else throw Excep_syntax{_hpath.str(), _ts.line(), "after" + mark(res.val) + 
                        "lack of" + mark(']')};
                }
            }
            // 2. () []
            else if (usage::is_lbracket(peek.kind) && peek.kind != '<' && peek.kind != '{') {
                res.val += _ts.get().val;
                char c = usage::ret_rbracket(res.val[0]);
                if (c == _ts.peek().kind) res.val +=  _ts.get().val;
                else if (usage::is_bracket(_ts.peek().kind))
                    throw Excep_syntax{_hpath.str(), _ts.line(), "after" + mark(res.val) + "lack of" + 
                        mark(c)};
                else throw Excep_syntax{_hpath.str(), _ts.line(), "after" + mark("operator") + 
                    "lack of operator-char"};
            }
            // 3. ->
            else if (peek.kind == Lexer_kind::DCHAR_KIND) {
                if (peek.val == "->") res.val += _ts.get().val;
                else throw Excep_syntax{_hpath.str(), _ts.line(), mark(peek.val) + "isn't legal operator"};
            }
            // 4. << 、>> 、other 
            else if (usage::is_spe_ch(peek.kind)) {
                while (!_ts.eof()) {
                    if (_ts.peek().kind == '(') break;
                    res.val += _ts.get().val;
                }
            }
            else throw Excep_syntax{_hpath.str(), _ts.line(), "after" + mark("operator") + 
                    "lack of operator-char"};
            return res;
        }
    }
}