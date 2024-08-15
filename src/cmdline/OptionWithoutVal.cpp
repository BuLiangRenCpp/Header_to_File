#include "OptionWithoutVal.h"

namespace htf
{
namespace cmdline
{

OptionWithoutVal::OptionWithoutVal(const std::string& name, char short_name,
                                   const std::string& description, bool is_must)
    : OptionBase{name, short_name, description, is_must}
{}

}   // namespace cmdline
}   // namespace htf