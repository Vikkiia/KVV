#include "pch.h"
#include "Error.h"
#include "FST.h"
#include "Greibach.h"
#include "In.h"
#include "IT.h"
#include "Lexer.h"
#include "Log.h"
#include "LT.h"
#include "MFST.h"
#include "Parm.h"
#include "PolishNotation.h"



int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");
	
	Log::LOG log = Log::INITLOG;

	try {

		short lexErrors = 0;
		short semanticErrors = 0;
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in, parm.out);
		Log::WriteIn(log, in);
		LT::LexTable lextable = LT::Create(in.size);
		//LT::GetEntry(lextable, 128738147212371283);//122
		IT::IdTable idtable = IT::Create(in.size);
		/*IT::GetEntry(idtable, 128738147212371283);*///128

		lexErrors = LA::Scan(lextable, idtable, in, parm, log);

		if (lexErrors == 0)
		{
			*log.stream << "Лексический анализ выполнен без ошибок\n";
		}
		else
		{
			*log.stream << "Лексический анализ выполнен c ошибками\n";
		}
	
		MFST::Mfst mfst(lextable, GRB::getGreibach(),parm.out);
		LA::ShowIDtable(idtable, &mfst.outfile);
		LA::ShowLexTable(lextable, &mfst.outfile);
		MFST_TRACE_START(mfst.outfile);
		bool syntaxAnaliz = mfst.start(*log.stream);
		mfst.outfile << "\nИспользованные правила:\n\n";
		mfst.printrules();
		if (!syntaxAnaliz)
		{
			*log.stream << "Синтаксический анализ выполнен с ошибками\n";
			//return 1;
		}
		mfst.outfile << "\nСинтаксический анализ окончен\n";
		semanticErrors += SeAn::CheckingReturnInMain(lextable,idtable, log);
		semanticErrors += SeAn::CheckReturnInUserFunc(lextable, idtable, log);
		semanticErrors += SeAn::CheckParamsOfFunc(lextable, idtable,log);
		semanticErrors += SeAn::CheckParamsStdFunc(lextable, idtable,log);
		semanticErrors += SeAn::Types(lextable, idtable,log);

		if (semanticErrors == 0)
		{
			*log.stream << "Семантический анализ выполнен без ошибок\n";
		}
		else
		{
			*log.stream << "Семантический анализ выполнен c ошибками\n";
		}

		if ((lexErrors + semanticErrors) != 0)
		{
			*log.stream << "Генерация кода невозможна: были допущены ошибки на предыдущих этапах компилятора\n";
			return 1;
		}
		else {
			*log.stream << "Генерация кода выполнена успешно\n";
		}
		PN::PolishNotation(lextable, idtable, &mfst.outfile);
		LA::ShowLexTable(lextable, &mfst.outfile);
		Gener::CodeGeneration(lextable, idtable);
		mfst.outfile.close();
		LT::Delete(lextable);
		IT::Delete(idtable);
		Log::Close(log);
	}
	catch (Error::ERROR e) {

		Log::WriteError(log, e);
	}

	system("PAUSE");
	return 0;
}