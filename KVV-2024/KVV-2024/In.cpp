#include "pch.h"
#include "In.h"
#include "Error.h"
#include "IT.h"

In::IN In::getin(wchar_t infile[], wchar_t outfile[]) {
	std::ifstream inputFile(infile, std::ifstream::ate);
	std::ofstream outputFile(outfile);
	size_t fileSize = (size_t)inputFile.tellg();// Получаем размер файла.
	IN input;// Создаем объект типа IN для хранения результата обработки.
	int writingPosition = 0; 

	if (!inputFile.is_open())
		throw ERROR_THROW(110);

	if (!outputFile.is_open())
		throw ERROR_THROW(110);

	inputFile.seekg(std::ios::beg);

	input.text = DBG_NEW unsigned char[fileSize + 1500];// Выделяем память для хранения текста с запасом в 1500 байт.

	for (int column = 0, readChar = 0; readChar != EOF;) {
		readChar = inputFile.get();
		input.size++;

		if (input.code[readChar] == IN::Q) {
			do {
				input.text[writingPosition] = readChar;// Копируем символ в текст.
				writingPosition++;
				column++;
				readChar = inputFile.get(); // Считываем следующий символ.
				input.size++;
			} while (input.code[readChar] != IN::Q && column < TI_STR_MAXSIZE); 
		}

		if (input.code[readChar] == IN::O) {
			if (input.code[input.text[writingPosition - 1]] == IN::S) { 
				writingPosition--;// Уменьшаем позицию записи (удаляем пробел).
				input.ignor++;
			}
		}

		if (readChar == IN_CODE_ENDL || readChar == EOF) {
			input.lines++;
			if (input.text[writingPosition - 1] != IN_CODE_ENDL) {
				input.text[writingPosition] = IN_CODE_DELIM;
				writingPosition++;
				if (readChar != EOF) {
					input.text[writingPosition] = IN_CODE_ENDL;
					writingPosition++;
				}
				column = 0;// Сбрасываем колонку.
			}
			else {
				input.ignor++;
			}
			continue;// Переходим к следующей итерации.
		}
		else if (input.code[readChar] == IN::S) {
			if (input.code[input.text[writingPosition - 1]] != IN::S && input.code[input.text[writingPosition - 1]] != IN::O) {
				input.text[writingPosition] = readChar;
				writingPosition++;
			}
			else {
				input.ignor++;
			}
		}
		else if (input.code[readChar] == IN::F) {
			throw ERROR_THROW_IN(111, input.lines, column);
		}
		else if (input.code[readChar] == IN::I) {
			input.ignor++;
		}
		else if (input.code[readChar] == IN::T || input.code[readChar] == IN::O || input.code[readChar] == IN::Q) {
			input.text[writingPosition] = readChar;
			writingPosition++;
		}
		else {
			input.text[writingPosition] = input.code[readChar];// Преобразуем символ и записываем в текст.
			writingPosition++;
		}
		column++;
	}
	input.text[writingPosition] = '\0';// Завершаем строку нулевым символом.
	input.size--;// Уменьшаем общий размер на 1 (EOF не учитывается).
	inputFile.close();
	outputFile << input.text;
	outputFile.close();
	return input; 
}

In::IN::IN()
	:size(0), lines(0), ignor(0), text(nullptr), code IN_CODE_TABLE
{	}
// Конструктор копирования.
//Копирует размер (size) из объекта other в новый объект. Это может быть, например, количество символов в тексте.
In::IN::IN(const IN& other)
	: size(other.size), lines(other.lines), ignor(other.ignor), code IN_CODE_TABLE
{
	text = DBG_NEW unsigned char[strlen((const char*)other.text) + 1];// Выделяем память для текста. Размер выделяемой памяти равен длине строки other.text плюс один дополнительный байт для нулевого символа \0.
	std::copy(other.text, other.text + strlen((const char*)other.text) + 1, text); // Копируем текст.
}

In::IN::~IN() {
	delete[] text;// Освобождаем выделенную память для текста.
	text = nullptr;
}