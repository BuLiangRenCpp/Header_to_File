#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "Htf_args.h"

/**
 * ! 缺少核心功能 htf 
*/

namespace htf {
	namespace argument{
		// 解析指令，执行对应操作
		class DLL_API Resolve {
		public:
			Resolve();

			void resolve(const Htf_args&);

		private:
			void _htf_ins(const Htf_args&);
		};
	}
}

