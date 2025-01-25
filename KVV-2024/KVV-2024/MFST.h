#pragma once
#include <stack>
#include "Greibach.h"
#include "LT.h"
#include "Error.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define TRACE

#ifdef TRACE

#define MFST_TRACE_START(outfile) outfile << std::setw(4)<<std::left<<"\nШаг"<<": " \
	<< std::setw(30)	 << std::left << "Правило"  \
	<< std::setw(30) << std::left << "Входная лента" \
	<< std::setw(20) << std::left << "Стек" \
	<< std::endl;

#define MFST_TRACE1(outfile) outfile <<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << rule.getCRule(nrulechain)  \
	<< std::setw(30) << std::left << getCTape(tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< std::endl;

#define MFST_TRACE2(outfile)    outfile <<std::setw(4)<<std::left<<FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << " "  \
	<< std::setw(30) << std::left << getCTape(tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< std::endl;

#define MFST_TRACE3(outfile)     outfile<<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
	<< std::setw(30) << std::left << " "  \
	<< std::setw(30) << std::left << getCTape(tape_position) \
	<< std::setw(20) << std::left << getCSt() \
	<< std::endl;

#define MFST_TRACE4(c, outfile) outfile<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(c, outfile) outfile<<std::setw(4)<<std::left<<  FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE6(c,k,outfile) outfile<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<std::endl;

#define MFST_TRACE7(outfile)  outfile<<std::setw(4)<<std::left<<state.tape_position<<": "\
	<< std::setw(20) << std::left << rule.getCRule(state.nrulechain) \
	<< std::endl;

#else

#define MFST_TRACE_START ;
#define MFST_TRACE1 ;
#define MFST_TRACE2 ;
#define MFST_TRACE3 ;
#define MFST_TRACE4(c) ;
#define MFST_TRACE5(c) ;
#define MFST_TRACE6(c,k) ;
#define MFST_TRACE7 ;

#endif // TRACE

template<typename T>
struct use_container : T
{
	using T::T;
	using T::c;
};

typedef use_container<std::stack<short>> MFSTSTACK;

namespace MFST {
	struct MfstState {
		short tape_position;
		short nrule;
		short nrulechain;
		MFSTSTACK st;

		MfstState();
		MfstState(short position, MFSTSTACK st, short nrulechain);
		MfstState(short position, MFSTSTACK st, short nrule, short nrulechain);
	};

	struct Mfst {
		enum class RC_STEP {
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			TAPE_END,
			SURPRISE
		};
		std::ofstream outfile;
		struct MfstDiagnosis {
			short tape_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;

			MfstDiagnosis();
			MfstDiagnosis(short tape_position, RC_STEP rc_step, short nrule, short nrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* tape;
		short tape_position;
		short nrule;
		short nrulechain;
		short tape_size;
		GRB::Greibach greibach;
		LT::LexTable lex;
		MFSTSTACK st;
		use_container<std::stack<MfstState>> storestate;

		Mfst();
		Mfst(LT::LexTable lextable, GRB::Greibach greibach, wchar_t outfile[]);

		std::string getCSt();
		std::string getCTape(short pos, short n = 25);
		std::string getDiagnosis(short n);
		bool save_state();
		bool restore_state();
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step();
		bool start(std::ostream& outputStream);
		bool savediagnosis(RC_STEP rc_step);
		void printrules();
		struct Dedication {
			short size;
			short* nrules;
			short* nrulechains;

			Dedication();
		} dedication;
		bool save_dedication();
	};
}