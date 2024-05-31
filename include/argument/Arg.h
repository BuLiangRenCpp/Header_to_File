#pragma once

#include "Arg_key.h"
#include "Arg_con.h"


namespace htf {
	namespace argument{
		class Arg {
		public:
			Arg();
			// * NULL_ARG (if c.empty() -> NULL,  else -> -i)
			Arg(const Arg_key&, const Arg_con& c = Arg_con{});

			Arg_key key() const;
			Arg_con con() const;
			bool unite(const Arg&);
			bool empty() const;
			// 是否是 默认参数 default_arg
			// 有 _con 无 _key
			bool is_darg() const;

			// // 只能用于修改默认参数的key
			// void change_darg(const Arg_key& k);

		private:
			Arg_key _key;		// 保存参数，可为空
			Arg_con _con;		// 参数后跟的内容，可为空
		};

		std::istream& operator>>(std::istream&, Arg&);
	}
}