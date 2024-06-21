#include "Type.h"
#include "Excep_dev.h"
#include "output.h"

using namespace std;
using namespace output;

namespace htf {
    namespace parse {
        Type::Type()
            :_type{ }
        {
            
        }
        
        Type::Type(const lex::Lexer& l)
            :_type{ l.val }
        {
            if (l.kind != lex::Lexer_kind::TYPE_KIND) 
                throw excep::Excep_dev{"parse::Type::Type", _LINE + "lexer.kind isn't type_kind"};
        }

        string Type::str() const
        {
            return _type;
        }

        bool Type::empty() const
        {
            return _type.empty();
        }
    }
}