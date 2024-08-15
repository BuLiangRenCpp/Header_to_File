#include "OptionBase.h"
#include "ExcepCmdline.h"
#include "exceptdef.h"

namespace htf
{
namespace cmdline
{

OptionBase::OptionBase(const std::string name, char short_name, const std::string& description,
                       bool is_must)
    : _name{name}, _sname{short_name}, _is_must{is_must}, _desc{description}
{
    THROW_EXCEP_CMDLINE_IF(name.empty(), "option name cannot empty");
    THROW_EXCEP_CMDLINE_IF(short_name == '-', mark('-') << "cannot to be a short option name");
}

}   // namespace cmdline
}   // namespace htf