#include "pch.h"
#include "MFST.h"
// Глобальная переменная для трассировки
int FST_TRACE_n = -1;

MFST::MfstState::MfstState()
	: tape_position(0), nrule(-1), nrulechain(-1)
{	}
// Конструктор `MfstState` с заданной позицией, стеком и индексом цепочки правил
MFST::MfstState::MfstState(short position, MFSTSTACK st, short nrulechain)
	: tape_position(position), nrule(-1), nrulechain(nrulechain), st(st)
{	}

MFST::MfstState::MfstState(short position, MFSTSTACK st, short nrule, short nrulechain)
	: tape_position(position), nrule(nrule), nrulechain(nrulechain), st(st)
{	}

MFST::Mfst::MfstDiagnosis::MfstDiagnosis()
	: tape_position(-1), rc_step(RC_STEP::SURPRISE), nrule(-1), nrule_chain(-1)
{	}

MFST::Mfst::MfstDiagnosis::MfstDiagnosis(short tape_position, RC_STEP rc_step, short nrule, short nrule_chain)
	: tape_position(tape_position), rc_step(rc_step), nrule(nrule), nrule_chain(nrule_chain)
{	}

MFST::Mfst::Mfst()
	: tape(0), tape_size(0), tape_position(0), lex(LT::Create(0)), nrule(-1), nrulechain(-1)
{	}

MFST::Mfst::Mfst(LT::LexTable lex, GRB::Greibach greibach, wchar_t outfile[])
	: lex(lex), greibach(greibach), tape_position(0), nrule(-1), nrulechain(-1), tape_size(lex.size)
{
	
	this->outfile.open(outfile, std::ios_base::app);
	
	tape = DBG_NEW short[tape_size];// Выделяем память под ленту синтаксического анализатора
	// Заполняем ленту, конвертируя лексемы в идентификаторы терминалов
	for (int k = 0; k < tape_size; ++k) {
		tape[k] = GRB::TS(lex.table[k].lexeme);
	}
	// Инициализируем стек начальными значениями
	st.push(greibach.stbottomT);// Дно стека
	st.push(greibach.startN);  // Стартовый нетерминал
}
// Возвращает содержимое стека как строку
std::string MFST::Mfst::getCSt() {
	std::string output = "";
	// Перебираем стек сверху вниз
	for (int k = (signed)st.size() - 1; k >= 0; --k) {
		short p = st.c[k];
		output.push_back(GRB::Rule::Chain::alphabet_to_char(p));// Конвертируем идентификатор символа в символ
	}

	return output;
}
// Возвращает содержимое ленты от позиции `pos` на `n` символов как строку
std::string MFST::Mfst::getCTape(short pos, short n) {
	std::string output = "";
	short i;
	short k = (pos + n < tape_size) ? pos + n : tape_size;// Определяем конечную позицию
	// Перебираем ленту
	for (i = pos; i < k; ++i) {
		output.push_back(GRB::Rule::Chain::alphabet_to_char(tape[i])); // Конвертируем символ
	}

	return output;
}
// Возвращает диагноз синтаксической ошибки как строку
std::string MFST::Mfst::getDiagnosis(short n) {
	std::string output = "";
	std::stringstream ss;
	int errid = 0;
	int lpos = -1;
	// Проверяем, есть ли диагностическая информация
	if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].tape_position) >= 0) {
		errid = greibach.getRule(diagnosis[n].nrule).iderror;// Получаем идентификатор ошибки
		Error::ERROR err = Error::geterror(errid); // Получаем описание ошибки
		ss << err.id << ": строка " << lex.table[lpos].sn << "," << err.message; // Форматируем вывод
		output = ss.str();
	}

	return output;
}
// Сохраняет текущее состояние синтаксического анализатора
bool MFST::Mfst::save_state() {
	storestate.push(MfstState(tape_position, st, nrule, nrulechain));// Сохраняем состояние в стек
	MFST_TRACE6("SAVESTATE:", storestate.size(), outfile);
	return true;
}
// Восстанавливает последнее сохранённое состояние
bool MFST::Mfst::restore_state() {
	bool output = false;
	MfstState state;
	// Проверяем, есть ли состояния в стеке
	if (output = (storestate.size() > 0)) {
		state = storestate.top();// Извлекаем состояние
		tape_position = state.tape_position;
		st = state.st;
		nrule = state.nrule;
		nrulechain = state.nrulechain;
		storestate.pop();
		MFST_TRACE5("RESTORESTATE",outfile)
			MFST_TRACE2(outfile)
	}

	return output;
}
// Добавляет цепочку правил в стек
bool MFST::Mfst::push_chain(GRB::Rule::Chain chain) {
	for (int k = chain.size - 1; k >= 0; k--) {
		st.push(chain.nt[k]);
	}

	return true;
}
// Выполняет один шаг синтаксического анализа
MFST::Mfst::RC_STEP MFST::Mfst::step() {
	RC_STEP output = Mfst::RC_STEP::SURPRISE;// Инициализируем результат текущего шага как "неожиданный" (SURPRISE)
	if (tape_position < tape_size) { // Проверяем, не достигли ли конца ленты
		if (GRB::Rule::Chain::isN(st.top())) { // Если верхний элемент стека — нетерминал
			GRB::Rule rule; // Объявляем переменную для хранения правила
			if ((nrule = greibach.getRule(st.top(), rule)) >= 0) { // Ищем правило для текущего нетерминала
				GRB::Rule::Chain chain; // Объявляем переменную для хранения цепочки правил
				if ((nrulechain = rule.getNextChain(tape[tape_position], chain, nrulechain + 1)) >= 0) {// Ищем подходящую цепочку правил для текущего символа ленты
					MFST_TRACE1(outfile)
						save_state();// Сохраняем текущее состояние (для отката в случае ошибки)
					st.pop();  // Убираем текущий нетерминал со стека
					push_chain(chain); // Добавляем цепочку правил в стек
					output = Mfst::RC_STEP::NS_OK; // Отмечаем успешное завершение обработки нетерминала
					MFST_TRACE2(outfile)
				}
				else { // Если подходящей цепочки нет
					MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", outfile)// Сохраняем диагноз ошибки
						savediagnosis(Mfst::RC_STEP::NS_NORULECHAIN);
					output = restore_state() ? Mfst::RC_STEP::NS_NORULECHAIN : Mfst::RC_STEP::NS_NORULE; // Если можем восстановить состояние, возвращаемся к предыдущему шагу	
				};
			}// Если правило для текущего нетерминала не найдено
			else output = Mfst::RC_STEP::NS_ERROR;// Устанавливаем ошибку для нетерминала
		}
		else if ((st.top() == tape[tape_position])) { // Если верхний элемент стека совпадает с текущим символом ленты (терминал)
			tape_position++;  // Продвигаем позицию на ленте
			st.pop(); // Убираем терминал со стека
			nrulechain = -1; // Сбрасываем цепочку правил
			output = Mfst::RC_STEP::TS_OK;// Отмечаем успешное сопоставление терминала
			MFST_TRACE3(outfile)
		}
		else {  // Если символ ленты не совпадает с верхним элементом стека
			MFST_TRACE4("TS_NOK/NS_NORULECHAIN", outfile)
				output = restore_state() // Пытаемся восстановить состояние
				? Mfst::RC_STEP::TS_NOK : Mfst::RC_STEP::NS_NORULECHAIN;
		}
	}
	else { // Если достигли конца ленты
		output = Mfst::RC_STEP::TAPE_END;
		MFST_TRACE4("TAPE_END", outfile)// Отмечаем конец ленты
	};
	return output;
}

bool MFST::Mfst::start(std::ostream& outputStream) {
	bool output = false;// Флаг для хранения результата синтаксического анализа (успешен или нет)
	RC_STEP rc_step = RC_STEP::SURPRISE; // Инициализируем переменную для хранения текущего состояния шага (неожиданное состояние	

	do {
		rc_step = step(); // Выполняем один шаг синтаксического анализа, сохраняем результат	
	} while (rc_step == Mfst::RC_STEP::NS_OK || rc_step == Mfst::RC_STEP::NS_NORULECHAIN 
		|| rc_step == Mfst::RC_STEP::TS_OK || rc_step == Mfst::RC_STEP::TS_NOK);
	// Продолжаем, если успешно обработан нетерминал
	// Продолжаем, если была найдена цепочка правил
	// Продолжаем, если успешно обработан терминал
	// Продолжаем, если была ошибка с терминалом, но её можно восстановить

	switch (rc_step) {
	case Mfst::RC_STEP::TAPE_END: // Если достигли конца ленты, синтаксический анализ завершён успешно
		MFST_TRACE4("------>TAPE_END", outfile);
		outputStream << "Синтаксический анализ выполнен без ошибок\n";
		output = true;
		break;
		// Если для нетерминала не найдено правило
	case Mfst::RC_STEP::NS_NORULE:
		MFST_TRACE4("------>NS_NORULE", outfile);
		outputStream << "-------------------------------------------------------------------------------------" << std::endl;
		outputStream << getDiagnosis(0) << std::endl;
		//outputStream << getDiagnosis(1) << std::endl;
		//outputStream << getDiagnosis(2) << std::endl;
		break;
		// Если для текущей цепочки правил не найдено подходящего продолжения
	case Mfst::RC_STEP::NS_NORULECHAIN:
		MFST_TRACE4("------>NS_NORULECHAIN", outfile);
		break;
		// Если произошла ошибка в обработке нетерминала
	case Mfst::RC_STEP::NS_ERROR:
		MFST_TRACE4("------>NS_ERROR",outfile);
		break;
		// Если синтаксический анализатор оказался в неожиданном состоянии
	case Mfst::RC_STEP::SURPRISE:
		MFST_TRACE4("------>SURPRISE", outfile);
		break;
	}

	return output;
}

bool MFST::Mfst::savediagnosis(RC_STEP rc_step) {
	bool output = false; // Флаг, указывающий, удалось ли сохранить диагноз
	short k = 0;
	// Поиск позиции для сохранения диагноза
	while (k < MFST_DIAGN_NUMBER && tape_position <= diagnosis[k].tape_position)
		k++;// Увеличиваем индекс, пока не найдём подходящее место
	// Если есть место для сохранения	
	if (output = (k < MFST_DIAGN_NUMBER)) { // Сохраняем диагноз в найденной позиции
		diagnosis[k] = MfstDiagnosis(tape_position, rc_step, nrule, nrulechain);
		// Устанавливаем оставшиеся позиции как пустые (сбрасываем tape_position)
		for (short j = k + 1; j < MFST_DIAGN_NUMBER; ++j) {
			diagnosis[j].tape_position = -1;
		}
	}

	return output;// Возвращаем, удалось ли сохранить диагноз
}

void MFST::Mfst::printrules() {
	MfstState state;// Хранит текущее состояние из стека состояний
	GRB::Rule rule; // Хранит текущее правило

	// Перебираем сохранённые состояния
	for (unsigned short k = 0; k < storestate.size(); k++)
	{
		state = storestate.c[k];// Извлекаем состояние
		rule = greibach.getRule(state.nrule); // Получаем правило для текущего состояния
		MFST_TRACE7(outfile) // Логируем правило
	}
}

bool MFST::Mfst::save_dedication() {
	MfstState state;// Хранит текущее состояние из стека 
	GRB::Rule rule;// Хранит текущее правило

	dedication.size = (short)storestate.size();// Устанавливаем размер выделения (количество сохранённых состояний)
	dedication.nrules = DBG_NEW short[dedication.size];  // Выделяем память для массивов номеров правил и цепочек правил
	dedication.nrulechains = DBG_NEW short[dedication.size];

	for (unsigned short k = 0; k < storestate.size(); k++)  // Перебираем состояния и сохраняем их номера правил и цепочек
	{
		state = storestate.c[k]; // Извлекаем состояние
		dedication.nrules[k] = state.nrule; // Сохраняем номер правила	
		dedication.nrulechains[k] = state.nrulechain;// Сохраняем номер цепочки правил
	};
	return true;
}

MFST::Mfst::Dedication::Dedication()
	: size(0), nrules(nullptr), nrulechains(nullptr)
{	}