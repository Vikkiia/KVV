#include "pch.h"
#include "Error.h"

//   0 -  99 - системные ошибки
// 100 - 109 - ошибки параметров
// 110 - 119 - ошибки открытия и чтения файлов
// 120 - 129 - ошибки лексического анализатора
// 600 - 666 - ошибки синтаксического анализатора
Error::ERROR errors[ERROR_MAX_ENTRY] = {
	ERROR_ENTRY(0, "Системная ошибка: недопустимый код ошибки"),
	ERROR_ENTRY(1, "Системная ошибка: системный сбой"),
	ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
	ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
	ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
	ERROR_ENTRY_NODEF10(50),
	ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
	ERROR_ENTRY(100, "Ошибка открытия и чтения файлов: параметр -in должен быть задан"),
	ERROR_ENTRY(101,"Ошибка открытия и чтения файлов: ошибка при открытии файла .asm"), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
	ERROR_ENTRY(104, "Ошибка открытия и чтения файлов: превышена длина входного параметра"),
	ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
	ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
	ERROR_ENTRY(110, "Ошибка открытия и чтения файлов: ошибка при открытии файла с исходным кодом (-in)"),
	ERROR_ENTRY(111, "Ошибка открытия и чтения файлов: недопустимый символ в исходном файле (-in)"),
	ERROR_ENTRY(112, "Ошибка открытия и чтения файлов: ошибка при создании файла протокола (-log)"),
	ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
	ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
	ERROR_ENTRY(120, "Ошибка лексического анализа: превышено максимальное количество строк в таблице лексем"),
	ERROR_ENTRY(121, "Ошибка лексического анализа: превышена максимальная емкость таблицы лексем"),
	ERROR_ENTRY(122, "Ошибка лексического анализа: Выход за пределы таблицы лексем"),
	ERROR_ENTRY(123, "Ошибка лексического анализа: множественное объявление лексемы"),
	ERROR_ENTRY(124, "Ошибка лексического анализа: идентификатор не объявлен"),
	ERROR_ENTRY_NODEF(125),
	ERROR_ENTRY(126, "Ошибка лексического анализа: превышено максимальное количество строк в таблице идентификаторов"),
	ERROR_ENTRY(127, "Ошибка лексического анализа: превышена максимальная емкость таблицы идентификаторов"),
	ERROR_ENTRY(128, "Ошибка лексического анализа: выход за пределы таблицы идентификаторов"),
	ERROR_ENTRY(129, "Ошибка лексического анализа: неопознанная лексема"),
	ERROR_ENTRY(130, "Ошибка лексического анализа: отсутствует точка входа"),
	ERROR_ENTRY(131, "Ошибка лексического анализа: несколько точек входа"),
	ERROR_ENTRY(132,"Ошибка лексического анализа: значение литерала превышает максимальное значение типа uint"),	ERROR_ENTRY_NODEF(133), ERROR_ENTRY_NODEF(134), ERROR_ENTRY_NODEF(135),
	ERROR_ENTRY_NODEF(136), ERROR_ENTRY_NODEF(137), ERROR_ENTRY_NODEF(138),	ERROR_ENTRY_NODEF(139),
	ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
	ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
	ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
	ERROR_ENTRY(600,"Ошибка синтаксического анализа: неверная структура программы"),
	ERROR_ENTRY(601,"Ошибка синтаксического анализа: ошибочный оператор"),
	ERROR_ENTRY(602,"Ошибка синтаксического анализа: ошибка в выражении"),
	ERROR_ENTRY(603,"Ошибка синтаксического анализа: ошибка в параметрах функции"),
	ERROR_ENTRY(604,"Ошибка синтаксического анализа: ошибка в параметрах вызываемой функции"),
	ERROR_ENTRY(605, "Ошибка синтаксического анализа: ошибка в подвыражении"),
	ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608),
	ERROR_ENTRY(609, "Ошибка синтаксического анализа: неверный номер правила"),
	ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630),
	ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670),
	ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
	ERROR_ENTRY(700, "Ошибка семантического анализа: в функции отсутствует return"),
	ERROR_ENTRY(701, "Ошибка семантического анализа: функция не всегда возвращает значение"),
	ERROR_ENTRY(702, "Ошибка семантического анализа: попытка объявить функцию в main"), 
	ERROR_ENTRY(703,"Ошибка семантического анализа: ошибка в блоке main: функция возвращает не целочисленное занчение"),
	ERROR_ENTRY(704,"Ошибка семантического анализа: тип возвращаемого значения не соответствует типу функции"), 
	ERROR_ENTRY(705,"Ошибка семантического анализа: слишком много аргументов передано в функцию"),
	ERROR_ENTRY(706,"Ошибка семантического анализа: тип передаваемого аргумента не соответствует типу параметра функции"), 
	ERROR_ENTRY(707,"Ошибка семантического анализа: слишком мало аргументов передано в функцию"), 
	ERROR_ENTRY(708,"Ошибка семантического анализа: попытка переопределить библиотечную функцию"), 
	ERROR_ENTRY(709, "Ошибка семантического анализа: неопознаная библиотечная функция"), 
	ERROR_ENTRY(710, "Ошибка семантического анализа: в выражении используется не соответствующий тип данных"),
	ERROR_ENTRY(711, "Ошибка семантического анализа: в выражении используется не соответствующая ,по тиуп данных, функция"),
	ERROR_ENTRY(712, "Ошибка семантического анализа: попытка деления на 0"),
	ERROR_ENTRY(713, "Ошибка семантического анализа: попытка деления на переменную которая хранит 0"),
	ERROR_ENTRY_NODEF(714), ERROR_ENTRY_NODEF(715),
	ERROR_ENTRY_NODEF(716), ERROR_ENTRY_NODEF(717), ERROR_ENTRY_NODEF(718), ERROR_ENTRY_NODEF(719), ERROR_ENTRY_NODEF10(730),
	ERROR_ENTRY_NODEF10(740), ERROR_ENTRY_NODEF10(750), ERROR_ENTRY_NODEF10(760), ERROR_ENTRY_NODEF10(770),
	ERROR_ENTRY_NODEF10(780), ERROR_ENTRY_NODEF10(790), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
};

Error::ERROR Error::geterror(int id) {
	ERROR e;

	e.id = (id < 0 || id >= ERROR_MAX_ENTRY) ? 0 : id;
	strcpy_s(e.message, errors[e.id].message);
	
	return e;
}
Error::ERROR Error::geterrorin(int id, int line = -1, int col = -1) {
	ERROR e;

	e = geterror(id);
	e.inext.line = line; // Устанавливаем номер строки, переданный в качестве аргумента. По умолчанию -1 (не задано).
	e.inext.col = col;

	return e;
}