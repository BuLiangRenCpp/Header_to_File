#include "ins_judge.h" 

using namespace std;
using namespace ins_const;
using namespace prompt;


static bool is_ins_arg(const string& s, char c)
{
    if (!ins_judge::is_ins(s)) 
        throw string("ins_judge.cpp::is_ins_arg:" + mark_string(s) + "不是指令");
    return ins::Ins_to_Args.at(s).second.count(c);
}

static bool is_htf_args(const vector<char>& args)
{
    bool is_i = false, is_o = false, is_f = false, is_darg = false;
    for (char c : args) {
        if (!is_ins_arg(ins::HTF_INS, c)) 
            throw string("ins_judge.cpp::is_htf_args:" + mark_char(c) + "不是" + 
                        mark_string(ins::HTF_INS) + "的参数");
        if (c == arg::NULL_ARG) is_darg = true;
        else if (c == arg::INPUT_ARG) {
            if (is_i)
                throw string("ins_judge.cpp::is_htf_args:" + mark_string(ins::HTF_INS) + "指令的参数" +
                            mark_char(c) + "只能出现一次");
            is_i = true;
        }
        else if (c == arg::OUTPUT_ARG) {
            if (is_o) 
                throw string("ins_judge.cpp::is_htf_args:" + mark_string(ins::HTF_INS) + "指令的参数" +
                            mark_char(c) + "只能出现一次");
            is_o = true;
        }
        else if (c == arg::FORCE_ARG) {
            if (is_f)
                throw string("ins_judge.cpp::is_htf_args:" + mark_string(ins::HTF_INS) + "指令的参数" +
                            mark_char(c) + "只能出现一次");
            is_f = true;
        }
    }
    if (is_i == false && is_darg == false)
        throw string("ins_judge.cpp::is_htf_args:" + mark_string(ins::HTF_INS) + "缺少参数" + 
                    mark_char(arg::INPUT_ARG));
    return true;
}


namespace ins_judge {
    bool is_ins(const string& s)
    {
        return ins::Inss.count(s);
    }
    
    bool is_single_ins(const string& s) 
    {
        return ins::Single_Ins.count(s);
    }

    bool is_arg(char c) 
    {
        return arg::Args.count(c);
    }

    bool is_harg(char c) 
    {
        return arg::Hargs.count(c);
    }

    bool is_narg(char c) 
    {
        return arg::Nargs.count(c);
    }

    bool is_ins_args_throw(const string& s, const vector<char>& t) 
    {
        if (!is_ins(s)) 
            throw string("ins_judge.cpp::is_ins_arg:" + mark_string(s) + "不是指令");
        if (is_single_ins(s)) return t.empty();
        if (s == ins::HTF_INS) return is_htf_args(t);
        return true;
    }

    int arg_count_max(const string& s) 
    {
        if (!is_ins(s)) 
            throw string("Ins_judge::arg_count_max:" + mark_string(s) + "不是指令");
        return ins::Ins_to_Args.at(s).first; 
    }
}


