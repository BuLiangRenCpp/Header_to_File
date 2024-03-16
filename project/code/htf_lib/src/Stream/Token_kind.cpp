#include "Token_kind.h" 

using namespace usage;

namespace Token_kind{
    
    char ret_kind(const string& s) 
    {
        // 注意优先级
        if (is_class_key(s)) return CLASS_KEY_KIND;
        if (is_type_key(s)) return TYPE_KIND;
        if (is_identifier(s)) return IDENTIFIER_KIND;
        return NULL_KIND;
    }

}

