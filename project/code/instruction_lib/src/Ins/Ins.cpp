#include "Ins.h" 

using namespace std;
using namespace ins_judge;
using namespace prompt;
using namespace ins_const;

static vector<char> to_chars(const vector<Ins_arg>& args)
{
    vector<char> res;
    for (const auto& t : args) res.emplace_back(t.key().ch());
    return res;
}


// --------------------------------------- Ins ------------------------------------------

// -------------------- public -----------------------

Ins::Ins()
    :_key{}, _args{}
{
    
}

Ins::Ins(const Ins_key& k, const vector<Ins_arg>& args) 
    :_key{ k }, _args{ args }
{
	if (k.empty()) {
        if (args.empty()) return;
        throw string("Ins::Ins: 缺少指令");
    }
	// 1. 单指令
	if (is_single_ins(k.str())) {
		if (!args.empty()) throw string("Ins::Ins:" + mark_string(k.str()) + "是单指令，无法解析后面的内容");
		return;
	}
	// 2. 多指令
	if (args.empty()) throw string("Ins::Ins:" + mark_string(k.str()) + "指令后缺少内容");
	if (is_ins_args_throw(k.str(), to_chars(args))) _legalize();
}

Ins_key Ins::key() const
{
    return _key;
}

vector<Ins_arg> Ins::args() const
{
    return _args;
}

bool Ins::empty() const
{
    return _key.empty();        // 初始化保证了 不为空指令集的 _key 一定不为空
}

// ----------------------- private -------------------------------

// 返回一个vector，对应 args 下标的位置为是否 默认参数
static vector<bool> is_darg(const vector<Ins_arg>& args)
{
    auto size = args.size();
    vector<bool> res(size, false);
    for (int i = 0; i < size; i++) {
        if (args[i].is_darg()) res[i] = true;
    }
    return res;
}

static bool have_true(const vector<bool>& t)
{
    for (auto x : t)
        if (x == true) return true;
    return false;
}

static void update_io(bool& is_i, bool& is_o, const vector<Ins_arg>& args)
{
    for (auto it = args.begin(); it != args.end(); it++) {
        if (it->key().ch() == arg::INPUT_ARG) is_i = true;
        else if (it->key().ch() == arg::OUTPUT_ARG) is_o = true;
    }
}

// 返回无参参数的下标
static int have_nagr(const vector<Ins_arg>& args)
{
    for (int i = 0; i < args.size(); i++) {
        if (args[i].key().is_narg()) return i;
    }
    return -1;
}

// 对于 size == 2 的 保证 -f -i -o 的顺序
static void sort_htf_ins_2(vector<Ins_arg>& args)
{
    // false: -i '-f'    '-o' -i     true: -f -i    -i -o
    if (args[1].key().is_narg() || args[0].key().ch() == arg::OUTPUT_ARG) 
        swap(args[0], args[1]);
}

// 可以将 size == 2 的情况单独提出来，因为 size == 3 只需要将 -f 删掉即可
// have_force 是否存在 -f
static void legal_htf_ins_2(vector<Ins_arg>& args)
{
    sort_htf_ins_2(args);
    bool is_i = false, is_o = false;
    update_io(is_i, is_o, args);
    if (is_i == false && is_o == false) {  
        if (have_nagr(args) != -1) {       // 1. -f NULL 
            args[1].change_darg(Ins_arg_key{arg::INPUT_ARG});
        }
        else {      // 2. NULL NULL
            args[0].change_darg(Ins_arg_key{arg::INPUT_ARG});
            args[1].change_darg(Ins_arg_key{arg::OUTPUT_ARG});
        }
    }
    else if (is_i == false) {        // 3. NULL -o
        args[0].change_darg(Ins_arg_key{arg::INPUT_ARG});
    }
    else if (is_o == false) { 
        if (args[1].is_darg()) args[1].change_darg(Ins_arg_key{arg::OUTPUT_ARG});      // 4. -i NULL
        // 5. -f -i
    }
}

static void legal_htf_ins_3(vector<Ins_arg>& args)
{
    vector<Ins_arg> arg2;
    for (auto it = args.begin(); it != args.end(); it++) {
        if (it->key().is_narg()) continue;
        arg2.emplace_back(*it);
    }
    legal_htf_ins_2(arg2);
    args.clear();
    args.emplace_back(Ins_arg{ins_const::arg::FORCE_ARG});          // 确保第一个是 -f 
    args.emplace_back(arg2[0]);
    args.emplace_back(arg2[1]);
}

static void legal_htf_ins(vector<Ins_arg>& args) 
{
    auto size = args.size();
    vector<bool> temp = is_darg(args);
    if (!have_true(temp)) return;
    if (size == 1) args[0].change_darg(Ins_arg_key{arg::INPUT_ARG});
    if (size == 2) legal_htf_ins_2(args);
    if (size == 3) legal_htf_ins_3(args);
}

void Ins::_legalize()
{   
    legal_htf_ins(_args);
}


istream& operator>>(istream& is, Ins& t) 
{
    Ins_key key;
    vector<Ins_arg> args;

    is >> key;
    char c;
    my_std::getchar(is, c);
    if (is.eof()) {     // 如果是空行，不执行操作，因此返回 空
        t = Ins{key};
        return is;
    }
    is.putback(c);
    if (is_single_ins(key.str()))
        throw string("Ins.cpp::operator>>: 指令" + mark_string(key.str()) + "是单指令，无法解析后面的内容");
    
    int size_max = arg_count_max(key.str());
    while (true) {     
        Ins_arg arg;
        is >> arg;
        if (arg.empty()) break;
        args.emplace_back(arg);
        if (args.size() > size_max) 
            throw string("Ins.cpp::operator>>:" + mark_string(key.str()) + "参数个数超过限制，限制为" + to_string(size_max) + "个");
    }
    t = Ins{key, args};
    return is;
}


