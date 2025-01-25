#include "pch.h"
#include "Parm.h"
#include "Error.h"


wchar_t* findSubstr(int argc, _TCHAR* argv[], wchar_t(&destination)[PARM_MAX_SIZE], const wchar_t* key) {
	wchar_t* substr = nullptr; 

	for (int i = 1; i < argc; ++i) {// Перебираем аргументы командной строки
		substr = wcsstr(argv[i], key);// Ищем подстроку

		if (substr) {
			wcscpy_s(destination, substr + wcslen(key));
			break;
		}
	}
	return substr;
}
// Функция получения параметров из командной строки
Parm::PARM Parm::getparm(int argc, _TCHAR* argv[]) {
	PARM p; 
	wchar_t* inParm = nullptr;
	// Лямбда-функция для проверки аргументов командной строки
	[argc, argv]() {
		if (argc <= 1)
			throw ERROR_THROW(100);

		for (int i = 1; i < argc; ++i) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);
			if (wcsstr(argv[i], PARM_IN))
				return;
		}

		throw ERROR_THROW(100);
	}();

	inParm = findSubstr(argc, argv, p.in, PARM_IN);// Получаем значение параметра `-in` и сохраняем его в `p.in`
	// Если параметр `-out` отсутствует, генерируем его на основе `-in`
	if (!findSubstr(argc, argv, p.out, PARM_OUT)) {
		wcscpy_s(p.out, inParm + wcslen(PARM_IN));// Копируем имя файла из `-in`
		wcscat_s(p.out, PARM_OUT_DEFAULT_EXT);// Добавляем расширение по умолчанию (например, `.out`)
	}
	// Если параметр `-log` отсутствует, генерируем его на основе `-in`
	if (!findSubstr(argc, argv, p.log, PARM_LOG)) {
		wcscpy_s(p.log, inParm + wcslen(PARM_IN));// Копируем имя файла из `-in`
		wcscat_s(p.log, PARM_LOG_DEFAULT_EXT);// Добавляем расширение по умолчанию (например, `.log`)
	}

	return p;
}