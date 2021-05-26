#ifndef rrGetOptionsH
#define rrGetOptionsH
#include "rrExporter.h"
//---------------------------------------------------------------------------
namespace rr
{
RR_DECLSPEC     extern  int                         rrOptInd;
//RR_DECLSPEC     extern  int                         opterr;
RR_DECLSPEC     extern  char                       *rrOptArg;
RR_DECLSPEC             int   GetOptions(int argc, char *argv[], const char* optstring);
}

#endif
