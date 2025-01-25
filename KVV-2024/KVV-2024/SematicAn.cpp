#include "pch.h"
//
namespace SeAn
{
	 Error::ERROR temperr;// Переменная для хранения текущей ошибки
	short SeAn::CheckingReturnInMain(LT::LexTable& lextable, IT::IdTable& idtable,Log::LOG logfile)
	{
		short errExist = 0;//// Счётчик ошибок 
		short nestingLevel = 0;//уровень вложенности
		short warmingID = 0;//1 - не весь код возвращает значение(например return только в if блоке)  // Индикатор ошибок и предупреждений
		for (int lexema_I = 0; lexema_I < lextable.size; lexema_I++)// Перебор таблицы лексем
		{
			if (lextable.table[lexema_I].lexeme == LEX_MAIN)// Если найдена точка входа (main)
			{
				for (int lexIndexInMain = lexema_I; lexIndexInMain < lextable.size; lexIndexInMain++)// Обход содержимого main
				{
					if (lextable.table[lexIndexInMain].lexeme == LEX_LEFT_SQUARE_BRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_RIGHT_SQUARE_BRACE)
					{
						nestingLevel--;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_FUNCTION)// Если внутри main есть объявление функции
					{
						errExist++;// Ошибка, функция в main запрещена
						 temperr = Error::geterrorin(702, lextable.table[lexema_I].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_RETURN)
					{
						
						for ( ; lexIndexInMain <lextable.size&&lextable.table[lexIndexInMain].lexeme!=LEX_SEMICOLON; lexIndexInMain++)//идем до точки запятой
						{
							if (lextable.table[lexIndexInMain].idxTI != LT_TI_NULLIDX && idtable.table[lextable.table[lexIndexInMain].idxTI].iddatatype !=IT::UINT)
							{
								errExist++;
								 temperr = Error::geterrorin(703, lextable.table[lexema_I].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
							}//ЕСЛИ В МЕЙНЕ ВОЗВРАЩАЮТ НЕ INT
						}	

						if (nestingLevel == 0)
						{
							warmingID = -1;//соответствует количеству ретурнов
						}
						else
							warmingID = 1;
					}
					
				}
				if (warmingID == 1) // Проверка на отсутствие return
				{
					errExist++;
					 temperr = Error::geterrorin(701, lextable.table[lexema_I].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
				}//warming - не весь код возвращает return
				if (warmingID == 0)//если предыдущий не сработает
				{
					errExist++;
					 temperr = Error::geterrorin(700, lextable.table[lexema_I].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
				}//warming - нет return

				break;
			}
			
		}
		return errExist;
	}
	// Проверка наличия return в пользовательских функциях
	short SeAn::CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)
	{
		short errExist = 0;
		for (int i = 0; i < lextable.size; i++)
		{//ищем пользовательскую функцию
			if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F 
				&& lextable.table[i-1].lexeme == LEX_FUNCTION)
			{
				//сохраняем текущий тип функции
				IT::IDDATATYPE CurrentTypeOfFunction = idtable.table[lextable.table[i].idxTI].iddatatype;//записали в файле тип и передали его
				
				//когда нашли,скипаем параметры и переходим к телу функции
				while (lextable.table[i].lexeme != LEX_LEFTBRACE && i < lextable.size)
				{
					i++;
				}
				short nestingLevel = 0; 
				short warmingID = 0;
				//WARMING ID:
				//-1 - return всегда возвращает значение
				//0 - return отсутствует в функции
				//1 - return не всегда возвращает значение
				for (; i < lextable.size && lextable.table[i].lexeme!=LEX_BRACELET; i++)
				{
					//проверяем на вложенность 
					if (lextable.table[i].lexeme == LEX_LEFT_SQUARE_BRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[i].lexeme == LEX_RIGHT_SQUARE_BRACE)
					{
						nestingLevel--;
					}
					if (lextable.table[i].lexeme == LEX_RETURN)
					{
						//проверяем тип возвращаемого значения
						for (; i < lextable.size && lextable.table[i].lexeme != LEX_SEMICOLON; i++)
						{
							if (lextable.table[i].idxTI != LT_TI_NULLIDX && idtable.table[lextable.table[i].idxTI].iddatatype != CurrentTypeOfFunction)
							{
								errExist++;
								 temperr = Error::geterrorin(704, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
							}//ЕСЛИ возвращение значение не соотвтетсвует тип
						}
						if (nestingLevel != 0)
							warmingID = 1;
						if (nestingLevel == 0)
							warmingID = -1;
					}
				}
				switch (warmingID)
				{
				case -1:
					break;
				case 0:
					errExist++;
					 temperr = Error::geterrorin(700, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//ВЫШЕ ОПИСАНО
					break;
				case 1:
					errExist++;
					 temperr = Error::geterrorin(701, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//ВЫШЕ ОПИСАНО
					break;
				default:
					break;
				}
			}
		}
		return errExist;
	}

	short SeAn::CheckParamsOfFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)
	{
		short errExist = 0;
		for (int i = 0; i < lextable.size; i++)
		{
			//ищем вызов пользовательской функции
			if (lextable.table[i].lexeme == LEX_ID && lextable.table[i - 1].lexeme != LEX_FUNCTION &&
				idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F)
			{
				int currentFuncIndex = i;//запоминаем позицию проверяемой функции
				int currentIndexParam = 0;//номер аргумента
				i++;
				//проверяем до конца вызова функции
				while (lextable.table[i].lexeme!=LEX_RIGHTHESIS && i < lextable.size)
				{
					// если это идентификатор или литерал
					if (lextable.table[i].lexeme == LEX_ID || (lextable.table[i].idxTI!=TI_NULLIDX&&
						idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L))
					{
					
						
						if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount <= currentIndexParam)
						{
							errExist++;
							 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
							*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
						}//если аргументов больше чем параметров в функции
							//проверка на соответствие типов параметров и аргументоав
							if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[currentIndexParam] !=
								idtable.table[lextable.table[i].idxTI].iddatatype
								)
							{
								errExist++;
								 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//тип аргумента не соответствует типу параметра
							}
							currentIndexParam++;
						
					}
					i++;
				}
				if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount != currentIndexParam)
				{
					errExist++;
					 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
				}//СЛИШКОМ МАЛО АРГУМЕНТОВ ПЕРЕДАННО
			}
		}
		return errExist;
	}
	
	short SeAn::CheckParamsStdFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)//библ функ
	{
		short errExist = 0;
		for (int i = 0; i < lextable.size; i++)
		{
			if (SeAn::FindSTD(lextable.table[i].lexeme) && lextable.table[i - 1].lexeme == LEX_FUNCTION)
			{
				errExist++;
				 temperr = Error::geterrorin(708, lextable.table[i].sn, -1);
				*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
			}//попытка переопределить библиотечную функцию

			if (lextable.table[i].lexeme == LEX_DIRSLASH)
			{
				int divisionIndex = i;

				i++; 
				if (lextable.table[i].lexeme == LEX_LITERAL)
				{
					if (idtable.table[lextable.table[i].idxTI].value.vint == 0)
					{
						errExist++;
						temperr = Error::geterrorin(712, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ") " << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}
				}
				else if (lextable.table[i].lexeme == LEX_ID)
				{
					if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT &&
						idtable.table[lextable.table[i].idxTI].value.vint == 0)
					{
						errExist++;
						temperr = Error::geterrorin(713, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ") " << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}
				}
			}


			if (SeAn::FindSTD(lextable.table[i].lexeme))
			{
			
				short amountParams = 0;

				switch (lextable.table[i].lexeme)
				{
				
				case LEX_RAND:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
							{
								errExist++;
								 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
							}//тип арнгумента не соответсвует типу параметра
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
								{
									errExist++;
									 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
									*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//тип литерала не соответствует типу параметра
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 1)
					{
						errExist++;
						 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}//СЛИШКОМ МАЛО АРГУМЕНТОВ
					else if (amountParams > 1)
					{
						errExist++;
						 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//СЛИШКОМ МНОГО
					}

					break;
				case LEX_POW:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
							{
								errExist++;
								 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
							}//тип арнгумента не соответсвует типу параметра
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
								{
									errExist++;
									 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
									*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//тип литерала не соответствует типу параметра
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 2)
					{
						errExist++;
						 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}//СЛИШКОМ МАЛО АРГУМЕНТОВ
					else if (amountParams > 2)
					{
						errExist++;
						 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//СЛИШКОМ МНОГО
					}

					break;
				case LEX_PRINT:
				{
					i += 2;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						amountParams++;
						i++;
					}
					if (amountParams < 1)
					{
						errExist++;
						 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}//СЛИШКОМ МАЛО АРГУМЕНТОВ
					else if (amountParams > 1)
					{
						errExist++;
						 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//СЛИШКОМ МНОГО
					}
					break;
				}
				default:
					errExist++;
					 temperr = Error::geterrorin(709, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//неопознаная библиотечная функция
					break;
				}
			}
		}
		return errExist;
	}
	bool SeAn::FindSTD(unsigned char lex)
	{
		switch (lex)
		{
		case LEX_RAND:
			return true;
		case LEX_POW:
			return true;
		case LEX_PRINT:
			return true;
		default:
			return false;
			break;
		}
	}


	short SeAn::Types(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)
	{
		short errExist = 0;
		IT::IDDATATYPE currentType;//текущий тип


		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexeme == LEX_EQUALS )
			{
				currentType = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				while (lextable.table[i].lexeme!=LEX_SEMICOLON && i<lextable.size)
				{
					i++;
					if ((lextable.table[i].lexeme == LEX_ID || lextable.table[i].lexeme == LEX_LITERAL)
						&& idtable.table[lextable.table[i].idxTI].iddatatype != currentType)
					{
						if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F ||
							idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
						{
							errExist++;
							 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
							*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
						}//ФУНКЦИЯ ВОЗВРАЩАЕТ НЕ ТОТ ТИП ДАННЫХ
						else if(idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::V ||
							idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
						{
							errExist++;
							 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
							*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
						}//не верный тип данных используется
					}
					else if (lextable.table[i].lexeme == LEX_ID
						&& (idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::F&&
							idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::S)
						&& lextable.table[i + 1].lexeme == LEX_LEFTHESIS)
					{
						errExist++;
						 temperr = Error::geterrorin(711, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;//не верный вызов функции
					}
					else if (lextable.table[i].lexeme == LEX_RAND && currentType != IT::IDDATATYPE::UINT)
					{
						errExist++;
						 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}
					else if (lextable.table[i].lexeme == LEX_POW && currentType != IT::IDDATATYPE::UINT)
					{
						errExist++;
						 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line << std::endl;
					}
					if (SeAn::FindSTD(lextable.table[i].lexeme) ||
						lextable.table[i].lexeme == LEX_FUNCTION)
					{
						while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
							i++;
					}
				}
			}
		}
		return errExist;
	}
	
}

