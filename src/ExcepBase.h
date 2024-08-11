#pragma once

// * 此异常用于提示用户
// * dev: 应采用 exceptdef.h (<stdexcept>) or assertions.h

#include <sstream>

#include "output.h"   // 一般都会用到 mark()

namespace htf
{

class ExcepBase
{
public:
    using what_type = std::string;   // 可能往后会扩展: 采用新的数据结构存储 _what，使得能够格式化

public:
    ExcepBase(const std::string& where, const what_type& what);

    virtual std::string where() const { return _where; }
    std::string         what() const { return _what; }
    virtual std::string str() const
    {
        std::ostringstream oss;
#ifdef USER
        oss << _what;
        return oss.str();
#else
        oss << _where << ": " << _what;
        return oss.str();
#endif
    }

protected:
    std::string _where;   // error 发生的地方：such as __FILE__:__LINE__:函数名 ->
                          // '一般是给 dev 看的'
    what_type _what;      // error content，不能为空
};

}   // namespace htf