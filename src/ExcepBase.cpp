#include "ExcepBase.h"
#include "assertions.h"

namespace htf
{

// public
ExcepBase::ExcepBase(const std::string& where, const what_type& what) : _where{where}, _what{what}
{
    HTF_DEV_ASSERT_MESSAGE(!what.empty(), "Excep_base: what must be not empty");
}

}   // namespace htf
