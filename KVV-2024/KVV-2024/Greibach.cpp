#include "pch.h"
#include "Greibach.h"
#define GRB_ERROR_SERIES 600

namespace GRB {
#pragma region Rules
	const Greibach greibach(NS('S'), TS('$'),
		9,
		Rule(NS('S'), GRB_ERROR_SERIES + 0,//начало кода s-начальный символ(следущее функцию) N-выпажение r-ретурн E-литералы t-тип f-функций
			7,
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(7, TS('m'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,//выражения d-declare(обявление) i-индентификатор l-литерал  Q-16с.с p-вывод
			24,
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), TS('l'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), TS('l'), TS('Q'), TS(';')),


			Rule::Chain(6, TS('d'), TS('t'), TS('i'),TS('='),NS('E'), TS(';')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'),TS('='),NS('E'), TS(';'), NS('N')),
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), TS('l'), TS(';'), NS('N')),

			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),

			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			
		
			Rule::Chain(3, TS('p'), TS('i'), TS(';')),
			
			Rule::Chain(10, TS('u'), TS('('), NS('V'), NS('C'), NS('V'),TS(')'), TS('['), NS('N'),TS(']'), TS(';') ),
			Rule::Chain(14, TS('u'), TS('('), NS('V'), NS('C'), NS('V'),TS(')'), TS('['), NS('N'),TS(']'), TS('E'), TS('['), NS('N'), TS(']'), TS(';') ),
			Rule::Chain(15, TS('u'), TS('('), NS('V'), NS('C'), NS('V'),TS(')'), TS('['), NS('N'),TS(']'), TS('E'), TS('['), NS('N'), TS(']'), TS(';'), NS('N') ),
			Rule::Chain(11, TS('u'), TS('('), NS('V'), NS('C'), NS('V'),TS(')'), TS('['), NS('N'),TS(']'), TS(';'), NS('N') ),
			Rule::Chain(11, TS('W'), TS('('), NS('V'), NS('C'), NS('V'),TS(')'), TS('['), NS('N'),TS(']'), TS(';'), NS('N') ),
			Rule::Chain(10, TS('W'), TS('('),NS('V'), NS('С'), NS('V'),TS(')'), TS('['), NS('N'),TS(']'), TS(';') ),
			Rule::Chain(3, TS('p'), TS('l'), TS(';')),
			Rule::Chain(6, TS('p'), TS('i'), TS('('), NS('W'), TS(')'), TS(';'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,// литералы u-названеи функции  V-опернд истинности C- !><L P-степень R-рандом M-операции
			24,
			Rule::Chain(1, TS('i')),
			Rule::Chain(4, TS('L'),TS('('), TS('l'), TS(')')),
			Rule::Chain(4, TS('L'),TS('('), TS('i'), TS(')')),
			Rule::Chain(5, TS('L'),TS('('), TS('i'), TS(')'), NS('M')),
		
			Rule::Chain(6, TS('P'),TS('('), NS('V'), TS(','), NS('V'), TS(')')),
			Rule::Chain(7, TS('P'),TS('('), NS('V'), TS(','), NS('V'), TS(')'), NS('M')),
			
			Rule::Chain(4, TS('R'),TS('('), TS('l'), TS(')')),
			Rule::Chain(4, TS('R'),TS('('), TS('i'), TS(')')),

			Rule::Chain(5, TS('R'),TS('('), TS('l'), TS(')'), NS('M')),
			Rule::Chain(5, TS('R'),TS('('), TS('i'), TS(')'), NS('M')),
			Rule::Chain(4, TS('N'),TS('('), TS('l'), TS(')')),
			Rule::Chain(5, TS('N'),TS('('), TS('l'), TS(')'), NS('M')),
			Rule::Chain(6, TS('C'),TS('('), TS('l'), TS(','), TS('l'), TS(')')),
			Rule::Chain(5, TS('L'),TS('('), TS('l'), TS(')'), NS('M')),
			
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('l'),NS('M')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(3,TS('i'),TS('('),TS(')')),
			Rule::Chain(4,TS('i'),TS('('),TS(')'),NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 3,//операции
			20,
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(4, TS('+'), TS('('), NS('E'), TS(')')),
			Rule::Chain(5, TS('+'), TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('+'), NS('E'), NS('M')),
			
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(4, TS('-'), TS('('), NS('E'), TS(')')),
			Rule::Chain(5, TS('-'), TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(4, TS('*'), TS('('), NS('E'), TS(')')),
			
			Rule::Chain(5, TS('*'), TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('*'), NS('E'), NS('M')),
			Rule::Chain(2, TS('/'), NS('E')),
			Rule::Chain(4, TS('/'), TS('('), NS('E'), TS(')')),
			Rule::Chain(5, TS('/'), TS('('), NS('E'), TS(')'), NS('M')),
			
			Rule::Chain(3, TS('/'), NS('E'), NS('M')),
			Rule::Chain(2, TS('%'), NS('E')),
			Rule::Chain(4, TS('%'), TS('('), NS('E'), TS(')')),
			Rule::Chain(5, TS('%'), TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('%'), NS('E'), NS('M'))
			
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4,//параметры функции
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))

		),
		Rule(NS('W'), GRB_ERROR_SERIES + 5,//аргументы
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
		

			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		
		),
			Rule(NS('C'), GRB_ERROR_SERIES + 6,//операторы истинности
					6,
		
				Rule::Chain(1, TS('>')),

				Rule::Chain(1, TS('<')),
			
				Rule::Chain(1, TS('!')),

			
				Rule::Chain(1, TS('~')),
				
				Rule::Chain(1, TS('$')),
				
				Rule::Chain(1, TS('`'))

			),
			Rule(NS('V'), GRB_ERROR_SERIES + 7,//операнды истинности
				2,
				Rule::Chain(1, TS('i')),
				Rule::Chain(1, TS('l'))
			), 
			Rule(NS('Q'), GRB_ERROR_SERIES + 8,
				2,
				Rule::Chain(1, TS('i')),
				Rule::Chain(2, TS('i'), TS('Q'))
			)
	);
#pragma endregion

	Rule::Chain::Chain()
		: size(0), nt(nullptr)
	{	}

	Rule::Chain::Chain(short size, GRBALPHABET s, ...)
		: size(size)
	{
		nt = DBG_NEW GRBALPHABET[size];	

		int* p = (int*)&s;// Создаём указатель p, который указывает на адрес первого аргумента `s` м и интерпретируем его как указатель на int. 

		for (short i = 0; i < size; ++i) {
			nt[i] = (GRBALPHABET)p[i];// Приводим каждый элемент из p[i] к типу GRBALPHABET и записываем в nt[i].
		}
	}

	Rule::Rule()
		:nn(0x00), iderror(0), size(0), chains(nullptr)
	{	}

	Rule::Rule(GRBALPHABET nn, int iderror, short size, Chain c, ...)
		: nn(nn), iderror(iderror), size(size)
	{
		chains = DBG_NEW Chain[size];
		Chain* p = &c;
		for (int i = 0; i < size; ++i) {
			chains[i] = p[i];
		}
	}

	Greibach::Greibach()
		: size(0), startN(0), stbottomT(0), rules(nullptr)
	{	}

	Greibach::Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short size, Rule r, ...)
		: startN(startN), stbottomT(stbottomT), size(size)
	{
		rules = DBG_NEW Rule[size];
		const Rule* p = &r;
		for (int i = 0; i < size; ++i) {
			rules[i] = p[i];
		}
	}

	std::string Rule::Chain::getCChain() {
		std::string chain;// Создаём пустую строку для хранения результата.

		for (int i = 0; i < size; ++i) {
			chain.push_back(Chain::alphabet_to_char(nt[i]));
			// Преобразуем i-й элемент массива `nt` из типа GRBALPHABET в символ с помощью метода `alphabet_to_char`.
		// Добавляем этот символ в строку `chain`.
		}

		return chain;
	}

	std::string Rule::getCRule(short nchain) {
		std::string ruleChain(1, Chain::alphabet_to_char(nn));
		// Создаём строку с первым символом, представляющим нетерминал nn.
		ruleChain += "->";
		ruleChain += chains[nchain].getCChain();
		// Добавляем строковое представление выбранной цепочки `nchain` из массива chains.
	// Метод chains[nchain].getCChain() возвращает строку для цепочки.

		return ruleChain;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& chain, short n) {
		short output = -1; //цепочка пока не найдена.

		while (n < size && chains[n].nt[0] != t)
			++n;
		
   // Проверяем, совпадает ли первый символ `nt[0]` текущей цепочки с символом `t`.
   

		output = (n < size) ? n : -1;// Если такой цепочки нет, возвращаем `-1`.
		if (output >= 0) {
			chain = chains[output]; 
		}

		return output;
	}

	short Greibach::getRule(GRBALPHABET nn, Rule& rule) {
		short output = -1;
		short i = 0;


		while (i < size && rules[i].nn != nn)
			i++;
		
	// Если текущий элемент `rules[i].nn` не совпадает с указанным символом `nn`,
	
	

		if (i < size) {
			
			output = i;
			rule = rules[output]; // Копируем найденное правило в переданный аргумент `rule`.
		}

		return output;
	}

	Rule Greibach::getRule(short n) {
		if (n > size) {
			throw ERROR_THROW(609);
		}

		return rules[n];
	}

	const Greibach& getGreibach() {
		return greibach;
	}
}