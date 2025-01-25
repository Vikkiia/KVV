#include "pch.h"



bool Gener::CodeGeneration(LT::LexTable& lextable, IT::IdTable& idtable)
{
	std::ofstream AsmFile(ASM_PATH);
	if (!AsmFile.is_open())
	{
		throw ERROR_THROW(101);
	}
	bool gen_ok;//для генерации
	AsmFile << BEGIN;
	AsmFile << ".const\n";
	AsmFile << "ZEROMESSAGE " << " BYTE " << "\'Ошибка:деление на ноль\'," << 0<<std::endl;
	AsmFile << "OVERFLOWMESSAGE " << " BYTE " << "\'Ошибка:переполнение типа\'," << 0 << std::endl;
	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype == IT::L)
		{
			AsmFile << "\t" << idtable.table[i].scope<<idtable.table[i].id;//добав имя в файл асм и его идентификатор main10
			if (idtable.table[i].iddatatype ==IT::UINT)
			{
				AsmFile << " DWORD " << idtable.table[i].value.vint << std::endl;
			}
			else if (idtable.table[i].iddatatype == IT::STR)
			{
				AsmFile << " BYTE " <<'\''<< idtable.table[i].value.vstr.str<<'\'' << ", 0\n";
			}

		}
	}

	AsmFile << ".data\n";
	
	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype == IT::IDTYPE::V)
		{
			AsmFile << "\t" << idtable.table[i].scope<< idtable.table[i].id<<IT::V;
			if (idtable.table[i].iddatatype == IT::STR)
			{
				AsmFile << " DWORD ?\n";
			}
			if (idtable.table[i].iddatatype ==IT::UINT)
			{
				AsmFile << " DWORD ?\n";
			}
		}
	}
	AsmFile << "\n.code\n";
	bool declaredMain = false;//объявлен ли мэйн
	bool declaredFunc = false;
	int declaredFuncIndex;
	int ifsn = 0;//if 
	int elsefn = 0;
	bool elseExist = false;//выход из if
	bool ifExist = false;
	char currentLogicOperator;//логические операции
	for (int i = 0; i < lextable.size; i++)//идем по лиексемам 
	{
		switch (lextable.table[i].lexeme)
		{
			case LEX_FUNCTION:
			{
					declaredFunc = true;
					i++;
					declaredFuncIndex = i;
					AsmFile << (idtable.table[lextable.table[i].idxTI]).id << " PROC ";//сначала имя функции потом PROC
					i += 2;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS)
					{
						if (lextable.table[i].lexeme == LEX_ID)
						{
							
							AsmFile << idtable.table[lextable.table[i].idxTI].scope << idtable.table[lextable.table[i].idxTI].id<<IT::V << " : ";//обявление функции с парметрами 
							if (idtable.table[lextable.table[i].idxTI].iddatatype ==IT::UINT)
							{
								AsmFile << " DWORD ";
							}
							else if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::STR)
							{
								AsmFile << " DWORD ";
							}
						}
						if(lextable.table[i+1].lexeme!=LEX_RIGHTHESIS&& lextable.table[i].lexeme == LEX_ID)
						AsmFile << ", ";
						
						i++;


					}
					AsmFile << std::endl;
				break;
			}
			case LEX_MAIN:
			{
				AsmFile << "main PROC\n";
				declaredMain = true;
				break;
			}
		
			case LEX_BRACELET:
			{//если выход

				if (declaredFunc)
				{
					AsmFile << "ZEROERROR:\npush OFFSET ZEROMESSAGE\ncall writestrline\npush -1\n\tcall\t\tExitProcess\n" <<
						"OVERFLOW:\npush OFFSET OVERFLOWMESSAGE\ncall writestrline\npush -1\n\tcall\t\tExitProcess\n" << idtable.table[lextable.table[declaredFuncIndex].idxTI].id << " ENDP\n\n";
					declaredFunc = false;
				}
				else
				{
					AsmFile << "call\tsystem_pause\n \tcall\t\tExitProcess\nZEROERROR:\npush OFFSET ZEROMESSAGE\ncall writestrline\npush -1\n\tcall\t\tExitProcess\n" <<
						"OVERFLOW:\npush OFFSET OVERFLOWMESSAGE\ncall writestrline\npush -1\n\tcall\t\tExitProcess\n" <<" main ENDP\nEND main";
				}
				declaredFuncIndex = 0;
				break;
			}
			case LEX_RETURN:
			{
				i = Gener::GenExpHandler(AsmFile, lextable, idtable, ++i);//вызываемая функция (ниже)
				if (!declaredMain)
				{
					
					AsmFile  << " \nret\n";
				}
				else {
					AsmFile << "push eax\n";

				}
				break;
			}
			case LEX_PRINT:
			{
				IT::IDDATATYPE type = Gener::DetectType(lextable, idtable, i + 1);//(ниже)
				i = Gener::GenExpHandler(AsmFile, lextable, idtable, ++i);
				if (type ==IT::UINT)
				{
					AsmFile << "push eax\ncall writenumline\n";
				}
				else if(type == IT::STR)
				{
					AsmFile << "push eax\ncall writestrline\n";
				}
				break;
			}
			case LEX_EQUALS:
			{
				int idx = lextable.table[i - 1].idxTI;//получаем индекс ликсемы
				i = Gener::GenExpHandler(AsmFile, lextable, idtable, ++i);
				AsmFile << "push eax\npop " << idtable.table[idx].scope << idtable.table[idx].id;//извлечет знаеыниф из стека
				if (idtable.table[idx].idtype == IT::V)
					AsmFile << IT::V;
				AsmFile << std::endl;
				break;
			}
			case LEX_IF:
			{
				int checkerElseIndex = i;
				while (lextable.table[checkerElseIndex].lexeme!=LEX_RIGHT_SQUARE_BRACE)
				{
					checkerElseIndex++;
				}
				if (lextable.table[checkerElseIndex + 1].lexeme == LEX_ELSE)
				{
					elseExist = true;
					elsefn = lextable.table[checkerElseIndex+1].sn;//// Сохраняем номер строки для блока else 
				}
				ifExist = true;

				AsmFile <<
					"push " << idtable.table[lextable.table[i + 2].idxTI].scope << idtable.table[lextable.table[i + 2].idxTI].id;//Операнд помещается в стек с помощью команды push. Если это переменная (IT::V), добавляется суффикс.
				if (idtable.table[lextable.table[i+2].idxTI].idtype == IT::V)
					AsmFile << IT::V;
				AsmFile << std::endl;

				AsmFile << "push " << idtable.table[lextable.table[i + 4].idxTI].scope << idtable.table[lextable.table[i + 4].idxTI].id;//записываем второй операнд
				if (idtable.table[lextable.table[i+4].idxTI].idtype == IT::V)
					AsmFile << IT::V;
				AsmFile << std::endl<< "pop ebx\npop eax\ncmp eax, ebx\n";
				ifsn = lextable.table[i].sn;/// Запоминаем строку, где встретился if.
				currentLogicOperator = lextable.table[i + 3].lexeme; // Фиксируем логическую операцию .
				Gener::LogicOperations(AsmFile, lextable.table[i + 3].lexeme, ifsn);
				break;
			}
			case LEX_RIGHT_SQUARE_BRACE:
			{
				if (ifExist)
				{
					if(elseExist)
					Gener::LogicOperationsForElse(AsmFile, currentLogicOperator, elsefn);

					AsmFile << "SKIP" << ifsn << ":\n";
					ifExist = false;
				}
				else if (elseExist)
				{
					AsmFile << "SKIPELSE" << elsefn << ":\n";// Переход на else или выход
					elseExist = false;
				}
				break;
			}
		default:
			break;
		}
	}
	
	std::stack<std::string> stk;
	AsmFile.close();
	return true;

}

int Gener::GenExpHandler(std::ofstream& AsmFile, LT::LexTable& LEXTABLE, IT::IdTable& idtable, int i) {//
	for (; LEXTABLE.table[i].lexeme!=LEX_SEMICOLON; i++)
	{
		switch (LEXTABLE.table[i].lexeme)
		{
		case LEX_PLUS:
		{
			AsmFile << "pop eax" << std::endl <<
				"pop ebx" << std::endl << "add eax,ebx" << std::endl << "push eax\n" <<
				"jc OVERFLOW\n";
			break;
		}
		case LEX_MINUS:
		{
			AsmFile << "pop ebx" << std::endl <<
				"pop eax" << std::endl << "sub eax,ebx" << std::endl << "push eax" << std::endl <<
				"jc OVERFLOW\n";
			break;
		}
		case LEX_STAR:
		{
			AsmFile << "pop ebx" << std::endl <<
				"pop eax" << std::endl << "mul ebx" << std::endl << "push eax" << std::endl << 
				"jc OVERFLOW\n";
			break;
		}
		case LEX_DIRSLASH:
		{
			//idiv - знаковый , div - беззнаковый
			AsmFile << "\n\tpop ebx\n\tpop eax\ntest ebx, ebx\njz ZEROERROR\n\tcdq\n\tdiv ebx\n\tpush eax\n" << 
				"jc OVERFLOW\n";
			break;
		}
		case LEX_MOD:
		{
			AsmFile << "\n\tpop ebx\n\tpop eax\ntest ebx, ebx\njz ZEROERROR\n\tcdq\n\tdiv ebx\n\tpush edx\n" <<
				"jc OVERFLOW\n";
			break;
		}
		case LEX_LITERAL:
		case LEX_ID:
		{
			if (idtable.table[LEXTABLE.table[i].idxTI].iddatatype ==IT::UINT)
			{
				AsmFile << "push " << idtable.table[LEXTABLE.table[i].idxTI].scope << idtable.table[LEXTABLE.table[i].idxTI].id;
				if (idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::V || idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::P)
					AsmFile << IT::V;
				AsmFile << std::endl;
			}
		
			else if((idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::L) &&
				idtable.table[LEXTABLE.table[i].idxTI].iddatatype == IT::STR)
			{
				AsmFile << "push OFFSET " <<
					idtable.table[LEXTABLE.table[i].idxTI].scope <<
					idtable.table[LEXTABLE.table[i].idxTI].id << std::endl;
			}
			else
			{
				AsmFile << "push  " <<
					idtable.table[LEXTABLE.table[i].idxTI].scope <<
					idtable.table[LEXTABLE.table[i].idxTI].id;
				if (idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::V|| idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::P)
					AsmFile << IT::V;
				AsmFile << std::endl;
			}
			break;
		}
		case PL_AT:
		{
			AsmFile << "call " << idtable.table[LEXTABLE.table[i].idxTI].id << std::endl;
			if (idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::S)
				AsmFile << " pop ecx\n";
			AsmFile<<" push eax\n";

			
			break;
		}
		default:
			break;
		}
	}
	AsmFile << "pop eax\n";
	return i;
}

IT::IDDATATYPE Gener::DetectType(LT::LexTable lextable, IT::IdTable idtable, int i)//опред тип в айди таблцице 
{
	for (;lextable.table[i].lexeme!=LEX_SEMICOLON ; i++)
	{
		if (lextable.table[i].idxTI != LT_TI_NULLIDX)
		{
			return idtable.table[lextable.table[i].idxTI].iddatatype;//возвраш наг тип
		}
	}
}

void Gener::LogicOperations(std::ofstream& AsmFile, char lex, int sn)
{
	switch (lex)
	{
	case LEX_BIGGERTHEN:
	{
		AsmFile << "jbe SKIP" << sn << "\n";
		break;
	}
	case LEX_BIGGEROREQUAL:
	{
		AsmFile << "jb SKIP" << sn << "\n";

		break;
	}
	case LEX_SMALLERTHEN:
	{
		AsmFile << "jae SKIP" << sn << "\n";

		break;
	}
	case LEX_SMALLEROREQUAL:
	{
		AsmFile << "ja SKIP" << sn << "\n";

		break;
	}
	case LEX_NOTEQUAL:
	{
		AsmFile << "je SKIP" << sn << "\n";

		break;
	}
	case LEX_EQUAL_ID:
	{
		AsmFile << "jne SKIP" << sn << "\n";

		break;
	}
	default:
		break;
	}
}

void Gener::LogicOperationsForElse(std::ofstream& AsmFile, char lex, int sn)
{
	switch (lex)
	{
	case LEX_BIGGERTHEN:
	{
		AsmFile << "ja SKIPELSE" << sn << "\n";
		break;
	}
	case LEX_BIGGEROREQUAL:
	{
		AsmFile << "jae SKIPELSE" << sn << "\n";

		break;
	}
	case LEX_SMALLERTHEN:
	{
		AsmFile << "jbe SKIPELSE" << sn << "\n";

		break;
	}
	case LEX_SMALLEROREQUAL:
	{
		AsmFile << "jbe SKIPELSE" << sn << "\n";

		break;
	}
	case LEX_NOTEQUAL:
	{
		AsmFile << "jne SKIPELSE" << sn << "\n";

		break;
	}
	case LEX_EQUAL_ID:
	{
		AsmFile << "je SKIPELSE" << sn << "\n";

		break;
	}
	default:
		break;
	}
}
//void Gener::LogicOperationsForWhile(std::ofstream& AsmFile, char lex, int sn)
//{
//	switch (lex)
//	{
//	case LEX_BIGGERTHEN:
//	{
//		AsmFile << "jae TOWHILE" << sn << "\n";
//		break;
//	}
//	case LEX_BIGGEROREQUAL:
//	{
//		AsmFile << "jae TOWHILE" << sn << "\n";
//
//		break;
//	}
//	case LEX_SMALLERTHEN:
//	{
//		AsmFile << "jb TOWHILE" << sn << "\n";
//
//		break;
//	}
//	case LEX_SMALLEROREQUAL:
//	{
//		AsmFile << "jbe TOWHILE" << sn << "\n";
//
//		break;
//	}
//	case LEX_NOTEQUAL:
//	{
//		AsmFile << "jne TOWHILE" << sn << "\n";
//
//		break;
//	}
//	case LEX_EQUAL_ID:
//	{
//		AsmFile << "je TOWHILE" << sn << "\n";
//
//		break;
//	}
	/*default:
		break;
	}*/
//}