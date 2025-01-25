#include "pch.h"
#include "Log.h"

#define BUFFER_MAX_SIZE 200

Log::LOG Log::getlog(wchar_t logfile[]) {
	LOG new_log;

	new_log.stream = DBG_NEW std::ofstream;
	new_log.stream->open(logfile);

	if (!new_log.stream->is_open()) 
		throw ERROR_THROW(112);

	wcscpy_s(new_log.logfile, logfile); // Сохраняем имя лог-файла

	return new_log;
}

// Функция для записи строки в лог-файл (версии для `const char*`)
void Log::Write(LOG log, const char* c, ...) {
	
	std::ostream& output = log.stream && log.stream->is_open() ? *log.stream : std::cout;
	const char** args = &c; 

	for (int i = 0; args[i] != ""; ++i) 
		output << args[i];
}
// Функция для записи строки в лог-файл (версии для `const wchar_t*`)
void Log::Write(LOG log, const wchar_t* c, ...) {
	std::ostream& output = log.stream && log.stream->is_open() ? *log.stream : std::cout; 
	char convertedBuffer[BUFFER_MAX_SIZE];
	const wchar_t** args = &c;// Буфер для конвертации из wchar_t в char

	
	for (int i = 0; args[i] != L""; ++i) {
		wcstombs_s(nullptr, convertedBuffer, args[i], sizeof(convertedBuffer)); // Конвертируем wchar_t в char
		output << convertedBuffer;// Записываем конвертированную строку в выходной поток
	}
}

// Функция для записи заголовка в лог-файл с текущей датой и временем
void Log::WriteLog(LOG log) {
	char timeBuffer[BUFFER_MAX_SIZE]; 
	tm currentTime;// Структура для хранения текущего времени
	time_t now = time(0);// Получаем текущее время

	localtime_s(&currentTime, &now); // Конвертируем время в локальную структуру
	strftime(timeBuffer, sizeof(timeBuffer), "%c", &currentTime); // Форматируем время в строку

	*log.stream << "---- Протокол ------ Дата: " << timeBuffer << " ------------" << std::endl;
}
// Вспомогательная функция для конвертации и вывода параметров в лог
void outputConverted(Log::LOG& log, wchar_t(&source)[PARM_MAX_SIZE], const wchar_t* key) {

	char convertedBuffer[BUFFER_MAX_SIZE];

	wcstombs_s(nullptr, convertedBuffer, key, sizeof(convertedBuffer)); // Конвертируем ключ (wchar_t) в char и записываем в лог
	*log.stream << convertedBuffer << " ";

	wcstombs_s(nullptr, convertedBuffer, source, sizeof(convertedBuffer));// Конвертируем значение (wchar_t) в char и записываем в лог
	*log.stream << convertedBuffer << std::endl;
}

void Log::WriteParm(LOG log, Parm::PARM parm) {
	*log.stream << "---- Параметры --------" << std::endl;
	outputConverted(log, parm.log, PARM_LOG);
	outputConverted(log, parm.out, PARM_OUT);
	outputConverted(log, parm.in, PARM_IN);
}

void Log::WriteIn(LOG log, In::IN in) {
	*log.stream << "---- Исходные данные ------" << std::endl <<
		"Количество символов: " << in.size << std::endl <<
		"Проигнорировано: " << in.ignor << std::endl <<
		"Количество строк: " << in.lines << std::endl;
	*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
}

void Log::WriteError(LOG log, Error::ERROR error) {
	std::ostream& output = log.stream && log.stream->is_open() ? *log.stream : std::cout;// Определяем, куда писать данные: в файл или консоль

	output << "Ошибка " << error.id << ": " << error.message; // Если есть информация о строке и позиции, записываем её
	error.inext.col + error.inext.line >= 0 ? output <<
		", строка " << error.inext.line <<
		", позиция " << error.inext.col << std::endl : output << std::endl;
}

void Log::Close(LOG& log) {
	if (!log.stream)
		return;

	log.stream->close();   // Закрываем поток
	delete log.stream;
	log.stream = nullptr;
}