#pragma once
#include "pch.h"

namespace SeAn
{
	
	

	short CheckingReturnInMain(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile);
	short CheckParamsOfFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile);
	short CheckParamsStdFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile);
	
	short Types(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile);
	short CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile);
	
	bool FindSTD(unsigned char lex);
}