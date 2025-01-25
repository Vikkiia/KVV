#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <sstream>

#include "Log.h"
#include "Error.h"
#include "Parm.h"
#include "LT.h"
#include "IT.h"
#include "In.h"
#include "FST.h"

#include "Lexer.h"
#include "Greibach.h"
#include "MFST.h"
#include "PolishNotation.h"
#include "Generator.h"
#include "SematicAn.h"
