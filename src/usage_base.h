#pragma once

namespace htf
{

bool is_lbracket(char c);
bool is_rbracket(char c);
bool is_bracket(char c);
// 如果不是返回它本身
char ret_lbracket(char c);
// 如果不是返回它本身
char ret_rbracket(char c);

}//