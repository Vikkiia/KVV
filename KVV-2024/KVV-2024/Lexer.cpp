#include "pch.h"
#include "Lexer.h"
#include "FST.h"


char LA::Tokenize(const char* string) {
	
	FST::FST GRAPHS[] = { FST_ELSE, FST_IF,FST_LEFT_SQUARE_BRACE,FST_RIGHT_SQUARE_BRACE
	, FST_STRING, FST_FUNCTION, FST_DECLARE,
	FST_RETURN, FST_PRINT, FST_MAIN,
	FST_LEFTHESIS, FST_RIGHTHESIS, FST_SEMICOLON, FST_COMMA,
	FST_LEFTBRACE, FST_BRACELET,
	FST_PLUS, FST_MINUS, FST_STAR, FST_DIRSLASH, FST_EQUALS,
	 FST_STRING_LITERAL, FST_INTEGER_LITERAL, FST_UINT,
	GRAPH_POW, GRAPH_RANDOM, FST_MOD, FST_BIGGEROREQUAL,FST_SMALLEROREQUAL,FST_NOTEQUAL,FST_SMALLERTHAN,
	FST_BIGGERTHAN,FST_EQUALS_TWO,/*GRAPH_LENGTH,*/FST_ID
	};
	const int size = sizeof(GRAPHS) / sizeof(GRAPHS[0]);//берет весь размер массива GRAPHS и делит его на размер первого элемента  узнать сколько будет итераций
	const char tokens[] = { LEX_ELSE, LEX_IF/*, LEX_WHILE*/,LEX_LEFT_SQUARE_BRACE,LEX_RIGHT_SQUARE_BRACE,
	  LEX_STRING, LEX_FUNCTION, LEX_DECLARE,
	  LEX_RETURN, LEX_PRINT, LEX_MAIN,
	  LEX_LEFTHESIS, LEX_RIGHTHESIS, LEX_SEMICOLON, LEX_COMMA,
	  LEX_LEFTBRACE, LEX_BRACELET,
	  LEX_PLUS, LEX_MINUS, LEX_STAR, LEX_DIRSLASH, LEX_EQUALS,
	   LEX_STRING_LITERAL, LEX_INTEGER_LITERAL, LEX_UINT,LEX_POW, LEX_RAND, LEX_MOD,LEX_BIGGEROREQUAL,LEX_SMALLEROREQUAL
	  ,LEX_NOTEQUAL,LEX_SMALLERTHEN,LEX_BIGGERTHEN,LEX_EQUAL_ID,/*LEX_LENGTH,*/LEX_ID

	};
	
	for (int i = 0; i < size; ++i) {
		if (execute(string, GRAPHS[i])) {
			return tokens[i];
		}
	}

	return EOF;
}

short LA::Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, Parm::PARM& parm, Log::LOG& log) {
	short lexErrors = 0; // Счётчик ошибок лексического анализа.
	std::ofstream outfile(parm.out);
	outfile << "01 ";

	std::string accumulator = "";// Накопитель для текущего токена.
	std::string curScope = ""; 
	std::string prevScope = "";// Предыдущая область видимости.
	std::string curFunction = "";// Имя текущей функции.
	std::string prevId = "";// Имя предыдущего идентификатора.
	std::string processingFunction = "";// Имя функции, которая сейчас обрабатывается.
	bool declaredNewFunction = false;
	
	for (int i = 0, line = 1; in.text[i]; ++i) {
		IT::IDDATATYPE iddatatype;
		auto fillTables = [&] {//Лямбда-функция для обработки накопленного токена, добавления его в таблицы и проверки ошибок.
			char token = Tokenize(accumulator.c_str());// Анализируем накопленный токен.
			if (token == EOF) { // Если токен не распознан.
				lexErrors++;
				Log::Write(log, accumulator.c_str(), " - ", ""); 
				Error::ERROR temperr = Error::geterrorin(129, line, -1);
				*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line<< std::endl;
			}

			int ti_idx = TI_NULLIDX;// Индекс идентификатора в таблице идентификаторов.

			iddatatype = (token == LEX_UINT || token == LEX_INTEGER_LITERAL) ?
				(IT::IDDATATYPE::UINT) :
				(token == LEX_STRING || token == LEX_STRING_LITERAL) ? IT::IDDATATYPE::STR : iddatatype; // Определяем тип данных.

			if (token == LEX_LEFTHESIS) {
				declaredNewFunction = lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_FUNCTION;// Проверяем, объявляется ли функция

				if (declaredNewFunction) {
					prevScope = curScope;// Сохраняем текущую область видимости.
					curScope += curFunction;
				}
			}
			else if (token == LEX_RIGHTHESIS && declaredNewFunction) { 
				curScope = prevScope;
				declaredNewFunction = false;
			}
			else if (token == LEX_LEFTBRACE) { 
				prevScope = curScope;
				curScope += curFunction;
			}
			else if (token == LEX_BRACELET) {
				curScope.clear();
				curFunction.clear();
			}
			else if (token == LEX_INTEGER_LITERAL ) { 
				ti_idx = IT::IsId(idtable, curScope.c_str(), accumulator.c_str(), true);// Проверяем, есть ли литерал в таблице.
				if (ti_idx== LT_TI_NULLIDX) {
					IT::Add(idtable, { lextable.size,  curScope.c_str(), accumulator.c_str(), IT::IDDATATYPE::UINT, IT::IDTYPE::L });
					if (atoll(accumulator.c_str()) >_UI32_MAX)// Проверяем переполнение.
					{
						lexErrors++;// Логируем ошибку.
						Error::ERROR temperr = Error::geterrorin(132, line, -1);
						*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line<< std::endl;
						
					}
					idtable.table[idtable.size - 1].value.vint = atoi(accumulator.c_str());// Сохраняем значение.
				}
			
				token = LEX_LITERAL;
			}
			else if (token == LEX_STRING_LITERAL) {
				std::string newId = accumulator.substr(1, accumulator.size() - 2);
				/*1 — пропускается первый символ (открывающая кавычка).
accumulator.size() - 2 — определяет длину подстроки без кавычек.*/
				for (int counterSTR = 0; counterSTR < newId.length(); counterSTR++)
				{
					if (newId[counterSTR] == ' ')
						newId[counterSTR] = '_';//Если символ — пробел, он заменяется на _.
				}
				std::string literal = accumulator.substr(1, accumulator.size() - 2);//Создаётся ещё одна строка literal, содержащая значение строкового литерала без кавычек.
				ti_idx = IT::IsId(idtable, curScope.c_str(), newId.c_str(), true);//Проверяется, существует ли строковый литерал с идентификатором newId в текущей области видимости (curScope).	
				if (ti_idx == LT_TI_NULLIDX) {//Если строковый литерал отсутствует в таблице идентификаторов.
					IT::Add(idtable, { lextable.size,  curScope.c_str(), newId.c_str(), IT::IDDATATYPE::STR, IT::IDTYPE::L });//Добавляется новая запись в таблицу идентификаторов
					strcpy_s(idtable.table[idtable.size - 1].value.vstr.str, literal.c_str());//Копирует значение строкового литерала (literal) в поле value.vstr.str последней добавленной записи таблицы.
					idtable.table[idtable.size - 1].value.vstr.len = literal.length();
					strcpy_s(idtable.table[idtable.size - 1].id, newId.c_str());
				}
				token = LEX_LITERAL;

			}
			
			else if (token == LEX_POW)
			{ 
				
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "toPow", iddatatype, IT::IDTYPE::S , {2, IT::LENGHT_POW} });
			}
			else if (token == LEX_RAND)
			{
				
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "random", iddatatype, IT::IDTYPE::S, {1, IT::LENGHT_RAND} });
			}
			else if (token == LEX_PRINT)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "print", iddatatype, IT::IDTYPE::S });
				idtable.table[idtable.size - 1].value.params.amount = 1; // Указываем, что функция принимает один параметр.
			}
			else if (token == LEX_ID || token == LEX_POW || /*token == LEX_LENGTH ||*/ token == LEX_RAND ||token ==LEX_PRINT) {// Если токен является идентификатором, функцией random или print.
				std::string id = accumulator.substr(0, ID_MAXSIZE);// Получаем имя идентификатора (максимальная длина ID_MAXSIZE).
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str()); // Проверяем, существует ли идентификатор в таблице.
				
				if (ti_idx == TI_NULLIDX) {// Если идентификатор отсутствует.
					if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_DECLARE
						&& lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE) { // Если это объявление переменной, добавляем её в таблицу идентификаторов.
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_POW) ||
						(lextable.table[lextable.size - 1].lexeme == LEX_RAND )|| /*(lextable.table[lextable.size - 1].lexeme == LEX_LENGTH)||*/
						lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_PRINT)) { 
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FUNCTION) { 
						curFunction = id;
						processingFunction = id; 
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::F });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE
						&& declaredNewFunction) { // Если идентификатор — параметр новой функции, добавляем его в таблицу.
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::P });
					}
					else { // Если идентификатор не соответствует никакому шаблону, это ошибка.
						lexErrors++;
						Log::Write(log, accumulator.c_str(), " - ", ""); 
						Error::ERROR temperr = Error::geterrorin(124, line, -1);
						*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
						
					}
				}
				else if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_DECLARE
					|| lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FUNCTION) { 
					lexErrors++;
					Log::Write(log, accumulator.c_str(), " - ", "");
					Error::ERROR temperr = Error::geterrorin(123, line, -1);
					*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					
				}

			}
			else if (token == LEX_MAIN && curScope.empty()) {
				ti_idx = IT::IsId(idtable, "", accumulator.c_str());// Проверяем, существует ли идентификатор `main` в таблице идентификаторов в глобальной области.

				if (ti_idx != TI_NULLIDX) {// (повторное объявление).
					lexErrors++;// Увеличиваем счётчик лексических ошибок.
					Log::Write(log, accumulator.c_str(), " - ", "");
					Error::ERROR temperr = Error::geterrorin(131, line, -1);
					*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					
				}
				curScope.clear(); // Сбрасываем текущую область видимости (гарантируем глобальную область для `main`).
				curFunction = accumulator; // Устанавливаем имя текущей функции как `main`.

				IT::Add(idtable, { lextable.size, "", accumulator.c_str(), IT::IDTYPE::F, 0 });// Добавляем функцию `main` в таблицу идентификаторов с типом F (функция).
			}
			
			token = (token == LEX_UINT || token == LEX_STRING ) ? LEX_DATATYPE : token;// Если токен — это тип данных (UINT или STRING), преобразуем его в LEX_DATATYPE.

			
		
			if (token == LEX_ID  || token == LEX_MAIN) {
				if (ti_idx == TI_NULLIDX) {
					LT::Add(lextable, { token, line, idtable.size - 1 });
				}
				else {
					LT::Add(lextable, { token, line, ti_idx });
				}
			}
			else if (token == LEX_LITERAL)
			{
				if (ti_idx == LT_TI_NULLIDX) { 
					LT::Add(lextable, { token, line, idtable.size - 1 }); 
				}
				else
				{
					LT::Add(lextable, { token, line, ti_idx}); 

				}
			}
			else if (SeAn::FindSTD(token))// Если токен является стандартной функцией.
			{
				LT::Add(lextable, { token, line, idtable.size - 1 });
			}
			else {// Если токен не принадлежит ни одной из категорий.
				LT::Add(lextable, { token, line, LT_TI_NULLIDX });
			}

			outfile << token;
			accumulator.clear();
		};

		unsigned char& ch = in.text[i]; // Считываем текущий символ из входного текста.
		if (in.code[ch] == In::IN::T) {
			accumulator += ch;
		}
		else if (in.code[ch] == In::IN::Q) {
			do {
				accumulator += ch; 
				i++; 
				ch = in.text[i];
			} while (in.code[ch] != In::IN::Q);
			accumulator += ch;
			fillTables();// Обрабатываем накопленный токен.
		}
		else if (in.code[ch] == In::IN::S || in.code[ch] == In::IN::O || ch == IN_CODE_DELIM) {
			if (!accumulator.empty()) {// Если накопитель не пуст.
				if (ch == '=')
				{
					prevId = accumulator; // Сохраняем текущий накопитель как идентификатор.
				}
				fillTables();// Обрабатываем накопленный токен
			}
			if (in.code[ch] == In::IN::O) { 

				accumulator = ch;
				fillTables();
			}
		}

		if (ch == IN_CODE_DELIM) { 
			line++;
			if (in.text[i + 1]) { 
				outfile << "\n" << std::setw(2) << std::setfill('0') << line << " ";
				i++;
			}
		}
	}

	if (IT::IsId(idtable, "", "main") == TI_NULLIDX) {//есть ли main  // Проверяем, существует ли функция `main` в таблице идентификаторов.
		lexErrors++;
		Error::ERROR temperr = Error::geterror(130);
		*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке "<< std::endl;
	}
	LA::WriteDataForFunctions(lextable, idtable);
	outfile.close();
	return lexErrors;
}
void LA::WriteDataForFunctions(LT::LexTable& lextable, IT::IdTable& idtable)
{// Проходим по всем элементам таблицы лексем.
	for (int  i = 0; i < lextable.size; i++)
	{
		
		if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F &&
			lextable.table[i-1].lexeme==LEX_FUNCTION){ 
			int currentFuncIndex = i; // Сохраняем индекс текущей функции.
			short sizeParams = 0;// Переменная для подсчёта параметров функции.
			idtable.table[lextable.table[i].idxTI].value.params.amount = 0;// Устанавливаем начальное количество параметров функции равным 0.
			idtable.table[lextable.table[i].idxTI].value.params.types = new IT::IDDATATYPE; // Инициализируем массив для типов параметров функции.	
			i += 2;
			while (lextable.table[i].lexeme!=LEX_RIGHTHESIS) 
			{
				if (lextable.table[i].lexeme == LEX_ID) 
				{
					idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount++;// Увеличиваем счётчик параметров функции.
					int j = 0; // Индекс для копирования старых данных.
					IT::IDDATATYPE* buffer = new IT::IDDATATYPE[idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount]; // Создаём новый временный массив для хранения типов параметров.
					for (; j < idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount-1; j++) // Копируем существующие типы параметров в новый массив.
					{
						buffer[j] = idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[j];
					}  // Добавляем тип текущего параметра.
					idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[j] = idtable.table[lextable.table[i].idxTI].iddatatype;
				}
				i++;// Переходим к следующей лексеме.
			}

		}
	}
}

//void LA::ShowIDtable(IT::IdTable& idtable, std::ofstream* outfile)
//{
//	*outfile << "----------------------------------------------------[ID таблица]--------------------------------------------" << std::endl;
//	*outfile << "[ID]----------[SCOPE]---------[TYPE]-----------[IDTYPE]----------[value]--------------[LEidX]---------------" << std::endl;
//	for (int i = 0; i < idtable.size; i++)
//	{
//		*outfile <<'['<< i<<"] "<<idtable.table[i].id << "\t\t" << idtable.table[i].scope << "\t\t\t\t";
//		switch (idtable.table[i].iddatatype)
//		{
//		case IT::UINT:
//			*outfile << "INT\t\t\t\t";
//			break;
//		case IT::STR:
//			*outfile << "STR\t\t\t\t";
//			break;
//		default:
//			break;
//		}
//
//		switch (idtable.table[i].idtype)
//		{
//		case IT::IDTYPE::V:
//			*outfile << "variable\t\t\t\t";
//			break;
//		case IT::IDTYPE::F:
//			*outfile << "function\t\t\t\t";
//			break;
//		case IT::IDTYPE::S:
//			*outfile << "std function\t\t\t\t";
//			break;
//		case IT::IDTYPE::L:
//			*outfile << "literal\t\t\t\t";
//			break;
//		case IT::IDTYPE::P:
//			*outfile << "params\t\t\t";
//			break;
//		default:
//			break;
//		}
//		if (idtable.table[i].idtype == IT::IDTYPE::L)
//		{
//			if (idtable.table[i].iddatatype == IT::IDDATATYPE::UINT)
//			{
//				*outfile << idtable.table[i].value.vint;
//			}
//			
//			if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR)
//			{
//				*outfile << idtable.table[i].value.vstr.str;
//			}
//		}
//		else {
//			*outfile << "NULL";
//		}
//		*outfile << '\t' << idtable.table[i].idxfirstLE << std::endl;
//	}
//	*outfile << "------------------------------------------------------------------------------------------------------------" << std::endl;
//}


void LA::ShowIDtable(IT::IdTable& idtable, std::ofstream* outfile)
{
	*outfile << "----------------------------------------------------[ID таблица]--------------------------------------------" << std::endl;
	*outfile << std::left // Выравнивание текста по левому краю
		<< std::setw(10) << "[ID]"
		<< std::setw(15) << "[SCOPE]"
		<< std::setw(10) << "[TYPE]"
		<< std::setw(15) << "[IDTYPE]"
		<< std::setw(20) << "[VALUE]"
		<< std::setw(10) << "[LEidX]"
		<< std::endl;
	*outfile << "------------------------------------------------------------------------------------------------------------" << std::endl;

	for (int i = 0; i < idtable.size; i++)
	{
		*outfile << std::left
			<< std::setw(10) << ('[' + std::to_string(i) + ']') // Индекс идентификатора
			<< std::setw(15) << idtable.table[i].scope // Область видимости
			<< std::setw(10);

		switch (idtable.table[i].iddatatype)
		{
		case IT::UINT:
			*outfile << "INT";
			break;
		case IT::STR:
			*outfile << "STR";
			break;
		default:
			*outfile << "UNKNOWN";
			break;
		}

		*outfile << std::setw(15);
		switch (idtable.table[i].idtype)
		{
		case IT::IDTYPE::V:
			*outfile << "Variable";
			break;
		case IT::IDTYPE::F:
			*outfile << "Function";
			break;
		case IT::IDTYPE::S:
			*outfile << "Std Func";
			break;
		case IT::IDTYPE::L:
			*outfile << "Literal";
			break;
		case IT::IDTYPE::P:
			*outfile << "Params";
			break;
		default:
			*outfile << "Unknown";
			break;
		}

		if (idtable.table[i].idtype == IT::IDTYPE::L)
		{
			if (idtable.table[i].iddatatype == IT::IDDATATYPE::UINT)
			{
				*outfile << std::setw(20) << idtable.table[i].value.vint;
			}
			else if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR)
			{
				*outfile << std::setw(20) << idtable.table[i].value.vstr.str;
			}
		}
		else
		{
			*outfile << std::setw(20) << "NULL";
		}

		*outfile << std::setw(10) << idtable.table[i].idxfirstLE << std::endl;
	}

	*outfile << "------------------------------------------------------------------------------------------------------------" << std::endl;
}

void LA::ShowLexTable(LT::LexTable& lextable, std::ofstream* outfile)
{
	*outfile << "---------------------------------------[LEXTABLE]----------------------------------------------------------" << std::endl;
	*outfile << "-[LEXEME]--------[строка]-----[idx]------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < lextable.size; i++)
	{
		*outfile << '[' << i << "] " << lextable.table[i].lexeme << '\t' << lextable.table[i].sn << '\t';
		if (lextable.table[i].idxTI != TI_NULLIDX)
			*outfile << lextable.table[i].idxTI;
		else
			*outfile << "NULL";
		*outfile << std::endl;
	}
	*outfile << "-----------------------------------------------------------------------------------------------------------" << std::endl;
}

