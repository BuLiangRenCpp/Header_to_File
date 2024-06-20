#include "output.h"
#include "args_const.h"
#include "args_judge.h" 
#include "Excep_arg.h"
#include "Excep_dev.h"

using namespace std;
using namespace output;
using namespace htf::argument;
using namespace htf::exception;

static bool is_htf_args(const vector<char>& args)
{
    bool is_i = false, is_o = false, is_I = false, is_f = false;
    for (char c : args) {
        if (!args_judge::is_arg(c)) 
            throw Excep_arg("args_judge.cpp::is_htf_args", mark_char(c) + "isn't a legal argument");

        if (c == args_const::INPUT_ARG) 
            is_i = true;
        else if (c == args_const::OUTPUT_ARG) {
            if (is_o) 
                throw Excep_arg("args_judge.cpp::is_htf_args", "argument" +
                            mark_char(c) + "only apppear once");
            is_o = true;
        }
    }
    if (is_i == false)
        throw Excep_arg("args_judge.cpp::is_htf_args",  "lack of" + 
                    mark_char(args_const::INPUT_ARG));
    return true;
}


namespace htf {
    namespace argument{
        using namespace args_const;
        namespace args_judge {
            bool is_arg(char c) 
            {
                return Args.count(c);
            }

            bool is_harg(char c) 
            {
                return Hargs.count(c);
            }

            bool is_narg(char c) 
            {
                return Nargs.count(c);
            }

            bool is_htf_args_throw(const vector<char>& t) 
            {
                return is_htf_args(t);
            }

            int arg_count_max(char c)
            {
                if (!is_arg(c))
                    throw Excep_dev{"args_judge::arg_count_max", _LINE + mark_char(c) + "不是参数"};
                return (is_harg(c)) ? Hargs_Cnt.at(c) : 0;
            }

            int htf_arg_count_max()
            {
                return MAX_ARG_COUNT;
            }
        }
    }
}


