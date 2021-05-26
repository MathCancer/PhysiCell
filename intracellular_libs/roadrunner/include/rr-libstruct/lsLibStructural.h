/**
 * \author Frank T. Bergmann (fbergman@u.washington.edu)
 * \author     Herbert M. Sauro
 * \author     Ravishankar Rao Vallabhajosyula (developed a previous version of the sructural analysis code)
 *
 * Fixes and improvments: Totte Karsson
 */

#ifndef lsLibStructuralH
#define lsLibStructuralH
#include <vector>
#include <string>
#include <map>
#include <complex>
#include "lsExporter.h"
#include "lsMatrix.h"

namespace libsbml { class Model; }

namespace ls
{
typedef Matrix< int >                               IntMatrix;
typedef Matrix< double >                            DoubleMatrix;
typedef Matrix< ls::Complex >                       ComplexMatrix;

class SBMLmodel;

/**
 * analyze the structural properties of stoichiometric networks.
 */
class LIB_EXTERN LibStructural
{
private:
    /**
     * Clears everything. The load... metthods automatically call this,
     * as well as the dtor.
     */
    void                                            Reset();

    double                                          _Tolerance;

    SBMLmodel* _Model;

    /**
     * set to the total number of species, stochiometry matrix
     * species are the rows.
     */
    int                                             _NumRows;

    /**
     * set the the number of reactinos, stochiometry matrix reactions
     * are the number of columns.
     */
    int                                             _NumCols;

    DoubleMatrix*                                   _K0;
    DoubleMatrix*                                   _N0;
    DoubleMatrix*                                   _Nr;
    DoubleMatrix*                                   _L0;

    /**
     * Link Matrix
     */
    DoubleMatrix*                                   _L;

    /**
     * Null Space Matrix
     */
    DoubleMatrix*                                   _K;
    DoubleMatrix*                                   _NullN;
    DoubleMatrix*                                   _G;        // conservation law array

    DoubleMatrix*                                   _Nmat;
    DoubleMatrix*                                   _Nmat_orig;
    DoubleMatrix*                                   _NmatT;
    DoubleMatrix*                                   _NmatT_orig;


    double*                                         _Totals; // conserved totals
    double*                                         _IC;
    double*                                         _BC;

    /**
     * floating species
     */
    int*                                            spVec;

    /**
     * reactions
     */
    int*                                            colVec;

    std::vector < std::string >                     _consv_list;
    double                                          _Sparsity;
    double                                          _Pvalue;
    int                                             _svd_rank_Nmat;
    int                                             _svd_rank_Nrmat;
    int                                             _qr_rank_Nrmat;

    /**
     * number of linearly independent species,
     */
    int                                             _NumIndependent;
    int                                             _NumDependent;

    int                                             nz_count;
    int                                             numFloating;
    int                                             numReactions;
    int                                             numBoundary;
    bool                                            zero_nmat;


    int                                             _SvdRankNr;
    int                                             _SvdRankNmat;
    int                                             _QrRankNmat;

    std::string                                     _sModelName;

    std::map<int, std::string>                      _speciesIndexList;
    std::map<std::string, int>                      _speciesIndexList2;
    std::map<int, std::string>                      _speciesNamesList;
    std::map<std::string, int>                      _speciesNamesList2;
    std::map<int, std::string>                      _reactionIndexList;
    std::map<int, std::string>                      _reactionNamesList;
    std::map<std::string, int>                      _modSpeciesIndexList;
    std::map<std::string, int>                      _modSpeciesNamesList;
    std::map<std::string, double>                   _speciesValueList;
    std::map<std::string, double>                   _variableList;
    std::map<int, std::string>                      _bSpeciesIndexList;
    std::map<std::string, int>                      _bSpeciesIndexList2;
    std::map<int, std::string>                      _bSpeciesNamesList;
    std::map<std::string, int>                      _bSpeciesNamesList2;
    std::map<std::string, double>                   _bSpeciesValueList;
    std::vector<std::string>                        _inputSpeciesNames;
    std::vector<std::string>                        _inputReactionNames;
    std::vector<double>                             _inputValues;

    /**
     * save the analysis message so callers can look at it.
     */
    std::string                                     _analysisMsg;

private:

    std::string                                     GenerateResultString();

    /**
     * to be called AFTER we have a model.
     * populates all the ivars with values obtains from the model.
     */
    void                                            Initialize();

#ifndef NO_SBML
    void                                            InitializeFromModel(ls::SBMLmodel& oModel);
    void                                            BuildStoichiometryMatrixFromModel(ls::SBMLmodel& oModel);
#endif

    void                                            InitializeFromStoichiometryMatrix(DoubleMatrix& oMatrix);
    void                                            InitializeFromStoichiometryMatrix(DoubleMatrix& oMatrix,
                                                        std::vector<std::string>& speciesNames,
                                                        std::vector<std::string>& reactionNames,
                                                        std::vector<double>& inputValues);

    void                                            FreeMatrices();
    void                                            reorderNmatrix();
    void                                            computeNrMatrix();
    void                                            computeN0Matrix();
    void                                            computeLinkMatrix();
    void                                            computeConservedSums();
    void                                            computeConservedEntities();
    void                                            computeK0andKMatrices();
    bool                                            testConservationLaw_1();
    bool                                            testConservationLaw_2();
    bool                                            testConservationLaw_3();
    bool                                            testConservationLaw_4();
    bool                                            testConservationLaw_5();
    bool                                            testConservationLaw_6();

public:

    /*!    \example examples/cpp/loadstoichiometry.cpp
            This is an example of how to load a stoichiometry matrix and read test details.
     */
    /*!    \example examples/cpp/loadsbmlfromfile.cpp
            This is an example of how to load a SBML file and print structural analysis test results.
     */
    /*!    \example examples/cpp/printmatrices.cpp
            This example demonstrates how to access the matrices calculated by the library from C++
     */

    /*! \brief Load a new stoichiometry matrix.

            Loads the stoichiometry matrix into the library. To analyze the stoichiometry
            call one of the following:

            \li ::LibStructural_analyzeWithQR,
            \li ::LibStructural_analyzeWithLU,
            \li ::LibStructural_analyzeWithLUandRunTests,
            \li ::LibStructural_analyzeWithFullyPivotedLU or
            \li ::LibStructural_analyzeWithFullyPivotedLUwithTests

            \remarks if matrix labels are needed it is recommended to call ls::LibStructural::loadSpecies
            and ls::LibStructural::loadReactionNames after a call to this method.

            \param oMatrix the stoichiometry matrix to load
     */
    void loadStoichiometryMatrix (DoubleMatrix& oMatrix);

    /*! \brief Load species names and initial values.

            This function should be used whenever labeled matrices are important as these
            labels will be used in labeling the structural matrices. This function sets the species
            names (ids). It is also possible to provide an initial condition for each of
            the species. This will be used when calculating the conserved sums.

            \remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

            \param speciesNames a vector of species names (ids) to load
            \param speciesValues a vector of initial concentrations
     */
    void loadSpecies(std::vector< std::string > &speciesNames, std::vector<double> &speciesValues);

    /*! \brief Load reaction names.

            This function should be used whenever labeled matrices are important as these
            labels will be used in labeling the structural matrices. This function sets the reaction
            names (ids).

            \remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

            \param reactionNames a vector of reaction names (ids)
     */
    void loadReactionNames(std::vector< std::string > &reactionNames);

    /*! \brief Load a SBML model.
            \param sSBML the SBML string to load
            \return information about the loaded model
     */
    std::string loadSBML(std::string sSBML);

    /*! \brief Load a SBML model from the specified file.
            \param sFileName a file name to a SBML file to load
            \return information about the loaded model
     */
    std::string loadSBMLFromFile(std::string sFileName);

    /*! \brief Load an SBML model into the library and carry out tests using the internal test suite.
            \param sSBML the SBML file to load
            \return information about the loaded model and results of the internal test suite
     */
    std::string loadSBMLwithTests(std::string sSBML);

    /*! \brief Uses QR factorization for structural analysis

            This method performs the actual analysis of the stoichiometry matrix (loaded either
            via ls::LibStructural::loadStoichiometryMatrix or ls::LibStructural::loadSBML. Only after
            one of the analysis methods below has been called are the structural matrices (L0, K0...)
            available.

            \li ls::LibStructural::analyzeWithQR,
            \li ls::LibStructural::analyzeWithLU,
            \li ls::LibStructural::analyzeWithLUandRunTests,
            \li ls::LibStructural::analyzeWithFullyPivotedLU or
            \li ls::LibStructural::analyzeWithFullyPivotedLUwithTests


            \remarks This is the prefered method for structural analysis.

            \return a result string with information about the analysis process
     */
    std::string analyzeWithQR();
    /*! \brief Uses LU Decomposition for Conservation analysis

            This method performs the actual analysis of the stoichiometry matrix (loaded either
            via ls::LibStructural::loadStoichiometryMatrix or ls::LibStructural::loadSBML. Only after
            one of the analysis methods below has been called are the structural matrices (L0, K0...)
            available.

            \li ls::LibStructural::analyzeWithQR,
            \li ls::LibStructural::analyzeWithLU,
            \li ls::LibStructural::analyzeWithLUandRunTests,
            \li ls::LibStructural::analyzeWithFullyPivotedLU or
            \li ls::LibStructural::analyzeWithFullyPivotedLUwithTests

            \return a result string with information about the analysis process
     */
    std::string analyzeWithLU();
    /*! \brief Uses LU Decomposition for Conservation analysis

            This method performs the actual analysis of the stoichiometry matrix (loaded either
            via ls::LibStructural::loadStoichiometryMatrix or ls::LibStructural::loadSBML. Only after
            one of the analysis methods below has been called are the structural matrices (L0, K0...)
            available.

            \li ls::LibStructural::analyzeWithQR,
            \li ls::LibStructural::analyzeWithLU,
            \li ls::LibStructural::analyzeWithLUandRunTests,
            \li ls::LibStructural::analyzeWithFullyPivotedLU or
            \li ls::LibStructural::analyzeWithFullyPivotedLUwithTests

            This method additionally performs the integrated test suite and returns    those results.


            \return a result string with information about the analysis process
     */
    std::string analyzeWithLUandRunTests();
    /*! \brief Uses fully pivoted LU Decomposition for Conservation analysis

            This method performs the actual analysis of the stoichiometry matrix (loaded either
            via ls::LibStructural::loadStoichiometryMatrix or ls::LibStructural::loadSBML. Only after
            one of the analysis methods below has been called are the structural matrices (L0, K0...)
            available.

            \li ls::LibStructural::analyzeWithQR,
            \li ls::LibStructural::analyzeWithLU,
            \li ls::LibStructural::analyzeWithLUandRunTests,
            \li ls::LibStructural::analyzeWithFullyPivotedLU or
            \li ls::LibStructural::analyzeWithFullyPivotedLUwithTests


            \return a result string with information about the analysis process
     */
    std::string analyzeWithFullyPivotedLU();
    /*! \brief Uses fully pivoted LU Decomposition for Conservation analysis

            This method performs the actual analysis of the stoichiometry matrix (loaded either
            via ls::LibStructural::loadStoichiometryMatrix or ls::LibStructural::loadSBML. Only after
            one of the analysis methods below has been called are the structural matrices (L0, K0...)
            available.

            \li ls::LibStructural::analyzeWithQR,
            \li ls::LibStructural::analyzeWithLU,
            \li ls::LibStructural::analyzeWithLUandRunTests,
            \li ls::LibStructural::analyzeWithFullyPivotedLU or
            \li ls::LibStructural::analyzeWithFullyPivotedLUwithTests

            This method additionally performs the integrated test suite and returns    those results.

            \return a result string with information about the analysis process
     */
    std::string analyzeWithFullyPivotedLUwithTests();

    /*! \brief Returns the L0 Matrix.

            L0 is defined such that  L0 Nr = N0. L0 forms part of the link matrix, L.  N0 is the set of
            linear dependent rows from the lower portion of the reordered stoichiometry matrix.

     */
    DoubleMatrix* getL0Matrix();

    /*! \brief Returns the L0 Matrix row and column labels.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getL0MatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    /*! \brief Returns the Nr Matrix.

            The rows of the Nr matrix will be linearly independent.

     */
    DoubleMatrix* getNrMatrix();

    /*! \brief Returns the Nr Matrix row and column labels.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getNrMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //! Returns the Nr Matrix repartitioned into NIC (independent columns) and NDC (dependent columns).
    DoubleMatrix* getColumnReorderedNrMatrix();

    /*! \brief Returns the Nr Matrix row and column labels (repartitioned into NIC and NDC).
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getColumnReorderedNrMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //!  Returns the NIC Matrix (the set of linearly independent columns of Nr)
    DoubleMatrix* getNICMatrix();

    /*! \brief Returns the NIC Matrix row and column labels.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getNICMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //! Returns the NDC Matrix (the set of linearly dependent columns of Nr).
    DoubleMatrix* getNDCMatrix();
    /*!  \brief Returns the NDC Matrix row and column labels.

            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getNDCMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    /*! \brief Returns the N0 Matrix.

            The N0 matrix is the set of linearly dependent rows of N where L0 Nr = N0.
     */
    DoubleMatrix* getN0Matrix();

    /*! \brief Returns the N0 Matrix row and column labels.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getN0MatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    /*! \brief Returns L, the Link Matrix, left nullspace (aka nullspace of the transpose Nr).

            L will have the structure, [I L0]', such that L Nr  = N
     */
    DoubleMatrix* getLinkMatrix();

    /*! \brief Returns the row and column labels for the Link Matrix, L
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getLinkMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    /*! \brief Returns the K0 Matrix.

            K0 is defined such that K0 = -(NIC)^-1 NDC, or equivalently, [NDC NIC][I K0]' = 0 where [NDC NIC] = Nr
     */
    DoubleMatrix* getK0Matrix();

    /*! \brief  Returns the K0 Matrix row and column labels.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getK0MatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    /*! \brief Returns the K matrix (right nullspace of Nr)

            The K matrix has the structure, [I K0]'
     */
    DoubleMatrix* getKMatrix();

    /*! \brief  Returns the K matrix row and column labels.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getKMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    /*! \brief Returns Gamma, the conservation law array.

            Each row represents a single conservation law where the column indicate the
            participating molecular species. The number of rows is therefore equal to the
            number of conservation laws. Columns are ordered according to the rows in the
            reordered stoichiometry matrix, see ls::LibStructural::getReorderedSpeciesId and
            ls::LibStructural::getReorderedStoichiometryMatrix.

            Gamma is calculated based on: Gamma = [ -L0 I ]


     */
    DoubleMatrix* getGammaMatrix();


    /*! \brief Returns Gamma, the conservation law array.

            Each row represents a single conservation law where the column indicate the
            participating molecular species. The number of rows is therefore equal to the
            number of conservation laws.

            In this case the Gamma Matrix is recalculated for the given stoichiometry matrix.

            Gamma is calculated based on R =   GaussJordan ( [ stoichiometry  I ] ), where
            R has the form

                           R = [ R11 R12
                                  0  GAMMA ]


            The RowLabels should be an increasing number, to numerate the conservation law, the
            column label will be the same label as the stoichiometry matrix;

            \param stoichiometry the stoichiometry matrix that will be used to calculate gamma



     */
    DoubleMatrix* getGammaMatrixGJ(DoubleMatrix &stoichiometry);

    /*! \brief try and find a positive Gamma Matrix

            This function uses ls::LibStructural::getGammaMatrixGJ on
            row permutations of the given stoichiometries to find a positive gamma
            matrix.

            \param stoichiometry the stoichiometry matrix that will be used to calculate gamma
            \param rowLabels as the rows will be permutated b this method, the initial row names should
                             be provided. This vector will be permuted and will present the column
                             labels for the gamma matrix.
     */
    DoubleMatrix* findPositiveGammaMatrix(DoubleMatrix &stoichiometry,
            std::vector< std::string> &rowLabels);


    /*! \brief Returns the row and column labels for Gamma, the conservation law array.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getGammaMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //! Returns the original, unaltered stoichiometry matrix.
    DoubleMatrix* getStoichiometryMatrix();

    /*! \brief Returns the row and column labels for the original and unaltered stoichiometry matrix.
            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getStoichiometryMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //! Returns the reordered stoichiometry matrix (row reordered stoichiometry matrix, columns are not reordered!)
    DoubleMatrix* getReorderedStoichiometryMatrix();

    /*! \brief Returns the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getReorderedStoichiometryMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //! Returns the fully reordered stoichiometry matrix (row and column reordered stoichiometry matrix)
    DoubleMatrix* getFullyReorderedStoichiometryMatrix();

    /*! \brief Returns the row and column labels for the fully reordered stoichiometry matrix (row and column reordered stoichiometry matrix)

            \param oRows a string vector that will be overwritten to hold the row labels
            \param oCols a string vector that will be overwritten to hold the column labels.
     */
    void getFullyReorderedStoichiometryMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols );

    //! Returns the reordered list of molecular species. (choosing the SBML Id if possible )
    std::vector< std::string > getReorderedSpecies() const;

    //!  Returns the unordered list of species Ids
    std::vector< std::string > getSpecies() const;

    //! Returns the reordered list of molecular species.  (choosing the SBML Name if possible )
    std::vector< std::string > getReorderedSpeciesNamesList();

    //! Returns the list of linearly independent species
    std::vector< std::string > getIndependentSpecies() const;

    //! Returns the actual names of the linearly independent species
    std::vector< std::string > getIndependentSpeciesNamesList();

    //! Returns the list of dependent species
    std::vector< std::string > getDependentSpecies() const;

    //! Returns the actual names of the dependent species
    std::vector< std::string > getDependentSpeciesNamesList();

    //! Returns the list of Reactions
    std::vector< std::string > getReactions();

    //! Returns the list of independent reactions
    std::vector< std::string > getIndependentReactionIds();

    //! Returns the list of dependent reactions
    std::vector< std::string > getDependentReactionIds();

    //! Returns actual names of the Reactions
    std::vector< std::string > getReactionsNamesList();

    //! Returns the reordered list of reactions
    std::vector< std::string > getReorderedReactions();

    //! Returns algebraic expressions for conserved cycles
    std::vector< std::string > getConservedLaws();

    //! Returns values for conservation laws using the current initial conditions
    std::vector< double > getConservedSums();

    //! Returns Initial Conditions used in the model
    std::vector< std::pair <std::string, double> > getInitialConditions();

    /*! \brief Validates structural matrices.

            Calling this method will run the internal test suite against the structural
            matrices those tests include:\n

            \li Test 1 : Gamma*N = 0 (Zero matrix)
            \li Test 2 : Rank(N) using SVD (5) is same as m0 (5)
            \li Test 3 : Rank(NR) using SVD (5) is same as m0 (5)
            \li Test 4 : Rank(NR) using QR (5) is same as m0 (5)
            \li Test 5 : L0 obtained with QR matches Q21*inv(Q11)
            \li Test 6 : N*K = 0 (Zero matrix)
     */
    std::vector< std::string > validateStructuralMatrices();

    //! Return Return Details about validation tests.
    std::string getTestDetails();

    /*! \brief Returns the name of the model.

            Returns the name of the model if SBML model has Name-tag, otherwise it returns the
            SBML id. If only a stoichiometry matrix was loaded 'untitled' will be returned.
     */
    std::string getModelName();

    //! Returns the total number of species
    int getNumSpecies();
    //! Returns the number of independent species
    int getNumIndSpecies();
    //! Returns the number of dependent species
    int getNumDepSpecies();
    //! Returns the total number of reactions
    int getNumReactions();
    //! Returns the number of independent reactions
    int getNumIndReactions();
    //! Returns the number of dependent reactions
    int getNumDepReactions();

    //! Returns rank of stoichiometry matrix
    int getRank();
    //! Returns the number of nonzero values in Stoichiometry matrix
    double getNmatrixSparsity();
    /*! \brief Set user specified tolerance

            This function sets the tolerance used by the library to determine what value
            is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero
            and will be neglected.

            \param dTolerance Sets the tolerance used by the library to determine a  value close to zero
     */
    void setTolerance(double dTolerance);

    /*! \brief Returns the currently used tolerance

            This function returns the tolerance currently used by the library to determine what value
            is considered as zero. Any value with absolute value smaller than this tolerance is considered zero
            and will be neglected.
     */
    double getTolerance();


public:
    /**
     * constructor of a new empty instance of LibStructural
     */
    LibStructural();

    /**
     * construct a new LibStructural from an sbml string.
     *
     * This is the primary to create structural analyis from sbml models. This takes
     * the SBML string as an input . This could be in SBML level 1 or SBML level 2 format.
     * The users should check the validity of the SBML files before loading it into the method.
     * Conservation analysis is carried out using Householder QR algorithm to generate the L0
     * matrix, from which other matrices of interest can subsequently be generated. The results
     * of the analysis are output as a string and are also accessible from other methods in
     * the API.
     *
     * \param sSBML the SBML string to load
     */
    LibStructural(const std::string &sbml);

    /**
     * Attach a LibStructural to an existing sbml model.
     *
     * Performs all of the same analysis as LibStructural::LibStructural(std::string sbml);
     */
    LibStructural(const libsbml::Model *model);

    ~LibStructural();

    std::string getAnalysisMsg();
};    //End of class

} //namespace

#endif


