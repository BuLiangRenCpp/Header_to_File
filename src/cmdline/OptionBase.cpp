#include "OptionBase.h"

#include "exceptdef.h"

namespace htf
{
namespace cmdline
{

OptionBase::OptionBase(const std::string name, char short_name, const std::string& description,
                       bool is_must)
    : _name{name}, _sname{short_name}, _is_must{is_must}, _desc{description}
{
    THROW_LOGIC_ERROR_IF(name.empty(),
                         "OptionBase::OptionBase(): "
                             << "empty string don't be a name");
    THROW_LOGIC_ERROR_IF(short_name == '-',
                         "OptionBase::OptionBase():" << mark(short_name)
                                                     << "don't be a short_name");
}

}   // namespace cmdline
}   // namespace htf