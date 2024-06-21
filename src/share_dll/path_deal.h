#pragma once
#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "dll.h"
#include "header.h"

// ** 注: path_deal 中的 file_extension 是已经定义在源文件中的，其余的都认为不是扩展名
namespace htf {
	namespace path_deal {
		extern const std::string DLL_API IDEFAULT_EXTENSION;
		extern const std::string DLL_API ODEFAULT_EXTENSION;
		// "" 返回 true
		DLL_API bool is_header_extension(const std::string&);
		// "" 返回 true
		DLL_API bool is_source_extension(const std::string&);

		// 是否是存在的file or dir
		DLL_API bool is_exist(const std::string&);
		// 是否是存在的file，如果不存在或者是目录返回false
		DLL_API bool is_exist_file(const std::string&);
		// 判断是否是电脑中的目录，如果不存在或者是file返回false
		DLL_API bool is_exist_dir(const std::string&);
		// 两个路径的父目录是否相同
		DLL_API bool is_same_parent_dir(const std::string&, const std::string&);
		// 默认返回的有扩展名
		DLL_API std::string file_name(const std::string&, bool have_extension = true);
		// 返回扩展名
		DLL_API std::string extension(const std::string&);
		DLL_API std::string parent_dir(const std::string&);
		// 替换文件扩展名，路径格式必须正确
		DLL_API std::string change_extension(const std::string&, const std::string& extension = ODEFAULT_EXTENSION);
		// 返回目录下指定目录下的匹配正则表达式的所有文件的文件名
		// 规则： '*' 匹配任意字符，'.' 就表示 \.，其余同 C++ <regex>
		// ! 目前仅支持目录下的第一层文件匹配 ( -- 下一步是否扩展到匹配目录，多层文件匹配等 --)
		DLL_API std::vector<std::string> regex_find_files(const std::string&, std::string pattern = "*." + IDEFAULT_EXTENSION);
		// 搜寻指定目录下的程序允许的C++头文件
		DLL_API std::vector<std::string> find_cpp_header_files(const std::string& dir);
		// 返回当前所在目录
		DLL_API std::string current_dir();
		// 处理路径中的 ./  ../ 
		// 输出相对于 source 的规范化路径
		// 如果是不存在的目录，则自动加上后缀名然后判断是否存在
		// 			1. 如果存在：返回对应的路径
		// 			2. 不存在：返回原目录
		DLL_API std::string normalize(const std::string& sourse, const std::string& target);

		DLL_API bool remove_file(const std::string&);
		DLL_API bool create_dirs(const std::string&);
	}
}