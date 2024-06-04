#include "custom_main.h"

using namespace PhysiCell;

/* Change the current value of the input coefficient, increase or decrease according to up value */
double evolve_coef( int up, double coef, double dt )
/**{ 
	// increase exponentially
	if ( up )
	{
		if ( (*coef) < EPSILON ) 
			(*coef) = EPSILON; 	
		(*coef) = std::sqrt( (*coef) );
		(*coef) = (*coef) > 1 ? (1-EPSILON) : (*coef);
	}
	else
	{
		// decrease exponentially
		if ( (*coef) >= 1 )
			(*coef) = 1 - EPSILON;
		(*coef) *= (*coef);	
		(*coef) = (*coef) < 0 ? EPSILON : (*coef);
	}
}*/
{ 
	// if up, increase, else decrease
	if ( !up )
		dt = -dt;

	(coef) +=  (coef) * (1 - coef) * dt/200.0 ;

	(coef) = (coef) >= 1 ? (1-EPSILON) : (coef);
	(coef) = (coef) <= 0 ? (EPSILON) : (coef);

	return coef;
}

double get_threshold( std::string field)
{
	double pth = PhysiCell::parameters.doubles(field+"_threshold");
	return pth;
}