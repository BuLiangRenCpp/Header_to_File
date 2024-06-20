#include "Lexer.h"
#include "output.h"
#include "usage_lex.h"
#include "Excep_dev.h"

using namespace std;
using namespace output;

namespace htf {
    using namespace exception;
    namespace lex {
        using namespace Lexer_kind;
        using namespace usage;
        
        Lexer::Lexer()
            :kind{ NULL_KIND }, val{ }
        {
            
        }

		Lexer::Lexer(const stream::Token& t)
			:kind{ t.kind }, val{ t.val }
		{

		}

		Lexer::Lexer(char c, const stream::Token& t)
			:kind{ c }, val{ t.val }
		{
			if (!is_kind(kind)) 
				cout << " xxx " << endl;
		}

		bool Lexer::empty() const
		{
			return kind == NULL_KIND && val.empty();
		}

		bool operator==(const Lexer& t1, const Lexer& t2)
		{
			return (t1.kind == t2.kind && t1.val == t2.val);
		}

		bool operator!=(const Lexer& t1, const Lexer& t2)
		{
			return !(t1 == t2);
		}
    }
}