#pragma once
#include "IT.h"
#include "LT.h"
#include "Parm.h"
#include "Log.h"

namespace LA {
	char Tokenize(const char* string);
	short Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, Parm::PARM& parm, Log::LOG& log);
	void WriteDataForFunctions(LT::LexTable& lextable, IT::IdTable& idtable);
	void ShowIDtable(IT::IdTable& idtable, std::ofstream* outfile);
	void ShowLexTable(LT::LexTable& lextable, std::ofstream* outfile);
	
}