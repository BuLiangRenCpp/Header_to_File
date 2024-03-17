#pragma once

#include "Ins_arg_key.h"
#include "Ins_arg_con.h"


// 参数
class Ins_arg {
public:
	Ins_arg();
	// 默认无参参数
	Ins_arg(const Ins_arg_key&, const Ins_arg_con& c = Ins_arg_con{});

	Ins_arg_key key() const;
	Ins_arg_con con() const;
	bool empty() const;
    // 是否是 默认参数 default_arg
    // 有 _con 无 _key
    bool is_darg() const;

    // 只能用于修改默认参数的key
    void change_darg(const Ins_arg_key& k);

private:
	Ins_arg_key _key;		// 保存参数，可为空
	Ins_arg_con _con;		// 参数后跟的内容，可为空
	bool _harg;		// 是否是有参参数  -->  自动推导
};


// ------------------------------- 重载 --------------------------------------------

std::istream& operator>>(std::istream&, Ins_arg&);