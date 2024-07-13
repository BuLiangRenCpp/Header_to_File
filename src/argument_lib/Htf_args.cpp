#include "Htf_args.h"
#include "args_const.h"
#include "args_judge.h"
#include "Excep_arg.h"
#include "output.h"

using namespace std;
using namespace output;
using namespace htf::argument;
using namespace htf::argument::args_const;

static vector<char> to_chars(const vector<Arg>& t)
{
    vector<char> res;
    for (const auto& x : t) res.emplace_back(x.key().ch());
    return res;
}


static int _index(char c, const vector<Arg>& args)
{
    int i = args.size() - 1;
    for (; i >= 0; i--) {
        if (args[i].key().ch() == c) break;
    }
    return i;
}

// 将多个相同参数合并
static vector<Arg> _unite(vector<Arg> args)
{
    vector<Arg> temp;
    for (auto t : args) {
        auto index = _index(t.key().ch(), temp);
        if (index == -1) temp.emplace_back(t);
        else {
            temp[index].unite(t);
        }
    }
    return temp;
}

// -f -i -o -I
static void sort_args(vector<Arg>& args)
{
    args = _unite(args);
    vector<Arg> temp;
    int index = _index(FORCE_ARG, args);
    if (index != -1) temp.emplace_back(args[index]);

    index = _index(INPUT_ARG, args);
    if (index != -1) temp.emplace_back(args[index]);

    index = _index(OUTPUT_ARG, args);
    if (index != -1) temp.emplace_back(args[index]);

    index = _index(INCLUDE_ARG, args);
    if (index != -1) temp.emplace_back(args[index]);

    args = temp;
}

static void legal_htf_ins(vector<Arg>& args) 
{
    sort_args(args);
}


namespace htf {
    using namespace excep;
    namespace argument {
        using namespace args_judge;

        Htf_args::Htf_args()
        {
            Arg_key key; Arg_con con;
            Arg arg(key, con);
            _args.emplace_back(arg);
        }

        Htf_args::Htf_args(const vector<Arg>& args)
            :_args{ args }
        {
            if (args.empty()) throw Excep_arg("Htf_args::Htf_args", "no input files");
            if (is_htf_args_throw(to_chars(args))) 
                _legalize();
        }

        vector<Arg> Htf_args::args() const
        {
            return _args;
        }

        void Htf_args::_legalize()
        {   
            legal_htf_ins(_args);
        }

        istream& operator>>(istream& is, Htf_args& t)
        {
            Arg a;
            vector<Arg> args;
            while (true) {
                is >> a;
                if (a.empty()) break;
                args.emplace_back(a);
            }
            if (args.empty() && !is.eof()) throw Excep_arg("Htf_args.cpp::operator>>", "error argument format");
            t = Htf_args{ args };
            return is;
        }
    }
}

