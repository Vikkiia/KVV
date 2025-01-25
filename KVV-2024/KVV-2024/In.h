#pragma once

#define IN_MAX_LEN_TEXT 1024*1024
#define IN_CODE_ENDL '\n'
#define IN_CODE_DELIM '|'

#define IN_CODE_TABLE {\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::S, IN::T, IN::F, IN::F, IN::I, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::S, IN::O, IN::F, IN::F, IN::F, IN::O, IN::F, IN::Q, IN::O, IN::O, IN::O, IN::O, IN::O, IN::O, IN::T, IN::O, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::O, IN::O, IN::O, IN::O, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::T, IN::F, IN::F, \
	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::O, IN::F, IN::O, IN::O, IN::F, \
																													\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::O, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::O, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
}

namespace In {
	struct IN {
		// T - допустимый символ, F - недопустимый, I - игнорировать, O - операция, Q - ', S - whitespace, иначе - заменить
		enum { T = 1024, F = 2048, I = 4096, O = 8192, Q = 16384, S = 32768 };
		int size;
		int lines;
		int ignor;
		unsigned char* text;
		int code[256];

		IN();
		IN(const IN& other);
		~IN();
	};
	IN getin(wchar_t infile[], wchar_t outfile[]);
}