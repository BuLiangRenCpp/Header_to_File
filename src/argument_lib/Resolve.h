#pragma once
#include "Htf_args.h"

namespace htf {
	namespace argument{
		// 解析指令，执行对应操作
		class Resolve {
		public:
			Resolve();

			void resolve(const Htf_args&);

		private:
			void _htf_ins(const Htf_args&);
		};
	}
}

