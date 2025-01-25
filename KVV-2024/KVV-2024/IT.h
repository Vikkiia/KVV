#pragma once

#define ID_MAXSIZE		30 
#define SCOPE_MAXSIZE	10

#define TI_MAXSIZE		4096//126
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		((int)0xffffffff)
#define TI_STR_MAXSIZE	255
#define RANDOM_TYPE IT::IDDATATYPE::UINT
#define POW_TYPE IT::IDDATATYPE::UINT
#define LENGHT_TYPE IT::IDDATATYPE::UINT
#define MAX_PARAMS_COUNT 5			   	 //максимальное количество параметров у функции
#define POW_PARAMS_COUNT 2				 //кол-во параметров у функции pow
#define RANDOM_PARAMS_COUNT 1				 //кол-во параметров у функции rand



namespace IT {
	enum IDDATATYPE { UINT = 1,STR = 2};//типы данных идентификаторов: числовой, строковый
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5, Z = 6 };//типы идентификаторов: V = переменная, F = функция, P =параметр, L =литерал, S = стандартная функция Z - знак

	static IDDATATYPE LENGHT_PARAMS[] = { IT::STR };
	static IDDATATYPE LENGHT_POW[] = {IT::UINT,IT::UINT };
	static IDDATATYPE LENGHT_RAND[] = {IT::UINT };
	struct Entry {
		int idxfirstLE;
		char scope[SCOPE_MAXSIZE];
		char id[ID_MAXSIZE];
		IDDATATYPE iddatatype;
		IDTYPE idtype;
		struct parm
		{
			IDDATATYPE* types;
			short amount;
			parm(int amount, IDDATATYPE* types)
			{
				this->amount = amount;
				this->types = types;
			}
		};
		union {
			unsigned int vint;
			struct {
				char len;
				char str[TI_STR_MAXSIZE - 1];
			} vstr;
			double vdouble;
			struct
			{
				IDDATATYPE* types;
				
				short amount;
			} params;
		} value;
		Entry() = default;
		Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, int value);
		Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, double value);
		Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, const char* value);
		Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype);
		Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, Entry::parm value);
	};

	struct IdTable {
		int maxsize;
		int size;
		Entry* table;
	};
	IdTable Create(int size);
	void Add(IdTable& idtable, const Entry& entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE]);
	int IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE], bool lexFlag);
	int IsLiteral(IdTable& idtable, const char literal[TI_STR_MAXSIZE]);
	int IsLiteral(IdTable& idtable, int literal);
	void Delete(IdTable& idtable);
}