#define PATH_DEAL_DLL
#define PROMPT_DLL
#include "path_deal.h"

using namespace std;

const set<string> Extensions{ ".txt", ".h", ".hpp", ".h++", ".inl",
		".cpp", ".cxx", ".c++", ".c", ".cc" };			// 扩展名集合
const set<string> Header_Extensions{ ".h", ".hpp", ".h++", ".inl" };		// 头文件扩展名集合
const set<string> Source_Extensions{ ".cpp", ".cxx", ".c++", ".c", ".cc" };

using namespace prompt;
namespace FS = std::filesystem;


static bool is_extension(const string& s)
{
	return Extensions.count(s);
}

// 是否从开头开始有连续 . 的个数超过 3 个
// ** 在 win 平台 FS::exists("...") 返回 true
static bool is_more_point(const string& s)
{
	if (s.length() < 3) return false;
	return (s[0] == '.' && s[1] == '.' && s[2] == '.');
}


namespace path_deal {
	const string IDEFAULT_EXTENSION = ".h";
	const string ODEFAULT_EXTENSION = ".cpp";

	bool is_header_extension(const string& s)
	{
		return Header_Extensions.count(s);
	}

	bool is_source_extension(const string& s)
	{
		return Source_Extensions.count(s);
	}

	bool is_exist(const string& path)
	{
		if (is_more_point(path)) return false;			// 不处理 ... 连续三个以上的情形
		FS::path f{ path };
		return FS::exists(f);
	}

    bool is_exist_file(const string& path)
    {
        if (is_exist_dir(path)) return false;
        return is_exist(path);
    }

	bool is_exist_dir(const string& path)
	{
		FS::path f{ path };
		return FS::is_directory(f);
	}

	bool is_same_parent_dir(const string& file1, const string& file2)
	{
		FS::path dir1{ file1 }, dir2{ file2 };
		return (dir1.parent_path() == dir2.parent_path());
	}

	string file_name(const string& s, bool have_extension)
	{
		FS::path f(s);
		string name = f.filename().string();
		string extension = f.extension().string();
		if (!is_extension(extension)) return name;
		return (have_extension) ? name : name.substr(0, name.rfind(extension));
	}

	string parent_dir(const string& s)
	{
		FS::path f(s);
		return f.parent_path().string();
	}

	string replace_extension(const string& path, const string& extension)
	{
		if (!is_extension(extension))
			throw string("path_deal::replace_extension:" + mark_string(extension) + "不是程序所能处理的扩展名");

		FS::path dir{ path };
		string parent_dir = dir.parent_path().string();
		if (parent_dir != "") parent_dir += "\\";
		string suffix = dir.extension().string();
		if (!is_extension(suffix)) return path + extension;

		string file_name = dir.filename().string();
		file_name = file_name.substr(0, file_name.rfind(suffix));
		return parent_dir + file_name + extension;
	}

	vector<string> find_same_extension_files(const string& dir_path_str, const string& extension)
	{
		FS::path dir_path{ dir_path_str };
		if (!FS::is_directory(dir_path)) 
            throw string("path_deal::find_same_extension_files:" + mark_string(dir_path.string()) + "不是目录或者不存在");
		vector<string> res;
		for (const auto& entry : FS::directory_iterator(dir_path)) {
			if (entry.path().extension() == extension) res.emplace_back(entry.path().filename().string());
		}
		return res;
	}

	string current_dir()
	{
		return FS::current_path().string();
	}

	string normalize(const string& sourse, const string& target)
	{
		FS::path p(sourse);
		p.append(target);
		string res = p.lexically_normal().string();
		if (!is_exist_dir(res))
			if (is_exist(res + IDEFAULT_EXTENSION)) return res + IDEFAULT_EXTENSION;
		return res;
	}

	string extension(const string& s)
	{
		FS::path f(s);
		string extension = f.extension().string();
		return (is_extension(extension)) ? extension : "";
	}

	bool remove_file(const string& s)
	{
		FS::path f{ s };
		if (!FS::exists(s)) throw string("path_deal::remove_file:" + mark_string(s) + "not exist");
		return FS::remove(f);
	}

	bool create_dirs(const string& s)
	{
		FS::path f{ s };
		if (FS::exists(s)) throw string("path_deal::remove_file:" + mark_string(s) + "already exist");
		return FS::create_directories(s);
	}
}