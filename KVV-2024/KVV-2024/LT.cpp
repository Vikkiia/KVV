#include "pch.h"
#include "LT.h"
#include "Error.h"

LT::LexTable LT::Create(int size) {
	if (size > LT_MAXSIZE) {
		throw ERROR_THROW(120);
	}

	LexTable lextable;
	lextable.maxsize = size;
	lextable.size = 0;
	lextable.table = DBG_NEW Entry[size];
	return lextable;
}

void LT::Add(LexTable& lextable, Entry entry) {
	if (lextable.size + 1 > lextable.maxsize) {
		throw ERROR_THROW(121);
	}
	
	lextable.table[lextable.size] = entry;
	lextable.size++;

	//for (;;) {
	//	if (lextable.size + 1 > lextable.maxsize) {
	//		throw ERROR_THROW(121);
	//	}
	//	// ƒобавл€ем новую запись в таблицу
	//	lextable.table[lextable.size] = entry;// —охран€ем запись в массив
	//	lextable.size++;// ”величиваем текущий размер таблицы
	//}

}

LT::Entry LT::GetEntry(LexTable& lextable, int n) {
	if (n < 0 || n > lextable.size - 1) {
		throw ERROR_THROW(122);
	}

	return lextable.table[n];
}

void LT::Delete(LexTable& lextable) {
	if (!lextable.table) {
		return;
	}

	delete[] lextable.table;
	lextable.table = nullptr;// ќбнул€ем указатель, чтобы избежать ошибок повторного удалени€
}