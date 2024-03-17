#include "user_input.h"

using namespace std;

Ins get_ins()
{
	Ins ins;
	string line;

	getline(cin, line);		// 指令按行输入
	istringstream ist(line);
	ist >> ins;
	return ins;
}