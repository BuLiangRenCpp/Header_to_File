#pragma once

// 无参数的 option
// 在命令行中：(由 Parse 设置)
// - 写了: true
// - 未写: false

#include "OptionBase.h"
#include "exceptdef.h"

namespace htf
{
namespace cmdline
{

class OptionWithoutVal : public OptionBase
{
public:
    OptionWithoutVal(const std::string& name, char short_name = no_short_name,
                     const std::string& description = "", bool is_must = false);

    ~OptionWithoutVal() = default;
};

}   // namespace cmdline
}   // namespace htf