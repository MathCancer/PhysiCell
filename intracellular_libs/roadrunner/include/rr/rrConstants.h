#ifndef rrConstantsH
#define rrConstantsH
#include <limits>
#include <string>
#include <cstdlib>
#include "rrExporter.h"

typedef unsigned int    u_int;

namespace rr
{

//Useful constants...
RR_DECLSPEC extern const char     gPathSeparator;
RR_DECLSPEC extern const std::string gExeSuffix;

RR_DECLSPEC extern const char*             gDoubleFormat;
RR_DECLSPEC extern const char*             gIntFormat;
RR_DECLSPEC extern const char*             gComma;
RR_DECLSPEC extern const std::string     gNoneString;
RR_DECLSPEC extern const std::string    gEmptyString;

//Messages
RR_DECLSPEC extern const std::string gEmptyModelMessage;


// Constants
RR_DECLSPEC extern const char     gTab;
RR_DECLSPEC extern const char     gNL;
RR_DECLSPEC extern const double    gDoubleNaN;
RR_DECLSPEC extern const float  gFloatNaN;
RR_DECLSPEC extern const int    gMaxPath;

// Enums...
enum SBMLType {stCompartment = 0, stSpecies, stParameter};    //Species clashes with class Species, prefix enums with st, for SbmlType

}
#endif
