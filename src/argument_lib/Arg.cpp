#include "Arg.h"
#include "Excep_arg.h"
#include "Excep_dev.h"
#include "args_const.h"
#include "args_judge.h"
#include "output.h"

using namespace std;
using namespace output;


namespace htf
{
using namespace excep;
namespace argument
{
using namespace args_const;

Arg::Arg()
    : _key{}
    , _con{}
{
}

Arg::Arg(const Arg_key& k, const Arg_con& c)
    : _key{k}
    , _con{c}
{
    // ! 空参数
    if (k.empty()) {
        if (c.empty()) return;
        _key = Arg_key{args_const::INPUT_ARG};
    }
    if (args_judge::is_harg(_key.ch()) && _con.empty()) {
        if (k.ch() == args_const::INCLUDE_ARG || k.ch() == args_const::OUTPUT_ARG)
            throw Excep_arg("Arg::Arg", "missing directories after" + mark("-" + string{k.ch()}));
        else
            throw Excep_arg("Arg::Arg", "missing filename after" + mark("-" + string{k.ch()}));
    }
    if (_con.con().size() > args_judge::arg_count_max(_key.ch()))
        throw Excep_arg("Arg::Arg", mark("-" + string{k.ch()}) + "have excessive arguments: " + mark(_con.str()));
}

Arg_key Arg::key() const
{
    return _key;
}

Arg_con Arg::con() const
{
    return _con;
}

bool Arg::unite(const Arg& a)
{
    if (a.key().ch() != _key.ch())
        throw Excep_dev{"Arg::unite", _LINE + "a.key != _key"};
    return _con.insert(a.con());
}

bool Arg::empty() const
{
    return _key.empty() && _con.empty();
}

bool Arg::is_darg() const
{
    return _key.empty() && !_con.empty();
}

istream& operator>>(istream& is, Arg& t)
{
    Arg_key k;
    Arg_con c;

    is >> k;
    if (args_judge::is_narg(k.ch())) {
        t = Arg{k};
        return is;
    }
    is >> c;
    t = Arg{k, c};
    return is;
}
}   // namespace argument
}   // namespace htf
