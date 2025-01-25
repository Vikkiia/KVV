#include "pch.h"
#include "IT.h"
#include "Error.h"

IT::IdTable IT::Create(int size) {
	if (size > TI_MAXSIZE) {
		throw ERROR_THROW(126);
	}

	IdTable idtable;
	idtable.maxsize = size;
	idtable.size = 0;
	idtable.table = DBG_NEW Entry[size];
	return idtable;

	//for (;;) {
	//	IdTable idtable;// Создаём новый объект таблицы идентификаторов.
	//	idtable.maxsize = size;// Устанавливаем максимальный размер таблицы.
	//	idtable.size = 0;// Изначально таблица пуста.
	//	idtable.table = DBG_NEW Entry[size];// Выделяем память для массива записей (объекты `Entry`)
	//	return idtable;// Возвращаем созданную таблицу.
	//}
}

void IT::Add(IdTable& idtable, const Entry& entry) {
	if (idtable.size + 1 > idtable.maxsize) {
		throw ERROR_THROW(127);
	}

	idtable.table[idtable.size] = entry;
	idtable.size++;

	//for (;;) {
	//	if (idtable.size + 1 > idtable.maxsize) {
	//		throw ERROR_THROW(127);
	//	}

	//	idtable.table[idtable.size] = entry;// Добавляем запись в таблицу на позицию `size`.
	//	idtable.size++;
	//}
}
//Возвращает запись по её индексу.
IT::Entry IT::GetEntry(IdTable& idtable, int n) {
	if (n < 0 || n > idtable.size - 1) {
		throw ERROR_THROW(128);
	}

	return idtable.table[n];
}
//Проверяет, существует ли идентификатор с заданной областью видимости (scope) и именем
int IT::IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE]) {
	for (int i = 0; i < idtable.size; ++i) {
		if (idtable.table[i].idtype != IT::IDTYPE::L    
			&& !memcmp(scope, idtable.table[i].scope, (strlen(idtable.table[i].scope) < SCOPE_MAXSIZE ?
				strlen(idtable.table[i].scope) : SCOPE_MAXSIZE))// Сравниваем области видимости.
			&& !memcmp(id, idtable.table[i].id, (strlen(idtable.table[i].id) < ID_MAXSIZE ?
				strlen(idtable.table[i].id) : ID_MAXSIZE))) {// Сравниваем идентификаторы.
			return i;// Если идентификатор найден, возвращаем его индекс.
		}
	}

	return TI_NULLIDX;
}
//Проверяет идентификаторы с дополнительным флагом lexFlag.
int IT::IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE], bool lexFlag) {
	for (int i = 0; i < idtable.size; ++i) {
		if (!memcmp(scope, idtable.table[i].scope, (strlen(idtable.table[i].scope) < SCOPE_MAXSIZE ?
				strlen(idtable.table[i].scope) : SCOPE_MAXSIZE))
			&& !memcmp(id, idtable.table[i].id, (strlen(idtable.table[i].id) < ID_MAXSIZE ?
				strlen(idtable.table[i].id) : ID_MAXSIZE))&& (idtable.table[i].idtype == IT::L)) {// Проверяем, что это литерал.
			return i;// Возвращаем индекс найденного идентификатора.
		}
	}

	return TI_NULLIDX;
}
//Проверяют существование литералов в таблице.
int IT::IsLiteral(IdTable& idtable, const char literal[TI_STR_MAXSIZE]) {
	for (int i = 0; i < idtable.size; ++i) {
		if (!strcmp(idtable.table[i].value.vstr.str, literal) &&// Сравниваем строки.
			idtable.table[i].idtype == IT::IDTYPE::L) {
			return i;
		}
	}
	return TI_NULLIDX;
}
//Литерал-число:
int IT::IsLiteral(IdTable& idtable, int literal) {
	for (int i = 0; i < idtable.size; ++i) {
		if (idtable.table[i].value.vint == literal &&
			idtable.table[i].idtype == IT::IDTYPE::L) {
			return i;
		}
	}
	return TI_NULLIDX;// Литерал не найден.
}

void IT::Delete(IdTable& idtable) {
	if (!idtable.table) {//// Проверяем, есть ли данные для удаления.
		return;
	}

	delete[] idtable.table; 
	idtable.table = nullptr; 
}
//онструктор для числового значения :
IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, int value)
	: idxfirstLE(idxfirstLE), iddatatype(IDDATATYPE::UINT), idtype(idtype)// Инициализация членов.
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id)); // Копируем имя идентификатора.
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));// Копируем область видимости.

	this->value.vint = value;// Устанавливаем числовое значение.
}

// Конструктор для строкового значения:
IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, const char* value)
	: idxfirstLE(idxfirstLE), iddatatype(IDDATATYPE::STR), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));// Копируем имя.
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	this->value.vstr.len = (char)strlen(value);// Устанавливаем длину строки.
	strcpy_s(this->value.vstr.str, value); // Копируем строку.
}
// Конструктор для параметров:
IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype)
	: idxfirstLE(idxfirstLE), iddatatype(iddatatype), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));// Копируем имя идентификатора в поле `id`.
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	if (this->iddatatype == IDDATATYPE::UINT) {
		value.vint = TI_INT_DEFAULT;// Если тип данных UINT, устанавливаем значение по умолчанию.
	}
	else {
		value.vstr.len = TI_STR_DEFAULT;// Если тип данных STR, устанавливаем длину строки по умолчанию.
	}
}IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, Entry::parm value)
	: idxfirstLE(idxfirstLE), iddatatype(iddatatype), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	this->value.params.amount = value.amount;// Устанавливаем количество параметров.
	this->value.params.types = value.types;// Устанавливаем типы параметров
}