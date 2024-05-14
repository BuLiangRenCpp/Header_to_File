#pragma once
#include "header.h"

namespace htf {
	namespace argument{
		// 指令中的content，目前只是路径，按照路径处理
		// ** 采取 string 存储，避免扩展功能
		// 如果路径的格式跟特殊字符或者参数格式一致，不强制要求情况下是不允许相同的
		// ** 自动将特殊字符转为对应的路径，无扩展名的文件自动加上 
		class Arg_con {
		public:
			Arg_con();
			Arg_con(const std::vector<std::string>&);

			std::vector<std::string> con() const;
			bool insert(const Arg_con&);
			bool empty() const;

		private:
			std::vector<std::string> _con;	
		};

		// ! 下一个字符为 '-' 时停止读取
		std::istream& operator>>(std::istream&, Arg_con&);
	}
}