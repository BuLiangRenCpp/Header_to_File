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
	extern const string DLL_API IDEFAULT_EXTENSION;
	extern const string DLL_API ODEFAULT_EXTENSION;

	DLL_API bool is_header_extension(const string&);

    // 是否是存在的file or dir
	DLL_API bool is_exist(const string&);
    // 是否是存在的file，如果不存在或者是目录返回false
    DLL_API bool is_exist_file(const string&);
	// 判断是否是电脑中的目录，如果不存在或者是file返回false
	DLL_API bool is_exist_dir(const string&);
	// 两个路径的父目录是否相同
	DLL_API bool is_same_parent_dir(const string&, const string&);


    // *********** 以下函数不会判断路径是否合法 **************

	// 默认返回的有扩展名
	DLL_API string file_name(const string&, bool have_extension = true);
	DLL_API string parent_dir(const string&);
	// 替换文件扩展名，路径格式必须正确
	DLL_API string replace_extension(const string&, const string& extension = ODEFAULT_EXTENSION);
	// 返回目录下指定扩展名的所有文件的文件名
	DLL_API vector<string> find_same_extension_files(const string&, const string& extension = IDEFAULT_EXTENSION);
	// 返回当前所在目录，在末尾追加 "/"
	DLL_API string current_dir();
	// 返回扩展名（程序允许的扩展名）
	DLL_API string extension(const string&);

	DLL_API void remove_file(const string&);
}