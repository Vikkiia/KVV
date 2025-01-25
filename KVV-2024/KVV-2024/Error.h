#pragma once
#define ERROR_THROW(id) Error::geterror(id);// Макрос для вызова функции `Error::geterror` с указанным идентификатором ошибки.
#define	ERROR_THROW_IN(id, l, c) Error::geterrorin(id, l, c);// Макрос для вызова функции `Error::geterrorin` с указанием идентификатора ошибки, строки и столбца.
#define ERROR_ENTRY(id, m) {id, m, {-1, -1}}// Макрос для создания записи об ошибке с заданным идентификатором `id`, сообщением `m` и значениями строки и столбца, равными -1 (значение по умолчанию).
#define ERROR_MAXSIZE_MESSAGE 200
#define ERROR_ENTRY_NODEF(id)		ERROR_ENTRY(-id, "Неопределенная ошибка")
// Макрос для создания записи об ошибке с отрицательным идентификатором `id` и сообщением "Неопределенная ошибка".

#define ERROR_ENTRY_NODEF10(id)		ERROR_ENTRY_NODEF(id+0),ERROR_ENTRY_NODEF(id+1),ERROR_ENTRY_NODEF(id+2),ERROR_ENTRY_NODEF(id+3), \
									ERROR_ENTRY_NODEF(id+4),ERROR_ENTRY_NODEF(id+5),ERROR_ENTRY_NODEF(id+6),ERROR_ENTRY_NODEF(id+7), \
									ERROR_ENTRY_NODEF(id+8),ERROR_ENTRY_NODEF(id+9)

#define ERROR_ENTRY_NODEF100(id)	ERROR_ENTRY_NODEF10(id+0),ERROR_ENTRY_NODEF10(id+10),ERROR_ENTRY_NODEF10(id+20),ERROR_ENTRY_NODEF10(id+30), \
									ERROR_ENTRY_NODEF10(id+40),ERROR_ENTRY_NODEF10(id+50),ERROR_ENTRY_NODEF10(id+60),ERROR_ENTRY_NODEF10(id+70), \
									ERROR_ENTRY_NODEF10(id+80),ERROR_ENTRY_NODEF10(id+90)
#define ERROR_MAX_ENTRY 1000

namespace Error {
	struct ERROR {
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];
		struct IN {
			short line;
			short col;
		} inext;
	};

	ERROR geterror(int id);
	ERROR geterrorin(int id, int line, int col);
}