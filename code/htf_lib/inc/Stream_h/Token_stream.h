#pragma once

#include "Literal.h"
#include "Identifier.h"
#include "Token.h"

#include "path_deal.h"

// *********** 注: 对于 #include "A.h"，会将 A 作为自定义类型，开头字母大写，小写不算
// 对于 Literal (主要是函数参数的默认值) 不返回，直接忽略
// -------------------------- 模拟输入流 ------------------------------------
class Token_stream {
public:
	Token_stream();

	bool eof(istream& is);
	// 清空缓存区
	void sync();
	// 返回 token.kind == TOKEN_NULL_KIND 这说明没有内容了
	Token get(istream& is);
	void putback(Token);
	// 清空直到 token 字符之前(包括 token)的所有字符
	void ignore(istream& is, Token);
	// 功能同 usage.cpp 中的同名函数
	void ignore_between_bracket(istream& is);
	// ** 主要用于忽略参数默认值，此处给 Fun_par类 调用 
	void ignore_default(istream& is);		// 原本想直接读取到 = 就忽略后面的值，但是这种情况是否会造成bug情况位置，故不采用
	Token peek(istream& is);

	// 插入自定义类型的关键字
	void push_type(const string&);

	int line() const;
	// 将 TS 清空到初始状态，用于每个文件对应一个 TS，但是用的 TS 都是同一个类
	void clear();

private:
	bool _full{ false };		// 缓冲区是否有数据
	Token _buffer{};	
	set<string> _types;			// 保存自定义类型
	int _line = 1;
	
	// 忽略注释
	// flag == false: 默认行注释
	void _ignore_note(istream& is, bool flag = false);
};

// ***************************************
// -------------- 公用流 -----------------
extern Token_stream TS;