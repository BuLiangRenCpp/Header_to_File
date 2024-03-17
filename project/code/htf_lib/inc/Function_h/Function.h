#pragma once

#include "Fun_name.h"
#include "Fun_arg.h"


// 格式满足：Type + [Fun_name] + "(" + [Args] + ")" + [const] + ";" 的函数，可以为全为空
// 如：一般函数、重载运算符函数、成员常量函数、特殊函数(构造函数、析构函数等)
class Function {
public:
    Function();
    Function(const Type&, const Fun_name&, const std::vector<Fun_arg>&, bool have_const = false, bool is_construct = false);

    Type type() const;      // 用于 重载输出流
    // 返回定义语句  -->  如：int add(int) { }
    // count 控制缩进，"\t"的个数
    std::string str(unsigned int count = 0) const;
    // 类的成员函数，参数为类名
    // count 控制缩进，"\t"的个数
    std::string str_class(const std::string& class_name, unsigned int count = 0) const;
    bool empty() const;
    // 是否是特殊函数
    bool is_sepcial() const;
    // 格式化输出，按缩进输出
    void print(std::ostream& os, unsigned int count = 1) const;
    
private:
    Type _type;
    Fun_name _name;
    std::vector<Fun_arg> _pars;
    bool _have_const;       // 参数列表后是否有 const
    bool _is_construct;     // 是否是析构函数
};


// ------------------------ 重载 -----------------------------

// 如果读取到函数定义语句，则忽略，返回空函数
std::istream& operator>>(std::istream& is, Function& f);

std::ostream& operator<<(std::ostream& os, const Function& f);