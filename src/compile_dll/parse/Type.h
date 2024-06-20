#pragma once
#include "Lexer.h"

namespace htf {
	namespace parse {
		class Type {
		public:
			Type();	  
			Type(const lex::Lexer&);

			std::string str() const;
			bool empty() const;
		private:
			std::string _type;
		};
	}
}