
#include "pch.h"

namespace PN
{
	
	// Функция находит конец выражения, заканчивающегося символом `;` (LEX_SEMICOLON)
	int GetExpr(LT::LexTable lexTable, int i)
	{
		for (; lexTable.table[i].lexeme != LEX_SEMICOLON; i++); // Перебираем символы до конца выражения
		return i;
	}
	// Функция возвращает приоритет операции для польской нотации
	int Priorities(char operation)
	{
		if (operation == LEX_LEFTHESIS || operation == LEX_RIGHTHESIS)
			return 1;// Самый низкий приоритет: скобки
		if (operation == LEX_MINUS || operation == LEX_PLUS)
			return 2;// Приоритет: сложение/вычитание
		if (operation == LEX_DIRSLASH || operation == LEX_STAR||operation == LEX_MOD)
			return 3;// Самый высокий приоритет: умножение/деление/остаток
	}

	
	int ConverExpr(LT::Entry* expr, LT::LexTable lexTable, IT::IdTable idtable, int pos, std::ofstream* outfile)
	{
		for (int i = pos, j = GetExpr(lexTable, pos); i < j; i++)
		{
			*outfile << lexTable.table[i].lexeme;
		}
		*outfile << " => ";
		std::stack<LT::Entry> stack; // Стек для операций
		int sizeExpr = 0;// Счётчик размера выходного массива
		short leftBracket = 0;// Счётчик левых скобок

		// Перебираем выражение от начальной позиции до конца
		for (int j = GetExpr(lexTable, pos); pos < j; pos++)
		{
			if ((lexTable.table[pos].lexeme == LEX_ID || lexTable.table[pos].lexeme == LEX_LITERAL) && !CHECKFUNCTIONS(pos))
			{ // Если лексема - идентификатор или литерал, добавляем её в выходной массив
				expr[sizeExpr++] = lexTable.table[pos];
			}
			else if (lexTable.table[pos].idxTI!=LT_TI_NULLIDX && CHECKFUNCTIONS(pos))
			{// Обрабатываем функции
				LT::Entry Copy = lexTable.table[pos];
				Copy.lexeme = PL_AT; // Изменяем лексему для вызова функции
				pos++;
				for (int parmIndex = 0; parmIndex < idtable.table[Copy.idxTI].value.params.amount; pos++)
				{
					if (lexTable.table[pos].lexeme == LEX_ID || lexTable.table[pos].lexeme == LEX_LITERAL)
					{
						expr[sizeExpr++] = lexTable.table[pos];// Добавляем параметры функции
						parmIndex++;
					}
				}
				expr[sizeExpr++] = Copy;// Добавляем вызов функции в выходной массив
			}
			else if (lexTable.table[pos].lexeme == LEX_RIGHTHESIS)
			{ // Обрабатываем правую скобку
				
				while (stack.size())
				{
					if (stack.top().lexeme == LEX_LEFTHESIS)	break;// Убираем элементы из стека до левой скобки
					expr[sizeExpr++] = stack.top();
					stack.pop();
				}
				stack.pop();	leftBracket--;// Убираем левую скобку
			}
			else if (lexTable.table[pos].lexeme == LEX_LEFTHESIS)
			{// Добавляем левую скобку в стек
				leftBracket++;
				stack.push(lexTable.table[pos]);
			}
			else if (stack.size() == 0 || stack.top().lexeme == LEX_LEFTHESIS)
			{ // Если стек пуст или на вершине стека — левая скобка
				
				stack.push(lexTable.table[pos]);
			}
			else
			{
				// Обрабатываем операции с приоритетом
				while (stack.size())
				{
					if (Priorities(lexTable.table[pos].lexeme) > Priorities(stack.top().lexeme)) break;
					expr[sizeExpr++] = stack.top();// Убираем из стека элементы с меньшим или равным приоритетом
					stack.pop();
				}
				stack.push(lexTable.table[pos]);// Добавляем текущую операцию в стек
			}
		}
		// Добавляем оставшиеся операции из стека в выходной массив
		while (stack.size() != 0)
		{
			expr[sizeExpr++] = stack.top();
			stack.pop();
		}
		// Печатаем выражение после преобразования
		for (int i = 0; i < sizeExpr; i++)
		{
			*outfile << expr[i].lexeme;
		}
		*outfile << std::endl;
		return sizeExpr; // Возвращаем размер выходного массива
	}
	// Добавление нового выражения в лексическую таблицу
	void AddNewExpr(LT::LexTable& lexTable, IT::IdTable& idTable, LT::Entry* expr, int numConver, int sizeExpr)
	{
		for (int j = numConver, g = 0; g < sizeExpr; j++, g++)
		{
			lexTable.table[j] = expr[g];// Заменяем элементы в таблице
			if (ELEMIT)		idTable.table[lexTable.table[j].idxTI].idxfirstLE = j;// Обновляем индекс первой лексемы
		}
	}
	// Удаление пустых записей из лексической таблицы
	void DelNULLEntryLT(LT::LexTable& lexTable, IT::IdTable& idTable, int numConver,int sizeExpr, int endExpr)
	{
		for (int r = endExpr - (numConver + sizeExpr), g = 0; g < r; g++)
		{
			for (int j = numConver + sizeExpr; j < lexTable.size; j++)
			{
				lexTable.table[j] = lexTable.table[j + 1]; // Сдвиг таблицы
				if (ELEMIT)		idTable.table[lexTable.table[j].idxTI].idxfirstLE = j;// Обновление индексов
			}
			lexTable.size--;// Уменьшаем размер таблицы
		}
	}
	// Основная функция для преобразования выражений в польскую нотацию
	void PolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable, std::ofstream* outfile)
	{
		*outfile << "\n\t\tPolish Notation:\n";
		Check check;
		LT::Entry expr[200];// Массив для хранения выражения
		int numConver = 0;
		for (int i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].lexeme == LEX_EQUALS || lexTable.table[i].lexeme == LEX_RETURN || lexTable.table[i].lexeme == LEX_PRINT)
			{
				// Если найдена операция присваивания, возврата или вывода
				numConver = i + 1;
				int endExpr = GetExpr(lexTable, numConver);// Находим конец выражения
				int sizeExpr = ConverExpr(expr, lexTable, idTable, numConver, outfile);// Преобразуем выражение в польскую нотацию
				AddNewExpr(lexTable, idTable, expr, numConver,sizeExpr); // Добавляем преобразованное выражение в таблицу
				DelNULLEntryLT(lexTable, idTable, numConver,sizeExpr,endExpr);// Удаляем пустые записи
			}
		}
	}
}

