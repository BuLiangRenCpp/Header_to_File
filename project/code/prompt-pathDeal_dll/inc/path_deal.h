#pragma once

#ifdef PATH_DEAL_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


#include "prompt.h"
#include <filesystem>

// ** 注: path_deal 中的 file_extension 是已经定义在源文件中的，其余的都认为不是扩展名
namespace path_deal {
	extern const std::string DLL_API IDEFAULT_EXTENSION;
	extern const std::string DLL_API ODEFAULT_EXTENSION;

	DLL_API bool is_header_extension(const std::string&);
	DLL_API bool is_source_extension(const std::string&);

    // 是否是存在的file or dir
	DLL_API bool is_exist(const std::string&);
    // 是否是存在的file，如果不存在或者是目录返回false
    DLL_API bool is_exist_file(const std::string&);
	// 判断是否是电脑中的目录，如果不存在或者是file返回false
	DLL_API bool is_exist_dir(const std::string&);
	// 两个路径的父目录是否相同
	DLL_API bool is_same_parent_dir(const std::string&, const std::string&);


    // *********** 以下函数不会判断路径是否合法 **************

	// 默认返回的有扩展名
	DLL_API std::string file_name(const std::string&, bool have_extension = true);
	DLL_API std::string parent_dir(const std::string&);
	// 替换文件扩展名，路径格式必须正确
	DLL_API std::string replace_extension(const std::string&, const std::string& extension = ODEFAULT_EXTENSION);
	// 返回目录下指定扩展名的所有文件的文件名
	DLL_API std::vector<std::string> find_same_extension_files(const std::string&, const std::string& extension = IDEFAULT_EXTENSION);
	// 返回当前所在目录
	DLL_API std::string current_dir();
	// 处理路径中的 ./  ../ 
	// 输出相对于 source 的规范化路径
	// 如果是不存在的目录，则自动加上后缀名然后判断是否存在
	// 			1. 如果存在：返回对应的路径
	// 			2. 不存在：返回原目录
	DLL_API std::string normalize(const std::string& sourse, const std::string& target);
	// 返回扩展名（程序允许的扩展名）
	DLL_API std::string extension(const std::string&);

	DLL_API bool remove_file(const std::string&);
	DLL_API bool create_dirs(const std::string&);
}