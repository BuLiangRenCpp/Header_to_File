#include "Resolve.h" 

using namespace std;
using namespace ins_const;
using namespace prompt;
using namespace path_deal;


// --------------------------------------- Resolve ------------------------------------------

// -------------------- public -----------------------

Resolve::Resolve() 
    :_is_run{ true }
{

}

void Resolve::resolve(const Ins& ins) 
{
    if (ins.empty()) return;
    string k = ins.key().str();
    if (k == ins::RUN_END_INS) _run_end_ins();
    else if (k == ins::LIST_DEFAULT_INS) _list_default_ins();
    else _htf_ins(ins.args());
}

bool Resolve::is_run() const
{
    return _is_run;
}


// -------------------- private -----------------------

void Resolve::_run_end_ins() 
{
    _is_run = false;
}

void Resolve::_list_default_ins() 
{
    print_result(current_dir());
}

void Resolve::_htf_ins(const vector<Ins_arg>& args) 
{
    string header_path, target_path;
    bool is_force = false;
    if (args[0].key().ch() == arg::FORCE_ARG) is_force = true;

    auto size = args.size();
    if (size == 1) header_path = args[0].con().str();       // -i
    else if (size == 2) {
        if (is_force) {      // -f -i
            header_path = args[1].con().str();
        } else {        // -i -o
            header_path = args[0].con().str(); 
            target_path = args[1].con().str();
        }
    }
    else {    // -f -i -o 
        header_path = args[1].con().str(); 
        target_path = args[2].con().str();
    }
    // ***************************************************************
    if (!path_deal::is_exist(header_path) && !path_deal::is_exist(header_path + path_deal::IDEFAULT_EXTENSION))
        throw string("Resolve::_htf_ins:" + mark_string(header_path) + "not exist");
    if (path_deal::is_exist_dir(header_path))       // 目录
        htf::header_to_files(header_path, target_path, is_force);
    else {      // 单文件
        htf::header_to_file(header_path, target_path, is_force);
    }
}



