#ifndef __custom_main_h__
#define __custom_main_h__

#include "../core/PhysiCell.h"

inline double current_value( double min, double max, double percent )
{ return (min + (max-min) * percent); };

static const double EPSILON = std::numeric_limits<double>::epsilon();

/** \brief Relative difference between two numbers */
inline double relative_diff( double a, double b )
{ if ( b < EPSILON ) return 0; return ( fabs(a-b)/b ); };


/* Change the current value of the input coefficient, increase or decrease according to up value */
double evolve_coef( int up, double coef, double dt );
double get_threshold( std::string field);
#endif