#pragma once
#define LT_MAXSIZE				4096//таблица ликсем
#define LT_TI_NULLIDX			((int)0xffffffff)
#define LEX_STRING				's'//строковый

#define LEX_UINT				'U'//беззнаковый

#define LEX_DATATYPE			't'
#define LEX_ID					'i'
#define LEX_INTEGER_LITERAL		'1'
#define LEX_STRING_LITERAL		'2'

#define LEX_IF					'u'

#define LEX_POW					'P' //функция возведения в степень
#define LEX_RAND				'R' //функция генерация случайных чисел
#define LEX_MOD					'%' //получение остатка от деления
//#define LEX_LENGTH				'L' //функция возвращает длину строки


#define LEX_LITERAL				'l'
#define LEX_FUNCTION			'f'
#define LEX_DECLARE				'd'
#define LEX_RETURN				'r'
#define LEX_PRINT				'p'
#define LEX_MAIN				'm'
#define LEX_SEMICOLON			';'
#define LEX_COMMA				','
#define LEX_LEFTBRACE			'{'
#define LEX_BRACELET			'}'
#define LEX_LEFTHESIS			'('
#define LEX_RIGHTHESIS			')'

#define LEX_PLUS				'+'
#define LEX_MINUS				'-'
#define LEX_STAR				'*'
#define LEX_DIRSLASH			'/'
#define LEX_EQUALS				'='

#define LEX_BIGGERTHEN          '>'	
#define LEX_SMALLERTHEN         '<'
#define LEX_NOTEQUAL            '!'
#define LEX_SMALLEROREQUAL      '$'
#define LEX_BIGGEROREQUAL       '`'
#define LEX_EQUAL_ID            '~'    

#define LEX_NEWLINE				'|'


#define LEX_RIGHT_SQUARE_BRACE	']'
#define LEX_LEFT_SQUARE_BRACE	'['
#define LEX_ELSE				'E'
//
namespace LT {
	struct Entry {
		char lexeme;
		int sn;
		int idxTI;
	};

	struct LexTable {
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
}