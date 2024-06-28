#pragma once
#include "Type.h"
#include "Identifier.h"

namespace htf {
	namespace parse {
		// 存储函数单个参数
		class Fun_arg {
		public:
			Fun_arg() = delete;
			Fun_arg(const Type&, const stream::Identifier&);

			// 返回  _type + " " + _name
			std::string str() const;

		private:
			Type _type;
			stream::Identifier _name;		// 可为空
		};
	}
}