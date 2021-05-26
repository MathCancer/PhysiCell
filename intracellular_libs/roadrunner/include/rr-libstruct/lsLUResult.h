#ifndef lsLUResultH
#define lsLUResultH
#include "lsExporter.h"
#include "lsMatrix.h"
//---------------------------------------------------------------------------

namespace ls
{

/*! \class ls::LU_Result
\brief LUResult is intended to hold the return values of the Clapack LU decomposition methods

This class will hold the result from the methods ls::LibLA::getLU and
ls::LibLA::getLUwithFullPivoting.
*/

class LIB_EXTERN LU_Result
{
	public:
        /*! \brief  Info represents status information about the LU factorization its value is to be interpreted as:

        \li 0: successful exit
        \li < 0: if INFO = -i, the i-th argument had an illegal value
        \li > 0: if INFO = i, U(i,i) is exactly zero. The factorization has been completed,
        but the factor U is exactly singular, and division by zero will occur if it is used to solve a system of equations.

        */
        int nInfo;

        //! \brief L is a lower triangular matrix
        DoubleMatrix* L;
        //! \brief U is an upper triangular matrix
        DoubleMatrix* U;
        //! \brief P is a permutation matrix representing row permutations
        IntMatrix* P;
        /*! \brief Q is a permutation matrix representing column permutations.
        \remarks and is only available after a call to ls::LibLA::getLUwithFullPivoting and NULL otherwise
        */
        IntMatrix* Q;

        //! Constructor of a new result object (all result variables are NULL)
        LU_Result();

        //! Destructor deletes all non-NULL matrices
        ~LU_Result();
};
}//namespace ls

#endif
