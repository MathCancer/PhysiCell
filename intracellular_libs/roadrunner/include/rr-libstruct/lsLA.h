//---------------------------------------------------------------------------
#ifndef lsLAH
#define lsLAH
//---------------------------------------------------------------------------
#include "lsExporter.h"
#include "lsMatrix.h"

namespace ls
{

//    /// <summary>
//    /// <para>The LA class represents the entry point to all functions provided by
//    /// the C-library. These functions include a variety of useful functionality
//    /// operating on double and complex matrices. </para>
//    /// <para>The current scope of the library encompasses matrix factorizations (QR and LU
//    /// factorization) as well as commonly needed matrix operations, such as calculating
//    /// the inverse of a matrix, computing eigen values and singular values as well as
//    /// the null space of a matrix (both left and right null space) along with a method
//    /// for the computation of the row echelon or Gauss Jordan form of a matrix.</para>
//    /// </summary>
//    public static class LA
//    {
//        /// <summary>
//        /// The library file to where the members will be found. This will be
//        /// libstructural.dll on windows, and libLibStructural.so on linux and
//        /// OSX under mono.
//        /// </summary>
//        internal const string LIBRARY_FILE = "LibStructural";
//
//        /// <summary>
//        /// Passes the pointer 'matrix' to the library to have it freed.
//        /// </summary>
//        /// <param name="matrix">the 2d pointer to be freed</param>
//        /// <param name="numRows">the number of rows in this element</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_freeMatrix")]
//        internal static extern void FreeMatrix(System.IntPtr matrix, int numRows);
//
//        /// <summary>
//        /// passes the pointer 'vector' to the library to have it freed.
//        /// </summary>
//        /// <param name="vector">the vector to be freed</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_freeVector")]
//        internal static extern void FreeVector(System.IntPtr vector);
//
//        /// <summary>
//        /// Calculates the eigen-values of the given real matrix.
//        /// </summary>
//        /// <param name="oMatrix">a square real matrix</param>
//        public static Complex[] GetEigenValues(double[][] oMatrix)
//
//        /// <summary>
//        /// Calculates the eigen-values of a square complex matrix that has been split up
//        /// into real and complex parts.
//        /// </summary>
//        /// <param name="oMatrixReal">real part of the complex matrix</param>
//        /// <param name="oMatrixImag">imaginary part of the complex matrix</param>
//        public static Complex[] GetEigenValues(double[][] oMatrixReal, double[][] oMatrixImag)
//        /// <summary>
//        /// Calculates the eigen-values of a square complex matrix.
//        /// </summary>
//        /// <param name="oMatrix">the square complex matrix</param>
//        public static Complex[] GetEigenValues(Complex[][] oMatrix)
//        /// <summary>
//        /// <para>This method calculates the fully pivoted Gauss Jordan Form of the given matrix.
//        /// Fully pivoted here means, that row as well as column swaps will be used. These
//        /// permutations have been captured in the integer vectors rowPivots and colPivots.</para>
//        ///
//        /// <para>If no permutations have occurred those vectors will be in ascending form [ 0, 1, 2, 3 ];
//        /// However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];</para>
//        ///
//        /// </summary>
//        /// <param name="oMatrix">the matrix to be brought into (fully pivoted) Gauss Jordan Form</param>
//        /// <param name="rowPivots">a vector of row permutations</param>
//        /// <param name="colPivots">a vector of column permutations</param>
//        public static double[][] GetFullyPivotedGaussJordan(double[][] oMatrix, out int[] rowPivots, out int[] colPivots)
//        /// <summary>
//        /// This method calculates the fully pivoted Gauss Jordan Form of the given matrix.
//        /// Fully pivoted here means, that row as well as column swaps will be used. These
//        /// permutations will not be returned by this method.
//        ///
//        /// </summary>
//        /// <param name="oMatrix">the matrix to be brought into (fully pivoted) Gauss Jordan Form</param>
//        public static double[][] GetFullyPivotedGaussJordan(double[][] oMatrix)
//        /// <summary>
//        /// <para>This method calculates the Gauss Jordan or row echelon Form of the given matrix.
//        /// Only that row swaps will be used. These permutations will be returned in the 'pivots'
//        /// vector.</para>
//        ///
//        /// <para>If no permutations have occurred this vector will be in ascending form [ 0, 1, 2, 3 ];
//        /// However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];</para>
//        /// </summary>
//        /// <param name="oMatrix">the matrix to be brought into Gauss Jordan Form</param>
//        /// <param name="pivots">the vector of row permutations that have been used</param>
//        public static double[][] GetGaussJordan(double[][] oMatrix, out int[] pivots)
//        ///<summary>
//        /// This method calculates the Gauss Jordan or row echelon Form of the given matrix.
//        /// Only that row swaps will be used. These permutations will not be returned by this method.
//        ///
//        /// </summary>
//        /// <param name="oMatrix">the matrix to be brought into Gauss Jordan Form</param>
//        public static double[][] GetGaussJordan(double[][] oMatrix)
//
//        /// <summary>
//        /// <para>This function calculates the inverse of a square complex matrix. Employing the CLAPACK
//        /// functions: zgetrf and zgetri. That is first the matrix will be factorized using LU
//        /// decomposition followed by the calculation of the inverse based on:</para>
//        ///
//        /// <pre>inv(A)*L = inv(U) for inv(A).</pre>
//        ///
//        /// </summary>
//        /// <param name="oMatrix">a square complex matrix</param>
LIB_EXTERN ComplexMatrix GetInverse(const ComplexMatrix& oMatrix);
//
//        /// <summary>
//        /// Calculates the eigen-vectors of a square real matrix.
//        ///
//        /// This function calculates the complex (right) eigenvectors of the given real matrix.
//        /// The complex matrix returned contains the eigenvectors in the columns, in the same order as GetEigenValues.
//        ///
//        /// The right eigenvector v(j) of A satisfies:
//        ///
//        /// <pre>A * v(j) = lambda(j) * v(j) </pre>
//        ///
//        /// </summary>
//        /// <param name="oMatrix">a real matrix</param>
//        /// <returns>the complex eigenvectors of the matrix</returns>
//        public static Complex[][] GetEigenVectors(double[][] oMatrix)
//        /// <summary>
//        /// Factorizes the given matrix using SVD.
//        ///
//        /// This function computes the singular value decomposition (SVD) of the given real matrix.
//        ///
//        /// The SVD is written
//        /// <pre>A = U * SIGMA * transpose(V)</pre>
//        ///
//        /// Remarks:
//        /// this function returns the transpose of V
//        ///
//        /// </summary>
//        /// <param name="oMatrix">the real matrix to be decomposed (input) (With size M by N) </param>
//        /// <param name="uMatrix">(output) will be initialized with the M by M orthogonal matrix U </param>
//        /// <param name="singVals">(output) will be initialized with the min(M,N) singular values, returned in descending order </param>
//        /// <param name="vMatrix">(output) will be initialized with the N by N orthogonal matrix V </param>
//        public static void GetSVD(double[][] oMatrix, out double[][] uMatrix, out double[] singVals, out double[][] vMatrix)
//        /// <summary>
//        /// Factorizes the given matrix using SVD.
//        ///
//        /// This function computes the singular value decomposition (SVD) of the given complex matrix.
//        ///
//        /// The SVD is written
//        /// <pre>A = U * SIGMA * conjugate-transpose (V)</pre>
//        ///
//        /// Remarks:
//        /// this function returns the conjugate-transpose of V
//        ///
//        /// </summary>
//        /// <param name="oMatrix">the complex matrix to be decomposed (input) (With size M by N) </param>
//        /// <param name="uMatrix">(output) will be initialized with the M by M orthogonal matrix U </param>
//        /// <param name="singVals">(output) will be initialized with the min(M,N) singular values, returned in descending order </param>
//        /// <param name="vMatrix">(output) will be initialized with the N by N orthogonal matrix V </param>
//        public static void GetSVD(Complex[][] oMatrix, out Complex[][] uMatrix, out double[] singVals, out Complex[][] vMatrix)
//        /// <summary>
//        /// Factorizes the given matrix using SVD.
//        ///
//        /// This function computes the singular value decomposition (SVD) of the given complex matrix (separated into its real and imaginary part).
//        ///
//        /// The SVD is written
//        /// <pre>A = U * SIGMA * conjugate-transpose (V)</pre>
//        ///
//        /// Remarks:
//        /// this function returns the conjugate-transpose of V
//        ///
//        /// </summary>
//        /// <param name="oMatrixReal">the real part of the complex matrix to be decomposed (input) (With size M by N) </param>
//        /// <param name="oMatrixImag">the imaginary part of the complex matrix to be decomposed (input) (With size M by N) </param>
//        /// <param name="uMatrix">(output) will be initialized with the M by M orthogonal matrix U </param>
//        /// <param name="singVals">(output) will be initialized with the min(M,N) singular values, returned in descending order </param>
//        /// <param name="vMatrix">(output) will be initialized with the N by N orthogonal matrix V </param>
//        public static void GetSVD(double[][] oMatrixReal, double[][] oMatrixImag, out Complex[][] uMatrix, out double[] singVals, out Complex[][] vMatrix)
//        /// <summary>
//        /// Calculates the eigen-vectors of a complex matrix (separated into real and imaginary parts).
//        ///
//        /// This function calculates the complex (right) eigenvectors of the given complex matrix.
//        /// The complex matrix returned contains the eigenvectors in the columns, in the same order as GetEigenValues.
//        ///
//        /// The right eigenvector v(j) of A satisfies:
//        ///
//        /// <pre>A * v(j) = lambda(j) * v(j) </pre>
//        ///
//        /// </summary>
//        /// <param name="oMatrix">the complex matrix</param>
//        /// <returns>the complex eigenvectors of the matrix</returns>
//        public static Complex[][] GetEigenVectors(Complex[][] oMatrix)
//        /// <summary>
//        /// Calculates the eigen-vectors of a complex matrix (separated into real and imaginary parts).
//        ///
//        /// This function calculates the complex (right) eigenvectors of the given complex matrix.
//        /// The complex matrix returned contains the eigenvectors in the columns, in the same order as GetEigenValues.
//        ///
//        /// The right eigenvector v(j) of A satisfies:
//        ///
//        /// <pre>A * v(j) = lambda(j) * v(j) </pre>
//        ///
//        /// </summary>
//        /// <param name="oMatrixReal">real part of the matrix </param>
//        /// <param name="oMatrixImag">the imaginary part of the matrix</param>
//        /// <returns>the complex eigenvectors of the matrix</returns>
//        public static Complex[][] GetEigenVectors(double[][] oMatrixReal, double[][] oMatrixImag)
//        /// <summary>
//        /// Splits the given complex matrix into its real and imaginary matrix
//        /// </summary>
//        /// <param name="oMatrix">complex matrix</param>
//        /// <param name="oReal">matrix holding the real part</param>
//        /// <param name="oImag">matrix holding the imaginary part</param>
LIB_EXTERN void SplitComplexMatrix(const ComplexMatrix& oMatrix, DoubleMatrix& oReal, DoubleMatrix& oImag);
//        /// <summary>
//        /// <para>This function calculates the inverse of a square complex matrix. Employing the CLAPACK
//        /// functions: zgetrf and zgetri. That is first the matrix will be factorized using LU
//        /// decomposition followed by the calculation of the inverse based on:</para>
//        ///
//        /// <pre>inv(A)*L = inv(U) for inv(A).</pre>
//        ///
//        /// </summary>
//        /// <param name="oMatrixImag">The imaginary part of the matrix</param>
//        /// <param name="oMatrixReal">The real part of the matrix</param>
//LIB_EXTERN ComplexMatrix GetInverse(const DoubleMatrix& oMatrixReal, const DoubleMatrix& oMatrixImag);

//        /// <summary>
//        /// <para>This function calculates the inverse of a square real matrix. Employing the CLAPACK
//        /// functions: dgetrf and dgetri. That is first the matrix will be factorized using LU
//        /// decomposition followed by the calculation of the inverse based on: </para>
//        ///
//        /// <pre>inv(A)*L = inv(U) for inv(A).</pre>
//        ///
//        /// </summary>
//        /// <param name="oMatrix">a square real matrix</param>
//        public static double[][] GetInverse(double[][] oMatrix)

//        /// <summary>
//        /// <para>This function calculates the left null space of a given real matrix. That is:</para>
//        ///
//        /// <pre>null(A)*A = 0</pre>
//        ///
//        /// <para>This function is equivalent to <seealso cref="GetRightNullSpace"/> returning
//        /// the right null space of the transposed matrix.</para>
//        /// </summary>
//        /// <param name="oMatrix">The real matrix for which to compute the left null space</param>
//        public static double[][] GetLeftNullSpace(double[][] oMatrix)
//        /// <summary>
//        /// <para>This function computes the LU factorization of the given real M-by-N matrix A
//        /// using partial pivoting with row interchanges. This procedure is carried out by the LAPACK method dgetrf.</para>
//        /// <para>A is factorized into: </para>
//        /// <pre>A = P * L * U</pre>
//        /// <para>the result is encapsulated in an LUResult object (with an empty Q matrix)</para>
//        /// </summary>
//        /// <param name="oMatrix">real M-by-N matrix</param>
//        public static LUResult GetLU(double[][] oMatrix)
//
//        /// <summary>
//        /// <para>This function computes the LU factorization of the given real N-by-N matrix A
//        /// using complete pivoting (with row and column interchanges). This procedure is carried out by the LAPACK method dgetc2.</para>
//        /// <para>A is factorized into: </para>
//        /// <pre>A = P * L * U * Q</pre>
//        /// <para>the result is encapsulated in an LUResult object</para>
//        /// </summary>
//        /// <param name="oMatrix">real N-by-N matrix</param>
//        public static LUResult GetLUWithPivoting(double[][] oMatrix)
//        /// <summary>
//        /// <para>This function computes the QR factorization of the given real M-by-N
//        /// matrix A. The LAPACK method DGEQRF is used followed by an
//        /// orthonormalization of Q through the use of DORGQR.</para>
//        /// <para>The factorized form is: </para>
//        /// <pre>A = Q * R</pre>
//        /// <para>the result is encapsulated in an QRResult object (with an empty P matrix)</para>
//        /// </summary>
//        /// <param name="oMatrix">real M-by-N matrix</param>
//        public static QRResult GetQR(double[][] oMatrix)

//        /// <summary>
//        /// <para>This function computes the QR factorization of the given real M-by-N matrix A
//        /// with column pivoting. The LAPACK method dgeqp3 is
//        /// used followed by an orthonormalization of Q through the use of DORGQR.</para>
//        /// <para>The factorized form is: </para>
//        /// <pre>A = Q * R</pre>
//        /// <para>the result is encapsulated in an QRResult object (with the P matrix returning the
//        /// pivots used)</para>
//        /// </summary>
//        /// <param name="oMatrix">real M-by-N matrix</param>
//        public static QRResult GetQRWithPivot(double[][] oMatrix)
//        /// <summary>
//        /// <para>This method computes the rank of the given matrix. For this first the
//        /// singular values of the matrix are calculated and then the rank is determined
//        /// by the number of non-zero values. </para>
//        /// <para>Note that zero here is defined as any value whose absolute value is bigger than
//        /// the set tolerance.</para>
//        /// </summary>
//        /// <param name="oMatrix">a real matrix</param>
//        public static int GetRank(double[][] oMatrix)
//        /// <summary>
//        /// returns an estimate for the reciprocal of the condition of A in 1-norm using the LAPACK condition estimator.
//        /// If A is well conditioned, getRCond(A) is near 1.0. If A is badly conditioned, getRCond(A) is near 0.0.
//        /// </summary>
//        /// <param name="oMatrix">a real matrix</param>
//        /// <returns>reciprocal condition number estimate</returns>
//        public static double GetRCond(double[][] oMatrix)
//        /// <summary>
//        /// <para>This function calculates the right null space of a given real matrix. That is:</para>
//        ///
//        /// <pre>A*null(A) = 0</pre>
//        ///
//        /// <para>In order to calculate the (right) null space, we first calculate the full
//        /// singular value decomposition (employing dgesdd) of the matrix: </para>
//        ///
//        /// <pre>[U,S,V] = svd(A');</pre>
//        ///
//        /// <para>then calculate the rank: </para>
//        ///
//        /// <pre>r = rank(A)</pre>
//        ///
//        /// <para>and finally return the last columns of the U matrix (r+1...n) as the null space
//        /// matrix.</para>
//        /// </summary>
//        /// <param name="oMatrix">The real matrix for which to compute the right null space</param>
//        public static double[][] GetRightNullSpace(double[][] oMatrix)
//        /// <summary>
//        /// <para>This function calculates the left null space of a given real matrix. That is:</para>
//        ///
//        /// <pre>null(A)*A = 0</pre>
//        ///
//        /// <para>This function is equivalent to <seealso cref="GetRightNullSpace"/> returning
//        /// the right null space of the transposed matrix.</para>
//        /// </summary>
//        /// <param name="oMatrix">The real matrix for which to compute the left null space</param>
//        public static double[][] GetScaledLeftNullSpace(double[][] oMatrix)
//        /// <summary>
//        /// <para>This function calculates the right null space of a given real matrix. That is:</para>
//        ///
//        /// <pre>A*null(A) = 0</pre>
//        ///
//        /// <para>In order to calculate the (right) null space, we first calculate the full
//        /// singular value decomposition (employing dgesdd) of the matrix: </para>
//        ///
//        /// <pre>[U,S,V] = svd(A');</pre>
//        ///
//        /// <para>then calculate the rank: </para>
//        ///
//        /// <pre>r = rank(A)</pre>
//        ///
//        /// <para>and finally return the last columns of the U matrix (r+1...n) as the null space
//        /// matrix.</para>
//        /// <remarks>This method also scaled the right null space using gauss jorden</remarks>
//        /// </summary>
//        /// <param name="oMatrix">The real matrix for which to compute the right null space</param>
//        public static double[][] GetScaledRightNullSpace(double[][] oMatrix)
//        /// <summary>
//        /// This method performs the Singular Value Decomposition of the given real matrix,
//        /// returning only the singular values. For this the LAPACK method: dgesdd was used.
//        /// </summary>
//        /// <param name="oMatrix">a real matrix</param>
//        public static double[] GetSingularValues(double[][] oMatrix)
//        /// <summary>
//        /// This function returns the tolerance currently used by the library to determine what value
//        /// is seen as zero. (Any value with absolute value smaller than this tolerance is seen as zero
//        /// and will be neglected).
//        /// </summary>
//        /// <returns>The tolerance used by the CLAPACK library to determine a
//        /// value close to zero.
//        /// </returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getTolerance")]
//        public static extern double GetTolerance();
//
//        //[DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_gaussJordan2")]
//        //private static extern int LibLA_gaussJordan2(System.IntPtr inMatrix, int numRows, int numCols,
//        //    out System.IntPtr outMatrix, out int outRows, out int outCols);
//
//        //public static double[][] GetGaussJordan2(double[][] oMatrix)
//
//        /// <summary>
//        /// Lib clapack _fully pivoted gauss jordan
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        /// <param name="outRowPivot">Out row pivot</param>
//        /// <param name="nRowLength">N row length</param>
//        /// <param name="outColPivot">Out color pivot</param>
//        /// <param name="nColLength">N color length</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_fullyPivotedGaussJordan")]
//        private static extern int LibLA_fullyPivotedGaussJordan(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols,
//            out System.IntPtr outRowPivot, out int nRowLength,
//            out System.IntPtr outColPivot, out int nColLength);
//
//
//
//        /// <summary>
//        /// Lib clapack _gauss jordan
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        /// <param name="outPivot">Out pivot</param>
//        /// <param name="nLength">N length</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_gaussJordan")]
//        private static extern int LibLA_gaussJordan(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols, out System.IntPtr outPivot, out int nLength);
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getEigenValues")]
//        private static extern int LibLA_getEigenValues(System.IntPtr inMatrix,
//            int numRows,
//            int numCols,
//            out System.IntPtr outReal,
//            out System.IntPtr outImag,
//            out int outLength);
//
//
//        /// Return Type: int
//        ///inMatrix: double**
//        ///numRows: int
//        ///numCols: int
//        ///outL: double***
//        ///outLRows: int*
//        ///outLCols: int*
//        ///outU: double***
//        ///outURows: int*
//        ///outUCols: int*
//        ///outP: int***
//        ///outPRows: int*
//        ///outPCols: int*
//        ///info: int*
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outL">Out l</param>
//        /// <param name="outLRows">Out l rows</param>
//        /// <param name="outLCols">Out l cols</param>
//        /// <param name="outU">Ou texture coordinatet u texture coordinate</param>
//        /// <param name="outURows">Ou texture coordinatet u texture coordinate rows</param>
//        /// <param name="outUCols">Ou texture coordinatet u texture coordinate cols</param>
//        /// <param name="outP">Out p</param>
//        /// <param name="outPRows">Out p rows</param>
//        /// <param name="outPCols">Out p cols</param>
//        /// <param name="info">Info</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getLU")]
//        private static extern int LibLA_getLU(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outL, out int outLRows, out int outLCols,
//            out System.IntPtr outU, out int outURows, out int outUCols,
//            out System.IntPtr outP, out int outPRows, out int outPCols, out int info);
//
//
//        /// Return Type: int
//        ///inMatrix: double**
//        ///numRows: int
//        ///numCols: int
//        ///outL: double***
//        ///outLRows: int*
//        ///outLCols: int*
//        ///outU: double***
//        ///outURows: int*
//        ///outUCols: int*
//        ///outP: int***
//        ///outPRows: int*
//        ///outPCols: int*
//        ///outQ: int***
//        ///outQRows: int*
//        ///outQCols: int*
//        ///info: int*
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outL">Out l</param>
//        /// <param name="outLRows">Out l rows</param>
//        /// <param name="outLCols">Out l cols</param>
//        /// <param name="outU">Ou texture coordinatet u texture coordinate</param>
//        /// <param name="outURows">Ou texture coordinatet u texture coordinate rows</param>
//        /// <param name="outUCols">Ou texture coordinatet u texture coordinate cols</param>
//        /// <param name="outP">Out p</param>
//        /// <param name="outPRows">Out p rows</param>
//        /// <param name="outPCols">Out p cols</param>
//        /// <param name="outQ">Out q</param>
//        /// <param name="outQRows">Out q rows</param>
//        /// <param name="outQCols">Out q cols</param>
//        /// <param name="info">Info</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getLUwithFullPivoting")]
//        private static extern int LibLA_getLUwithFullPivoting(
//            System.IntPtr inMatrix,  int numRows, int numCols,
//            out System.IntPtr outL, out int outLRows, out int outLCols,
//            out System.IntPtr outU, out int outURows, out int outUCols,
//            out System.IntPtr outP, out int outPRows, out int outPCols,
//            out System.IntPtr outQ, out int outQRows, out int outQCols,
//            out int info);
//        /// Return Type: int
//        ///inMatrix: double**
//        ///numRows: int
//        ///numCols: int
//        ///outQ: double***
//        ///outQRows: int*
//        ///outQCols: int*
//        ///outR: double***
//        ///outRRows: int*
//        ///outRCols: int*
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outQ">Out q</param>
//        /// <param name="outQRows">Out q rows</param>
//        /// <param name="outQCols">Out q cols</param>
//        /// <param name="outR">Out r</param>
//        /// <param name="outRRows">Out r rows</param>
//        /// <param name="outRCols">Out r cols</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getQR")]
//        private static extern int LibLA_getQR(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outQ, out int outQRows, out int outQCols,
//            out System.IntPtr outR, out int outRRows, out int outRCols);
//        /// Return Type: int
//        ///inMatrix: double**
//        ///numRows: int
//        ///numCols: int
//        ///outQ: double***
//        ///outQRows: int*
//        ///outQCols: int*
//        ///outR: double***
//        ///outRRows: int*
//        ///outRCols: int*
//        ///outP: double***
//        ///outPRows: int*
//        ///outPCols: int*
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outQ">Out q</param>
//        /// <param name="outQRows">Out q rows</param>
//        /// <param name="outQCols">Out q cols</param>
//        /// <param name="outR">Out r</param>
//        /// <param name="outRRows">Out r rows</param>
//        /// <param name="outRCols">Out r cols</param>
//        /// <param name="outP">Out p</param>
//        /// <param name="outPRows">Out p rows</param>
//        /// <param name="outPCols">Out p cols</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getQRWithPivot")]
//        private static extern int LibLA_getQRWithPivot(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outQ, out int outQRows, out int outQCols,
//            out System.IntPtr outR, out int outRRows, out int outRCols,
//            out System.IntPtr outP, out int outPRows, out int outPCols);
//
//
//        /// <summary>
//        /// Lib clapack _get rank
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getRank")]
//        private static extern int LibLA_getRank(System.IntPtr inMatrix, int numRows, int numCols);
//
//
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getRCond")]
//        private static extern double LibLA_getRCond(System.IntPtr inMatrix, int numRows, int numCols);
//
//        /// Return Type: int
//        ///inMatrix: double**
//        ///numRows: int
//        ///numCols: int
//        ///outSingularVals: double**
//        ///outLength: int*
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outSingularVals">Out singular vals</param>
//        /// <param name="outLength">Out length</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getSingularValsBySVD")]
//        private static extern int LibLA_getSingularValsBySVD(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outSingularVals, out int outLength);
//
//
//
//
//        /// Return Type: int
//        ///inMatrix: double**
//        ///numRows: int
//        ///numCols: int
//        ///outMatrix: double***
//        ///outRows: int*
//        ///outCols: int*
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_inverse")]
//        private static extern int LibLA_inverse(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols);
//        /// <summary>
//        /// left null space
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_leftNullspace")]
//        private static extern int LibLA_leftNullspace(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols);
//
//        /// <summary>
//        /// right null space
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_rightNullspace")]
//        private static extern int LibLA_rightNullspace(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols);
//
//
//        /// <summary>
//        /// get scaled left null space
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_scaledLeftNullspace")]
//        private static extern int LibLA_scaledLeftNullspace(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols);
//
//        /// <summary>
//        /// get scaled right null space
//        /// </summary>
//        /// <param name="inMatrix">In matrix</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrix">Out matrix</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_scaledRightNullspace")]
//        private static extern int LibLA_scaledRightNullspace(System.IntPtr inMatrix, int numRows, int numCols,
//            out System.IntPtr outMatrix, out int outRows, out int outCols);
//
//
//
//        /// Return Type: int
//        ///inMatrixReal: double**
//        ///inMatrixImag: double**
//        ///numRows: int
//        ///numCols: int
//        ///outReal: double**
//        ///outImag: double**
//        ///outLength: int*
//        /// <param name="inMatrixReal">In matrix real</param>
//        /// <param name="inMatrixImag">In matrix imag</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outReal">Out real</param>
//        /// <param name="outImag">Out imag</param>
//        /// <param name="outLength">Out length</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_ZgetEigenValues")]
//        private static extern int LibLA_ZgetEigenValues(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
//            out System.IntPtr outReal, out System.IntPtr outImag, out int outLength);
//        /// Return Type: int
//        ///inMatrixReal: double**
//        ///inMatrixImag: double**
//        ///numRows: int
//        ///numCols: int
//        ///outMatrixReal: double***
//        ///outMatrixImag: double**
//        ///outRows: int*
//        ///outCols: int*
//        /// <param name="inMatrixReal">In matrix real</param>
//        /// <param name="inMatrixImag">In matrix imag</param>
//        /// <param name="numRows">Number rows</param>
//        /// <param name="numCols">Number cols</param>
//        /// <param name="outMatrixReal">Out matrix real</param>
//        /// <param name="outMatrixImag">Out matrix imag</param>
//        /// <param name="outRows">Out rows</param>
//        /// <param name="outCols">Out cols</param>
//        /// <returns>Int</returns>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_Zinverse")]
//        private static extern int LibLA_Zinverse(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
//            out System.IntPtr outMatrixReal, out System.IntPtr outMatrixImag, out int outRows, out int outCols);
//
//
//        /// <summary>
//        /// This function sets the tolerance used by the library to determine what value
//        /// is seen as zero. (Any value with absolute value smaller than this tolerance is seen as zero
//        /// and will be neglected).
//        /// </summary>
//        /// <param name="value">Sets the tolerance used by the CLAPACK library to determine a
//        /// value close to zero.</param>
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_setTolerance")]
//        public static extern void SetTolerance(double value);
//
//
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getEigenVectors")]
//        private static extern int LibLA_getEigenVectors(System.IntPtr inMatrix, int numRows, int numCols,
//                                     out System.IntPtr outMatrixReal, out System.IntPtr outMatrixImag, out int outRows, out int outCols);
//
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_ZgetEigenVectors")]
//        private static extern int LibLA_ZgetEigenVectors(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
//                                       out System.IntPtr outMatrixReal, out System.IntPtr outMatrixImag, out int outRows, out int outCols);
//
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getSVD")]
//        private static extern int LibLA_getSVD(System.IntPtr inMatrix, int numRows, int numCols,
//                               out System.IntPtr outU, out int outRowsU, out int outColsU,
//                               out System.IntPtr outSingVals, out int outLength,
//                               out System.IntPtr outV, out int outRowsV, out int outColsV);
//
//        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_ZgetSVD")]
//        private static extern int LibLA_ZgetSVD(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
//                               out System.IntPtr outUReal, out System.IntPtr outUImag, out int outRowsU, out int outColsU,
//                               out System.IntPtr outSingVals, out int outLength,
//                               out System.IntPtr outVReal, out System.IntPtr outVImag, out int outRowsV, out int outColsV);
//
//
//    } // class Clapack
//} // namespace libstructural




}//Namespace ls




#endif

// The Original C# code below is from file LA.cs, in folder C:\...\libstruct\LibStructuralCSharp

////// Author: fbergmann
////// Project: LibStructuralCSharp
////// Path: C:\Users\fbergmann\Documents\Visual Studio Codename Orcas\Projects\CLAPACK\LibStructuralCSharp
////// Creation date: 6/27/2008 5:26 PM
////// Last modified: 7/2/2008 5:10 PM
////
////#region Using directives
////using System;
////using System.Runtime.InteropServices;
////#endregion
////
////namespace libstructural
////{
////
////    /// <summary>
////    /// <para>The LA class represents the entry point to all functions provided by
////    /// the C-library. These functions include a variety of useful functionality
////    /// operating on double and complex matrices. </para>
////    /// <para>The current scope of the library encompasses matrix factorizations (QR and LU
////    /// factorization) as well as commonly needed matrix operations, such as calculating
////    /// the inverse of a matrix, computing eigen values and singular values as well as
////    /// the null space of a matrix (both left and right null space) along with a method
////    /// for the computation of the row echelon or Gauss Jordan form of a matrix.</para>
////    /// </summary>
////    public static class LA
////    {
////        /// <summary>
////        /// The library file to where the members will be found. This will be
////        /// libstructural.dll on windows, and libLibStructural.so on linux and
////        /// OSX under mono.
////        /// </summary>
////        internal const string LIBRARY_FILE = "LibStructural";
////
////        /// <summary>
////        /// Passes the pointer 'matrix' to the library to have it freed.
////        /// </summary>
////        /// <param name="matrix">the 2d pointer to be freed</param>
////        /// <param name="numRows">the number of rows in this element</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_freeMatrix")]
////        internal static extern void FreeMatrix(System.IntPtr matrix, int numRows);
////
////        /// <summary>
////        /// passes the pointer 'vector' to the library to have it freed.
////        /// </summary>
////        /// <param name="vector">the vector to be freed</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_freeVector")]
////        internal static extern void FreeVector(System.IntPtr vector);
////
////        /// <summary>
////        /// Calculates the eigen-values of the given real matrix.
////        /// </summary>
////        /// <param name="oMatrix">a square real matrix</param>
////        public static Complex[] GetEigenValues(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerReal; IntPtr pointerImag; int nLength;
////            if (LibLA_getEigenValues(pointer, nRows, nCols, out pointerReal, out pointerImag, out nLength) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointer, nRows);
////                throw new Exception("The eigenvalues could not be computed, only square matrices are supported.");
////            } // if (LibLA_getEigenValues)
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            double[] realTemp = InteropUtil.GetDoubleArrayFromPtr(pointerReal, nLength);
////            double[] imagTemp = InteropUtil.GetDoubleArrayFromPtr(pointerImag, nLength);
////
////            Complex[] oResult = new Complex[nLength];
////            for (int i = 0; i < nLength; i++)
////            {
////                oResult[i] = new Complex(realTemp[i], imagTemp[i]);
////            } // for (int)
////            return oResult;
////        } // GetEigenValues(oMatrix)
////
////        /// <summary>
////        /// Calculates the eigen-values of a square complex matrix that has been split up
////        /// into real and complex parts.
////        /// </summary>
////        /// <param name="oMatrixReal">real part of the complex matrix</param>
////        /// <param name="oMatrixImag">imaginary part of the complex matrix</param>
////        public static Complex[] GetEigenValues(double[][] oMatrixReal, double[][] oMatrixImag)
////        {
////            IntPtr pointerReal; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrixReal, out pointerReal, out nRows, out nCols);
////            IntPtr pointerImag;
////            InteropUtil.MapMatrixToPointer(oMatrixImag, out pointerImag, out nRows, out nCols);
////
////            IntPtr oReal; IntPtr oImag; int nLength;
////            if (LibLA_ZgetEigenValues(pointerReal, pointerImag, nRows, nCols, out oReal, out oImag, out nLength) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointerReal, nRows);
////                InteropUtil.FreePtrMatrix(pointerImag, nRows);
////                throw new Exception("The eigenvalues could not be computed, only square matrices are supported.");
////            } // if (LibLA_ZgetEigenValues)
////
////            InteropUtil.FreePtrMatrix(pointerReal, nRows);
////            InteropUtil.FreePtrMatrix(pointerImag, nRows);
////
////            return InteropUtil.GetComplexArrayFromPtr(oReal, oImag, nLength);
////        } // GetEigenValues(oMatrixReal, oMatrixImag)
////
////
////        /// <summary>
////        /// Calculates the eigen-values of a square complex matrix.
////        /// </summary>
////        /// <param name="oMatrix">the square complex matrix</param>
////        public static Complex[] GetEigenValues(Complex[][] oMatrix)
////        {
////            double[][] oReal = new double[oMatrix.Length][];
////            double[][] oImag = new double[oMatrix.Length][];
////
////            for (int i = 0; i < oMatrix.Length; i++)
////            {
////                oReal[i] = new double[oMatrix[i].Length];
////                oImag[i] = new double[oMatrix[i].Length];
////
////                for (int j = 0; j < oMatrix[i].Length; j++)
////                {
////                    oReal[i][j] = oMatrix[i][j].Real;
////                    oImag[i][j] = oMatrix[i][j].Imag;
////                } // for (int)
////            } // for (int)
////
////            return GetEigenValues(oReal, oImag);
////        } // GetEigenValues(oMatrix)
////
////        /// <summary>
////        /// <para>This method calculates the fully pivoted Gauss Jordan Form of the given matrix.
////        /// Fully pivoted here means, that row as well as column swaps will be used. These
////        /// permutations have been captured in the integer vectors rowPivots and colPivots.</para>
////        ///
////        /// <para>If no permutations have occurred those vectors will be in ascending form [ 0, 1, 2, 3 ];
////        /// However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];</para>
////        ///
////        /// </summary>
////        /// <param name="oMatrix">the matrix to be brought into (fully pivoted) Gauss Jordan Form</param>
////        /// <param name="rowPivots">a vector of row permutations</param>
////        /// <param name="colPivots">a vector of column permutations</param>
////        public static double[][] GetFullyPivotedGaussJordan(double[][] oMatrix, out int[] rowPivots, out int[] colPivots)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////            IntPtr rowPivotArray; int nRowLength; IntPtr colPivotArray; int nColLength;
////            LibLA_fullyPivotedGaussJordan(pointer, nRows, nCols,
////                out pointerMatrix, out nMatrixRows, out nMatrixCols,
////                out rowPivotArray, out nRowLength, out colPivotArray, out nColLength);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            rowPivots = InteropUtil.GetIntArrayFromPtr(rowPivotArray, nRowLength);
////            colPivots = InteropUtil.GetIntArrayFromPtr(colPivotArray, nColLength);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetFullyPivotedGaussJordan(oMatrix, rowPivots, colPivots)
////
////
////        /// <summary>
////        /// This method calculates the fully pivoted Gauss Jordan Form of the given matrix.
////        /// Fully pivoted here means, that row as well as column swaps will be used. These
////        /// permutations will not be returned by this method.
////        ///
////        /// </summary>
////        /// <param name="oMatrix">the matrix to be brought into (fully pivoted) Gauss Jordan Form</param>
////        public static double[][] GetFullyPivotedGaussJordan(double[][] oMatrix)
////        {
////            int[] rowPivots; int[] colPivots;
////            return GetFullyPivotedGaussJordan(oMatrix, out rowPivots, out colPivots);
////        } // GetFullyPivotedGaussJordan(oMatrix)
////
////
////        /// <summary>
////        /// <para>This method calculates the Gauss Jordan or row echelon Form of the given matrix.
////        /// Only that row swaps will be used. These permutations will be returned in the 'pivots'
////        /// vector.</para>
////        ///
////        /// <para>If no permutations have occurred this vector will be in ascending form [ 0, 1, 2, 3 ];
////        /// However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];</para>
////        /// </summary>
////        /// <param name="oMatrix">the matrix to be brought into Gauss Jordan Form</param>
////        /// <param name="pivots">the vector of row permutations that have been used</param>
////        public static double[][] GetGaussJordan(double[][] oMatrix, out int[] pivots)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols; IntPtr pivotArray; int nLength;
////            LibLA_gaussJordan(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols, out pivotArray, out nLength);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            pivots = InteropUtil.GetIntArrayFromPtr(pivotArray, nLength);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetGaussJordan(oMatrix, pivots)
////
////        ///<summary>
////        /// This method calculates the Gauss Jordan or row echelon Form of the given matrix.
////        /// Only that row swaps will be used. These permutations will not be returned by this method.
////        ///
////        /// </summary>
////        /// <param name="oMatrix">the matrix to be brought into Gauss Jordan Form</param>
////        public static double[][] GetGaussJordan(double[][] oMatrix)
////        {
////            int[] pivots;
////            return GetGaussJordan(oMatrix, out pivots);
////        } // GetGaussJordan(oMatrix)
////
////
////        /// <summary>
////        /// <para>This function calculates the inverse of a square complex matrix. Employing the CLAPACK
////        /// functions: zgetrf and zgetri. That is first the matrix will be factorized using LU
////        /// decomposition followed by the calculation of the inverse based on:</para>
////        ///
////        /// <pre>inv(A)*L = inv(U) for inv(A).</pre>
////        ///
////        /// </summary>
////        /// <param name="oMatrix">a square complex matrix</param>
////        public static Complex[][] GetInverse(Complex[][] oMatrix)
////        {
////            double[][] oReal; double[][] oImag;
////
////            SplitComplexMatrix(oMatrix, out oReal, out oImag);
////
////            return GetInverse(oReal, oImag);
////        }
////
////        /// <summary>
////        /// Calculates the eigen-vectors of a square real matrix.
////        ///
////        /// This function calculates the complex (right) eigenvectors of the given real matrix.
////        /// The complex matrix returned contains the eigenvectors in the columns, in the same order as GetEigenValues.
////        ///
////        /// The right eigenvector v(j) of A satisfies:
////        ///
////        /// <pre>A * v(j) = lambda(j) * v(j) </pre>
////        ///
////        /// </summary>
////        /// <param name="oMatrix">a real matrix</param>
////        /// <returns>the complex eigenvectors of the matrix</returns>
////        public static Complex[][] GetEigenVectors(double[][] oMatrix)
////        {
////            IntPtr pointerReal; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointerReal, out nRows, out nCols);
////
////            System.IntPtr outMatrixReal; System.IntPtr outMatrixImag; int outRows; int outCols;
////            if (LibLA_getEigenVectors(pointerReal, nRows, nCols,
////                out outMatrixReal, out outMatrixImag, out outRows, out outCols) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointerReal, nRows);
////                throw new Exception("The Eigen Vectors could not be computed.");
////            }
////
////            InteropUtil.FreePtrMatrix(pointerReal, nRows);
////
////            return InteropUtil.GetComplexMatrixFromPtr(outMatrixReal, outMatrixImag, outRows, outCols);
////
////        }
////
////        /// <summary>
////        /// Factorizes the given matrix using SVD.
////        ///
////        /// This function computes the singular value decomposition (SVD) of the given real matrix.
////        ///
////        /// The SVD is written
////        /// <pre>A = U * SIGMA * transpose(V)</pre>
////        ///
////        /// Remarks:
////        /// this function returns the transpose of V
////        ///
////        /// </summary>
////        /// <param name="oMatrix">the real matrix to be decomposed (input) (With size M by N) </param>
////        /// <param name="uMatrix">(output) will be initialized with the M by M orthogonal matrix U </param>
////        /// <param name="singVals">(output) will be initialized with the min(M,N) singular values, returned in descending order </param>
////        /// <param name="vMatrix">(output) will be initialized with the N by N orthogonal matrix V </param>
////        public static void GetSVD(double[][] oMatrix, out double[][] uMatrix, out double[] singVals, out double[][] vMatrix)
////        {
////            IntPtr pointerReal; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointerReal, out nRows, out nCols);
////
////            IntPtr uMatrixPtr; int rowsU; int colsU;
////            IntPtr singValsPtr; int length;
////            IntPtr vMatrixPtr; int rowsV; int colsV;
////
////            if (LibLA_getSVD(pointerReal, nRows, nCols,
////                out uMatrixPtr, out rowsU, out colsU,
////                out singValsPtr, out length,
////                out vMatrixPtr, out rowsV, out colsV) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointerReal, nRows);
////                throw new Exception("Error during singular value decomposition (SVD)");
////            }
////
////            InteropUtil.FreePtrMatrix(pointerReal, nRows);
////
////            uMatrix = InteropUtil.GetDoubleMatrixFromPtr(uMatrixPtr, rowsU, colsU);
////            singVals = InteropUtil.GetDoubleArrayFromPtr(singValsPtr, length);
////            vMatrix = InteropUtil.GetDoubleMatrixFromPtr(vMatrixPtr, rowsV, colsV);
////
////        }
////
////        /// <summary>
////        /// Factorizes the given matrix using SVD.
////        ///
////        /// This function computes the singular value decomposition (SVD) of the given complex matrix.
////        ///
////        /// The SVD is written
////        /// <pre>A = U * SIGMA * conjugate-transpose (V)</pre>
////        ///
////        /// Remarks:
////        /// this function returns the conjugate-transpose of V
////        ///
////        /// </summary>
////        /// <param name="oMatrix">the complex matrix to be decomposed (input) (With size M by N) </param>
////        /// <param name="uMatrix">(output) will be initialized with the M by M orthogonal matrix U </param>
////        /// <param name="singVals">(output) will be initialized with the min(M,N) singular values, returned in descending order </param>
////        /// <param name="vMatrix">(output) will be initialized with the N by N orthogonal matrix V </param>
////        public static void GetSVD(Complex[][] oMatrix, out Complex[][] uMatrix, out double[] singVals, out Complex[][] vMatrix)
////        {
////            double[][] oMatrixReal; double[][] oMatrixImag;
////            SplitComplexMatrix(oMatrix, out oMatrixReal, out oMatrixImag);
////
////            GetSVD(oMatrixReal, oMatrixImag, out uMatrix, out singVals, out vMatrix);
////        }
////
////        /// <summary>
////        /// Factorizes the given matrix using SVD.
////        ///
////        /// This function computes the singular value decomposition (SVD) of the given complex matrix (separated into its real and imaginary part).
////        ///
////        /// The SVD is written
////        /// <pre>A = U * SIGMA * conjugate-transpose (V)</pre>
////        ///
////        /// Remarks:
////        /// this function returns the conjugate-transpose of V
////        ///
////        /// </summary>
////        /// <param name="oMatrixReal">the real part of the complex matrix to be decomposed (input) (With size M by N) </param>
////        /// <param name="oMatrixImag">the imaginary part of the complex matrix to be decomposed (input) (With size M by N) </param>
////        /// <param name="uMatrix">(output) will be initialized with the M by M orthogonal matrix U </param>
////        /// <param name="singVals">(output) will be initialized with the min(M,N) singular values, returned in descending order </param>
////        /// <param name="vMatrix">(output) will be initialized with the N by N orthogonal matrix V </param>
////        public static void GetSVD(double[][] oMatrixReal, double[][] oMatrixImag, out Complex[][] uMatrix, out double[] singVals, out Complex[][] vMatrix)
////        {
////            IntPtr pointerReal; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrixReal, out pointerReal, out nRows, out nCols);
////            IntPtr pointerImag;
////            InteropUtil.MapMatrixToPointer(oMatrixImag, out pointerImag, out nRows, out nCols);
////
////            IntPtr uMatrixPtrReal; IntPtr uMatrixPtrImag; int rowsU; int colsU;
////            IntPtr singValsPtr; int length;
////            IntPtr vMatrixPtrReal; IntPtr vMatrixPtrImag; int rowsV; int colsV;
////
////            if (LibLA_ZgetSVD(pointerReal, pointerImag, nRows, nCols,
////                out uMatrixPtrReal, out uMatrixPtrImag, out rowsU, out colsU,
////                out singValsPtr, out length,
////                out vMatrixPtrReal, out vMatrixPtrImag, out rowsV, out colsV) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointerReal, nRows);
////                InteropUtil.FreePtrMatrix(pointerImag, nRows);
////                throw new Exception("Error during singular value decomposition (SVD)");
////            }
////
////            InteropUtil.FreePtrMatrix(pointerReal, nRows);
////            InteropUtil.FreePtrMatrix(pointerImag, nRows);
////
////            uMatrix = InteropUtil.GetComplexMatrixFromPtr(uMatrixPtrReal, uMatrixPtrImag, rowsU, colsU);
////            singVals = InteropUtil.GetDoubleArrayFromPtr(singValsPtr, length);
////            vMatrix = InteropUtil.GetComplexMatrixFromPtr(vMatrixPtrReal, vMatrixPtrImag, rowsV, colsV);
////
////        }
////
////        /// <summary>
////        /// Calculates the eigen-vectors of a complex matrix (separated into real and imaginary parts).
////        ///
////        /// This function calculates the complex (right) eigenvectors of the given complex matrix.
////        /// The complex matrix returned contains the eigenvectors in the columns, in the same order as GetEigenValues.
////        ///
////        /// The right eigenvector v(j) of A satisfies:
////        ///
////        /// <pre>A * v(j) = lambda(j) * v(j) </pre>
////        ///
////        /// </summary>
////        /// <param name="oMatrix">the complex matrix</param>
////        /// <returns>the complex eigenvectors of the matrix</returns>
////        public static Complex[][] GetEigenVectors(Complex[][] oMatrix)
////        {
////            double[][] real; double[][] complex;
////            SplitComplexMatrix(oMatrix, out real, out complex);
////            return GetEigenVectors(real, complex);
////        }
////
////        /// <summary>
////        /// Calculates the eigen-vectors of a complex matrix (separated into real and imaginary parts).
////        ///
////        /// This function calculates the complex (right) eigenvectors of the given complex matrix.
////        /// The complex matrix returned contains the eigenvectors in the columns, in the same order as GetEigenValues.
////        ///
////        /// The right eigenvector v(j) of A satisfies:
////        ///
////        /// <pre>A * v(j) = lambda(j) * v(j) </pre>
////        ///
////        /// </summary>
////        /// <param name="oMatrixReal">real part of the matrix </param>
////        /// <param name="oMatrixImag">the imaginary part of the matrix</param>
////        /// <returns>the complex eigenvectors of the matrix</returns>
////        public static Complex[][] GetEigenVectors(double[][] oMatrixReal, double[][] oMatrixImag)
////        {
////            IntPtr pointerReal; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrixReal, out pointerReal, out nRows, out nCols);
////            IntPtr pointerImag;
////            InteropUtil.MapMatrixToPointer(oMatrixImag, out pointerImag, out nRows, out nCols);
////
////            System.IntPtr outMatrixReal; System.IntPtr outMatrixImag; int outRows; int outCols;
////            if (LibLA_ZgetEigenVectors(pointerReal, pointerImag, nRows, nCols,
////                out outMatrixReal, out outMatrixImag, out outRows, out outCols) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointerReal, nRows);
////                InteropUtil.FreePtrMatrix(pointerImag, nRows);
////                throw new Exception("The Eigen Vectors could not be computed.");
////            }
////
////            InteropUtil.FreePtrMatrix(pointerReal, nRows);
////            InteropUtil.FreePtrMatrix(pointerImag, nRows);
////
////            return InteropUtil.GetComplexMatrixFromPtr(outMatrixReal, outMatrixImag, outRows, outCols);
////
////        }
////
////        /// <summary>
////        /// Splits the given complex matrix into its real and imaginary matrix
////        /// </summary>
////        /// <param name="oMatrix">complex matrix</param>
////        /// <param name="oReal">matrix holding the real part</param>
////        /// <param name="oImag">matrix holding the imaginary part</param>
////        private static void SplitComplexMatrix(Complex[][] oMatrix, out double[][] oReal, out double[][] oImag)
////        {
////            oReal = new double[oMatrix.Length][];
////            oImag = new double[oMatrix.Length][];
////
////            for (int i = 0; i < oMatrix.Length; i++)
////            {
////                oReal[i] = new double[oMatrix[i].Length];
////                oImag[i] = new double[oMatrix[i].Length];
////
////                for (int j = 0; j < oMatrix[i].Length; j++)
////                {
////                    oReal[i][j] = oMatrix[i][j].Real;
////                    oImag[i][j] = oMatrix[i][j].Imag;
////                } // for (int)
////            } // for (int)
////        }
////        /// <summary>
////        /// <para>This function calculates the inverse of a square complex matrix. Employing the CLAPACK
////        /// functions: zgetrf and zgetri. That is first the matrix will be factorized using LU
////        /// decomposition followed by the calculation of the inverse based on:</para>
////        ///
////        /// <pre>inv(A)*L = inv(U) for inv(A).</pre>
////        ///
////        /// </summary>
////        /// <param name="oMatrixImag">The imaginary part of the matrix</param>
////        /// <param name="oMatrixReal">The real part of the matrix</param>
////        public static Complex[][] GetInverse(double[][] oMatrixReal, double[][] oMatrixImag)
////        {
////            IntPtr pointerReal; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrixReal, out pointerReal, out nRows, out nCols);
////            IntPtr pointerImag;
////            InteropUtil.MapMatrixToPointer(oMatrixImag, out pointerImag, out nRows, out nCols);
////
////            System.IntPtr outMatrixReal; System.IntPtr outMatrixImag; int outRows; int outCols;
////            if (LibLA_Zinverse(pointerReal, pointerImag, nRows, nCols,
////                out outMatrixReal, out outMatrixImag, out outRows, out outCols) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointerReal, nRows);
////                InteropUtil.FreePtrMatrix(pointerImag, nRows);
////                throw new Exception("The Inverse could not be computed, only square matrices have an inverse.");
////            } // if (LibLA_Zinverse)
////
////            InteropUtil.FreePtrMatrix(pointerReal, nRows);
////            InteropUtil.FreePtrMatrix(pointerImag, nRows);
////
////            return InteropUtil.GetComplexMatrixFromPtr(outMatrixReal, outMatrixImag, outRows, outCols);
////
////        } // GetInverse(oMatrixReal, oMatrixImag)
////
////        /// <summary>
////        /// <para>This function calculates the inverse of a square real matrix. Employing the CLAPACK
////        /// functions: dgetrf and dgetri. That is first the matrix will be factorized using LU
////        /// decomposition followed by the calculation of the inverse based on: </para>
////        ///
////        /// <pre>inv(A)*L = inv(U) for inv(A).</pre>
////        ///
////        /// </summary>
////        /// <param name="oMatrix">a square real matrix</param>
////        public static double[][] GetInverse(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////            if (LibLA_inverse(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointer, nRows);
////                throw new Exception("The Inverse could not be computed, only square matrices have an inverse.");
////            } // if (LibLA_inverse)
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetInverse(oMatrix)
////
////        /// <summary>
////        /// <para>This function calculates the left null space of a given real matrix. That is:</para>
////        ///
////        /// <pre>null(A)*A = 0</pre>
////        ///
////        /// <para>This function is equivalent to <seealso cref="GetRightNullSpace"/> returning
////        /// the right null space of the transposed matrix.</para>
////        /// </summary>
////        /// <param name="oMatrix">The real matrix for which to compute the left null space</param>
////        public static double[][] GetLeftNullSpace(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////            LibLA_leftNullspace(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetLeftNullSpace(oMatrix)
////
////        /// <summary>
////        /// <para>This function computes the LU factorization of the given real M-by-N matrix A
////        /// using partial pivoting with row interchanges. This procedure is carried out by the LAPACK method dgetrf.</para>
////        /// <para>A is factorized into: </para>
////        /// <pre>A = P * L * U</pre>
////        /// <para>the result is encapsulated in an LUResult object (with an empty Q matrix)</para>
////        /// </summary>
////        /// <param name="oMatrix">real M-by-N matrix</param>
////        public static LUResult GetLU(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            System.IntPtr L; int nLRows; int nLCols;
////            System.IntPtr U; int nURows; int nUCols;
////            System.IntPtr P; int nPRows; int nPCols;int info;
////
////            LibLA_getLU(pointer, nRows, nCols,
////                out L, out nLRows, out nLCols,
////                out U, out nURows, out nUCols,
////                out P, out nPRows, out nPCols,out info);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return new LUResult(info,
////                InteropUtil.GetDoubleMatrixFromPtr(L, nLCols, nLCols),
////                InteropUtil.GetDoubleMatrixFromPtr(U, nUCols, nUCols),
////                InteropUtil.GetIntMatrixFromPtr(P, nPCols, nPCols)
////                );
////        } // GetLU(oMatrix)
////
////        /// <summary>
////        /// <para>This function computes the LU factorization of the given real N-by-N matrix A
////        /// using complete pivoting (with row and column interchanges). This procedure is carried out by the LAPACK method dgetc2.</para>
////        /// <para>A is factorized into: </para>
////        /// <pre>A = P * L * U * Q</pre>
////        /// <para>the result is encapsulated in an LUResult object</para>
////        /// </summary>
////        /// <param name="oMatrix">real N-by-N matrix</param>
////        public static LUResult GetLUWithPivoting(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            System.IntPtr L; int nLRows; int nLCols;
////            System.IntPtr U; int nURows; int nUCols;
////            System.IntPtr P; int nPRows; int nPCols;
////            System.IntPtr Q; int nQRows; int nQCols; int info;
////            if (LibLA_getLUwithFullPivoting(pointer, nRows, nCols,
////                out L, out nLRows, out nLCols,
////                out U, out nURows, out nUCols,
////                out P, out nPRows, out nPCols,
////                out Q, out nQRows, out nQCols, out info) < 0)
////            {
////                InteropUtil.FreePtrMatrix(pointer, nRows);
////                throw new Exception("The LU factorization (with pivoting) could not be computed, only square matrices are supported.");
////            } // if (LibLA_getLUwithFullPivoting)
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return new LUResult(info,
////                InteropUtil.GetDoubleMatrixFromPtr(L, nLCols, nLCols),
////                InteropUtil.GetDoubleMatrixFromPtr(U, nUCols, nUCols),
////                InteropUtil.GetIntMatrixFromPtr(P, nPCols, nPCols),
////                InteropUtil.GetIntMatrixFromPtr(Q, nQCols, nQCols)
////                );
////        } // GetLUWithPivoting(oMatrix)
////
////        /// <summary>
////        /// <para>This function computes the QR factorization of the given real M-by-N
////        /// matrix A. The LAPACK method DGEQRF is used followed by an
////        /// orthonormalization of Q through the use of DORGQR.</para>
////        /// <para>The factorized form is: </para>
////        /// <pre>A = Q * R</pre>
////        /// <para>the result is encapsulated in an QRResult object (with an empty P matrix)</para>
////        /// </summary>
////        /// <param name="oMatrix">real M-by-N matrix</param>
////        public static QRResult GetQR(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerQ; int nRowsQ; int nColsQ;
////            IntPtr pointerR; int nRowsR; int nColsR;
////
////            LibLA_getQR(pointer, nRows, nCols,
////                out pointerQ, out nRowsQ, out nColsQ,
////                out pointerR, out nRowsR, out nColsR);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return new QRResult(
////                InteropUtil.GetDoubleMatrixFromPtr(pointerQ, nRowsQ, nColsQ),
////                InteropUtil.GetDoubleMatrixFromPtr(pointerR, nRowsR, nColsR));
////
////        } // GetQR(oMatrix)
////
////        /// <summary>
////        /// <para>This function computes the QR factorization of the given real M-by-N matrix A
////        /// with column pivoting. The LAPACK method dgeqp3 is
////        /// used followed by an orthonormalization of Q through the use of DORGQR.</para>
////        /// <para>The factorized form is: </para>
////        /// <pre>A = Q * R</pre>
////        /// <para>the result is encapsulated in an QRResult object (with the P matrix returning the
////        /// pivots used)</para>
////        /// </summary>
////        /// <param name="oMatrix">real M-by-N matrix</param>
////        public static QRResult GetQRWithPivot(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerQ; int nRowsQ; int nColsQ;
////            IntPtr pointerR; int nRowsR; int nColsR;
////            IntPtr pointerP; int nRowsP; int nColsP;
////
////            LibLA_getQRWithPivot(pointer, nRows, nCols,
////                out pointerQ, out nRowsQ, out nColsQ,
////                out pointerR, out nRowsR, out nColsR,
////                out pointerP, out nRowsP, out nColsP);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return new QRResult(
////                InteropUtil.GetDoubleMatrixFromPtr(pointerQ, nRowsQ, nColsQ),
////                InteropUtil.GetDoubleMatrixFromPtr(pointerR, nRowsR, nColsR),
////                InteropUtil.GetDoubleMatrixFromPtr(pointerP, nRowsP, nColsP));
////
////        } // GetQRWithPivot(oMatrix)
////
////
////
////        /// <summary>
////        /// <para>This method computes the rank of the given matrix. For this first the
////        /// singular values of the matrix are calculated and then the rank is determined
////        /// by the number of non-zero values. </para>
////        /// <para>Note that zero here is defined as any value whose absolute value is bigger than
////        /// the set tolerance.</para>
////        /// </summary>
////        /// <param name="oMatrix">a real matrix</param>
////        public static int GetRank(double[][] oMatrix)
////        {
////            int nResult = 0;
////
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////            nResult = LibLA_getRank(pointer, nRows, nCols);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return nResult;
////
////        } // GetRank(oMatrix)
////
////
////
////        /// <summary>
////        /// returns an estimate for the reciprocal of the condition of A in 1-norm using the LAPACK condition estimator.
////        /// If A is well conditioned, getRCond(A) is near 1.0. If A is badly conditioned, getRCond(A) is near 0.0.
////        /// </summary>
////        /// <param name="oMatrix">a real matrix</param>
////        /// <returns>reciprocal condition number estimate</returns>
////        public static double GetRCond(double[][] oMatrix)
////        {
////            double nResult = 0.0;
////
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////            nResult = LibLA_getRCond(pointer, nRows, nCols);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return nResult;
////
////        } // GetRCond(oMatrix)
////
////
////        /// <summary>
////        /// <para>This function calculates the right null space of a given real matrix. That is:</para>
////        ///
////        /// <pre>A*null(A) = 0</pre>
////        ///
////        /// <para>In order to calculate the (right) null space, we first calculate the full
////        /// singular value decomposition (employing dgesdd) of the matrix: </para>
////        ///
////        /// <pre>[U,S,V] = svd(A');</pre>
////        ///
////        /// <para>then calculate the rank: </para>
////        ///
////        /// <pre>r = rank(A)</pre>
////        ///
////        /// <para>and finally return the last columns of the U matrix (r+1...n) as the null space
////        /// matrix.</para>
////        /// </summary>
////        /// <param name="oMatrix">The real matrix for which to compute the right null space</param>
////        public static double[][] GetRightNullSpace(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////            LibLA_rightNullspace(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetRightNullSpace(oMatrix)
////
////        /// <summary>
////        /// <para>This function calculates the left null space of a given real matrix. That is:</para>
////        ///
////        /// <pre>null(A)*A = 0</pre>
////        ///
////        /// <para>This function is equivalent to <seealso cref="GetRightNullSpace"/> returning
////        /// the right null space of the transposed matrix.</para>
////        /// </summary>
////        /// <param name="oMatrix">The real matrix for which to compute the left null space</param>
////        public static double[][] GetScaledLeftNullSpace(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////            LibLA_scaledLeftNullspace(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetScaledLeftNullSpace(oMatrix)
////
////        /// <summary>
////        /// <para>This function calculates the right null space of a given real matrix. That is:</para>
////        ///
////        /// <pre>A*null(A) = 0</pre>
////        ///
////        /// <para>In order to calculate the (right) null space, we first calculate the full
////        /// singular value decomposition (employing dgesdd) of the matrix: </para>
////        ///
////        /// <pre>[U,S,V] = svd(A');</pre>
////        ///
////        /// <para>then calculate the rank: </para>
////        ///
////        /// <pre>r = rank(A)</pre>
////        ///
////        /// <para>and finally return the last columns of the U matrix (r+1...n) as the null space
////        /// matrix.</para>
////        /// <remarks>This method also scaled the right null space using gauss jorden</remarks>
////        /// </summary>
////        /// <param name="oMatrix">The real matrix for which to compute the right null space</param>
////        public static double[][] GetScaledRightNullSpace(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////            LibLA_scaledRightNullspace(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols);
////
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        } // GetScaledRightNullSpace(oMatrix)
////
////        /// <summary>
////        /// This method performs the Singular Value Decomposition of the given real matrix,
////        /// returning only the singular values. For this the LAPACK method: dgesdd was used.
////        /// </summary>
////        /// <param name="oMatrix">a real matrix</param>
////        public static double[] GetSingularValues(double[][] oMatrix)
////        {
////            IntPtr pointer; int nRows; int nCols;
////            InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////            IntPtr singularVals; int nLength;
////            LibLA_getSingularValsBySVD(pointer, nRows, nCols, out singularVals, out nLength);
////            InteropUtil.FreePtrMatrix(pointer, nRows);
////
////            return InteropUtil.GetDoubleArrayFromPtr(singularVals, nLength);
////        } // GetSingularValues(oMatrix)
////
////        /// <summary>
////        /// This function returns the tolerance currently used by the library to determine what value
////        /// is seen as zero. (Any value with absolute value smaller than this tolerance is seen as zero
////        /// and will be neglected).
////        /// </summary>
////        /// <returns>The tolerance used by the CLAPACK library to determine a
////        /// value close to zero.
////        /// </returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getTolerance")]
////        public static extern double GetTolerance();
////
////        //[DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_gaussJordan2")]
////        //private static extern int LibLA_gaussJordan2(System.IntPtr inMatrix, int numRows, int numCols,
////        //    out System.IntPtr outMatrix, out int outRows, out int outCols);
////
////        //public static double[][] GetGaussJordan2(double[][] oMatrix)
////        //{
////        //    IntPtr pointer; int nRows; int nCols;
////        //    InteropUtil.MapMatrixToPointer(oMatrix, out pointer, out nRows, out nCols);
////
////        //    IntPtr pointerMatrix; int nMatrixRows; int nMatrixCols;
////        //    LibLA_gaussJordan2(pointer, nRows, nCols, out pointerMatrix, out nMatrixRows, out nMatrixCols);
////
////        //    InteropUtil.FreePtrMatrix(pointer, nRows);
////
////        //    return InteropUtil.GetDoubleMatrixFromPtr(pointerMatrix, nMatrixRows, nMatrixCols);
////
////        //}
////
////        /// <summary>
////        /// Lib clapack _fully pivoted gauss jordan
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        /// <param name="outRowPivot">Out row pivot</param>
////        /// <param name="nRowLength">N row length</param>
////        /// <param name="outColPivot">Out color pivot</param>
////        /// <param name="nColLength">N color length</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_fullyPivotedGaussJordan")]
////        private static extern int LibLA_fullyPivotedGaussJordan(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols,
////            out System.IntPtr outRowPivot, out int nRowLength,
////            out System.IntPtr outColPivot, out int nColLength);
////
////
////
////        /// <summary>
////        /// Lib clapack _gauss jordan
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        /// <param name="outPivot">Out pivot</param>
////        /// <param name="nLength">N length</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_gaussJordan")]
////        private static extern int LibLA_gaussJordan(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols, out System.IntPtr outPivot, out int nLength);
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getEigenValues")]
////        private static extern int LibLA_getEigenValues(System.IntPtr inMatrix,
////            int numRows,
////            int numCols,
////            out System.IntPtr outReal,
////            out System.IntPtr outImag,
////            out int outLength);
////
////
////        /// Return Type: int
////        ///inMatrix: double**
////        ///numRows: int
////        ///numCols: int
////        ///outL: double***
////        ///outLRows: int*
////        ///outLCols: int*
////        ///outU: double***
////        ///outURows: int*
////        ///outUCols: int*
////        ///outP: int***
////        ///outPRows: int*
////        ///outPCols: int*
////        ///info: int*
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outL">Out l</param>
////        /// <param name="outLRows">Out l rows</param>
////        /// <param name="outLCols">Out l cols</param>
////        /// <param name="outU">Ou texture coordinatet u texture coordinate</param>
////        /// <param name="outURows">Ou texture coordinatet u texture coordinate rows</param>
////        /// <param name="outUCols">Ou texture coordinatet u texture coordinate cols</param>
////        /// <param name="outP">Out p</param>
////        /// <param name="outPRows">Out p rows</param>
////        /// <param name="outPCols">Out p cols</param>
////        /// <param name="info">Info</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getLU")]
////        private static extern int LibLA_getLU(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outL, out int outLRows, out int outLCols,
////            out System.IntPtr outU, out int outURows, out int outUCols,
////            out System.IntPtr outP, out int outPRows, out int outPCols, out int info);
////
////
////        /// Return Type: int
////        ///inMatrix: double**
////        ///numRows: int
////        ///numCols: int
////        ///outL: double***
////        ///outLRows: int*
////        ///outLCols: int*
////        ///outU: double***
////        ///outURows: int*
////        ///outUCols: int*
////        ///outP: int***
////        ///outPRows: int*
////        ///outPCols: int*
////        ///outQ: int***
////        ///outQRows: int*
////        ///outQCols: int*
////        ///info: int*
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outL">Out l</param>
////        /// <param name="outLRows">Out l rows</param>
////        /// <param name="outLCols">Out l cols</param>
////        /// <param name="outU">Ou texture coordinatet u texture coordinate</param>
////        /// <param name="outURows">Ou texture coordinatet u texture coordinate rows</param>
////        /// <param name="outUCols">Ou texture coordinatet u texture coordinate cols</param>
////        /// <param name="outP">Out p</param>
////        /// <param name="outPRows">Out p rows</param>
////        /// <param name="outPCols">Out p cols</param>
////        /// <param name="outQ">Out q</param>
////        /// <param name="outQRows">Out q rows</param>
////        /// <param name="outQCols">Out q cols</param>
////        /// <param name="info">Info</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getLUwithFullPivoting")]
////        private static extern int LibLA_getLUwithFullPivoting(
////            System.IntPtr inMatrix,  int numRows, int numCols,
////            out System.IntPtr outL, out int outLRows, out int outLCols,
////            out System.IntPtr outU, out int outURows, out int outUCols,
////            out System.IntPtr outP, out int outPRows, out int outPCols,
////            out System.IntPtr outQ, out int outQRows, out int outQCols,
////            out int info);
////        /// Return Type: int
////        ///inMatrix: double**
////        ///numRows: int
////        ///numCols: int
////        ///outQ: double***
////        ///outQRows: int*
////        ///outQCols: int*
////        ///outR: double***
////        ///outRRows: int*
////        ///outRCols: int*
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outQ">Out q</param>
////        /// <param name="outQRows">Out q rows</param>
////        /// <param name="outQCols">Out q cols</param>
////        /// <param name="outR">Out r</param>
////        /// <param name="outRRows">Out r rows</param>
////        /// <param name="outRCols">Out r cols</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getQR")]
////        private static extern int LibLA_getQR(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outQ, out int outQRows, out int outQCols,
////            out System.IntPtr outR, out int outRRows, out int outRCols);
////        /// Return Type: int
////        ///inMatrix: double**
////        ///numRows: int
////        ///numCols: int
////        ///outQ: double***
////        ///outQRows: int*
////        ///outQCols: int*
////        ///outR: double***
////        ///outRRows: int*
////        ///outRCols: int*
////        ///outP: double***
////        ///outPRows: int*
////        ///outPCols: int*
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outQ">Out q</param>
////        /// <param name="outQRows">Out q rows</param>
////        /// <param name="outQCols">Out q cols</param>
////        /// <param name="outR">Out r</param>
////        /// <param name="outRRows">Out r rows</param>
////        /// <param name="outRCols">Out r cols</param>
////        /// <param name="outP">Out p</param>
////        /// <param name="outPRows">Out p rows</param>
////        /// <param name="outPCols">Out p cols</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getQRWithPivot")]
////        private static extern int LibLA_getQRWithPivot(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outQ, out int outQRows, out int outQCols,
////            out System.IntPtr outR, out int outRRows, out int outRCols,
////            out System.IntPtr outP, out int outPRows, out int outPCols);
////
////
////        /// <summary>
////        /// Lib clapack _get rank
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getRank")]
////        private static extern int LibLA_getRank(System.IntPtr inMatrix, int numRows, int numCols);
////
////
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getRCond")]
////        private static extern double LibLA_getRCond(System.IntPtr inMatrix, int numRows, int numCols);
////
////        /// Return Type: int
////        ///inMatrix: double**
////        ///numRows: int
////        ///numCols: int
////        ///outSingularVals: double**
////        ///outLength: int*
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outSingularVals">Out singular vals</param>
////        /// <param name="outLength">Out length</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getSingularValsBySVD")]
////        private static extern int LibLA_getSingularValsBySVD(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outSingularVals, out int outLength);
////
////
////
////
////        /// Return Type: int
////        ///inMatrix: double**
////        ///numRows: int
////        ///numCols: int
////        ///outMatrix: double***
////        ///outRows: int*
////        ///outCols: int*
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_inverse")]
////        private static extern int LibLA_inverse(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols);
////        /// <summary>
////        /// left null space
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_leftNullspace")]
////        private static extern int LibLA_leftNullspace(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols);
////
////        /// <summary>
////        /// right null space
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_rightNullspace")]
////        private static extern int LibLA_rightNullspace(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols);
////
////
////        /// <summary>
////        /// get scaled left null space
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_scaledLeftNullspace")]
////        private static extern int LibLA_scaledLeftNullspace(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols);
////
////        /// <summary>
////        /// get scaled right null space
////        /// </summary>
////        /// <param name="inMatrix">In matrix</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrix">Out matrix</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_scaledRightNullspace")]
////        private static extern int LibLA_scaledRightNullspace(System.IntPtr inMatrix, int numRows, int numCols,
////            out System.IntPtr outMatrix, out int outRows, out int outCols);
////
////
////
////        /// Return Type: int
////        ///inMatrixReal: double**
////        ///inMatrixImag: double**
////        ///numRows: int
////        ///numCols: int
////        ///outReal: double**
////        ///outImag: double**
////        ///outLength: int*
////        /// <param name="inMatrixReal">In matrix real</param>
////        /// <param name="inMatrixImag">In matrix imag</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outReal">Out real</param>
////        /// <param name="outImag">Out imag</param>
////        /// <param name="outLength">Out length</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_ZgetEigenValues")]
////        private static extern int LibLA_ZgetEigenValues(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
////            out System.IntPtr outReal, out System.IntPtr outImag, out int outLength);
////        /// Return Type: int
////        ///inMatrixReal: double**
////        ///inMatrixImag: double**
////        ///numRows: int
////        ///numCols: int
////        ///outMatrixReal: double***
////        ///outMatrixImag: double**
////        ///outRows: int*
////        ///outCols: int*
////        /// <param name="inMatrixReal">In matrix real</param>
////        /// <param name="inMatrixImag">In matrix imag</param>
////        /// <param name="numRows">Number rows</param>
////        /// <param name="numCols">Number cols</param>
////        /// <param name="outMatrixReal">Out matrix real</param>
////        /// <param name="outMatrixImag">Out matrix imag</param>
////        /// <param name="outRows">Out rows</param>
////        /// <param name="outCols">Out cols</param>
////        /// <returns>Int</returns>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_Zinverse")]
////        private static extern int LibLA_Zinverse(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
////            out System.IntPtr outMatrixReal, out System.IntPtr outMatrixImag, out int outRows, out int outCols);
////
////
////        /// <summary>
////        /// This function sets the tolerance used by the library to determine what value
////        /// is seen as zero. (Any value with absolute value smaller than this tolerance is seen as zero
////        /// and will be neglected).
////        /// </summary>
////        /// <param name="value">Sets the tolerance used by the CLAPACK library to determine a
////        /// value close to zero.</param>
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_setTolerance")]
////        public static extern void SetTolerance(double value);
////
////
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getEigenVectors")]
////        private static extern int LibLA_getEigenVectors(System.IntPtr inMatrix, int numRows, int numCols,
////                                     out System.IntPtr outMatrixReal, out System.IntPtr outMatrixImag, out int outRows, out int outCols);
////
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_ZgetEigenVectors")]
////        private static extern int LibLA_ZgetEigenVectors(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
////                                       out System.IntPtr outMatrixReal, out System.IntPtr outMatrixImag, out int outRows, out int outCols);
////
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_getSVD")]
////        private static extern int LibLA_getSVD(System.IntPtr inMatrix, int numRows, int numCols,
////                               out System.IntPtr outU, out int outRowsU, out int outColsU,
////                               out System.IntPtr outSingVals, out int outLength,
////                               out System.IntPtr outV, out int outRowsV, out int outColsV);
////
////        [DllImportAttribute(LIBRARY_FILE, EntryPoint = "LibLA_ZgetSVD")]
////        private static extern int LibLA_ZgetSVD(System.IntPtr inMatrixReal, System.IntPtr inMatrixImag, int numRows, int numCols,
////                               out System.IntPtr outUReal, out System.IntPtr outUImag, out int outRowsU, out int outColsU,
////                               out System.IntPtr outSingVals, out int outLength,
////                               out System.IntPtr outVReal, out System.IntPtr outVImag, out int outRowsV, out int outColsV);
////
////
////    } // class Clapack
////} // namespace libstructural

