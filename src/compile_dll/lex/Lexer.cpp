#include "Lexer.h"
#include "output.h"
#include "usage_lex.h"
#include "Excep_dev.h"

using namespace std;
using namespace output;

namespace htf {
    using namespace excep;
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
			// 虽然目前的 lexer_kind 包含 token_kind，但是以防万一今后有变动 
			if (!usage::is_spe_ch(kind) && !is_kind(kind))
				throw Excep_dev{"Lexer::Lexer(const Token&)", _LINE + mark(kind) + "isn't lexer's kind"};
		}

		Lexer::Lexer(char c, const stream::Token& t)
			:kind{ c }, val{ t.val }
		{
			if (!usage::is_spe_ch(c) && !is_kind(c))
				throw Excep_dev{"Lexer::Lexer(const Token&)", _LINE + mark(kind) + "isn't lexer's kind"};
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