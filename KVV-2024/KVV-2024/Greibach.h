#pragma once
#include "Error.h"

typedef short GRBALPHABET;

#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
//не терменал и терменал
namespace GRB {
	struct Rule {
		GRBALPHABET nn;
		int iderror;
		short size;

		struct Chain {
			short size;
			GRBALPHABET* nt;

			Chain();
			Chain(short size, GRBALPHABET s, ...);

			std::string getCChain();
			static GRBALPHABET T(char t) { return GRBALPHABET(t); }
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }
			static bool isT(GRBALPHABET s) { return s > 0; }
			static bool isN(GRBALPHABET s) { return !isT(s); }
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); }
		} *chains;

		Rule();
		Rule(GRBALPHABET nn, int iderror, short size, Chain c, ...);

		std::string getCRule(short nchain);
		short getNextChain(GRBALPHABET t, Rule::Chain& chain, short n);
	};

	struct Greibach {
		short size;
		GRBALPHABET startN;
		GRBALPHABET stbottomT;
		Rule* rules;

		Greibach();
		Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short size, Rule r, ...);

		short getRule(GRBALPHABET nn, Rule& rule);
		Rule getRule(short n);
	};
	const Greibach& getGreibach();
}