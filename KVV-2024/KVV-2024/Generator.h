#pragma once
#include "pch.h"
#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + string(x) + " ------------\n"


#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"includelib libucrt.lib\n"\
"includelib kernel32.lib\n"\
"includelib ..\\Debug\\FuncLib.lib\n"\
"ExitProcess PROTO:DWORD \n"\
"EXTRN random: PROC \n"\
"EXTRN toPow: PROC\n"\
"EXTRN writestr: PROC\n"\
"EXTRN writenum: PROC\n"\
"EXTRN writenumline: PROC\n"\
"EXTRN writestrline: PROC\n"\
"EXTRN system_pause:PROC\n"\
".stack 4096\n"

//"EXTRN strLength: PROC\n"

				

#define ITENTRY(x)  idtable.table[lextable.table[x].idxTI]
#define LEXEMA(x)   lextable.table[x].lexeme




namespace Gener
{
	bool CodeGeneration(LT::LexTable& lextable, IT::IdTable& idtable);
	int GenExpHandler(std::ofstream& AsmFile, LT::LexTable& LEXTABLE, IT::IdTable& idtable,int pos);
	IT::IDDATATYPE DetectType(LT::LexTable lextable, IT::IdTable idtable, int i);
	void LogicOperations(std::ofstream& AsmFile, char lex, int sn);
	void LogicOperationsForElse(std::ofstream& AsmFile, char lex, int sn);
	void LogicOperationsForWhile(std::ofstream& AsmFile, char lex, int sn);
};