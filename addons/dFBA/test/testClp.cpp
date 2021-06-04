#include <iostream>
#include <string>

#include <coin-or/CoinPackedMatrix.hpp>
#include <coin-or/CoinPackedVector.hpp>
#include <coin-or/ClpSimplex.hpp>

using namespace std;

/*
    minimise
        4 x0 + 5 x1 + 6 x2
    subject to
        x0 + x1 >= 11
        x0 - x1 <= 5
        x2 - x0 - x1 = 0
        7 x0 - 12 x1 >= 35 
        x0 >= 0 x1 >= 0 x2 >= 0

Solution:
 objective value: 113
 primal
    x0: 8
    x1: 3
    x2: 11
*/


int main()
{
    const int n_rows = 4;
    const int n_cols = 3;
    static double BIGBOUND = 1000000.0;

    // set z_sense = 1 to minimize a function
    int z_sense = 1;
    double Z[n_cols] = {4.0, 5.0, 6.0};
    double A[n_rows][n_cols] = {{1.0,1.0,0.0}, {1.0,-1.0,0.0}, {-1.0,-1.0,1.0}, {7.0,12.0,0.0}};
    double row_bounds[n_rows][2] = {{11.0,BIGBOUND}, {-BIGBOUND,5.0}, {0.0,0.0}, {35.0,BIGBOUND}};
    

    ClpSimplex* lp_model = new ClpSimplex();

    CoinPackedMatrix matrix;
    matrix.setDimensions(n_rows, 0);

    double* row_lb = new double[n_rows]; //the row lower bounds
    double* row_ub = new double[n_rows]; //the row upper bounds
    double* col_lb = new double[n_cols]; //the column lower bounds
    double* col_ub = new double[n_cols]; //the column upper bounds
    double* objective = new double[n_cols]; //the objective coefficients

    for(int i=0; i<n_rows; i++) {
        row_lb[i] = row_bounds[i][0];
        row_ub[i] = row_bounds[i][1];

    }
    
    for(int j=0; j<n_cols; j++) {
        col_lb[j] = 0.0;
        col_ub[j] = BIGBOUND;
        
        objective[j] = Z[j];
        
        CoinPackedVector col;
        for(int i=0; i<n_rows; i++)
            col.insert(i, A[i][j]);

        matrix.appendCol(col);
    }

    lp_model->loadProblem(matrix, col_lb, col_ub, objective, row_lb, row_ub);
    lp_model->setOptimizationDirection(z_sense);
    
    const char* filename = "test/test_problem";
    std::cout << "- Saving problem in lp format in " << filename << std::endl;
    lp_model->writeLp(filename, "lp");

    std::cout << "- Solving LP problem" << std::endl;
    lp_model->initialSolve();
    lp_model->primal();
    
    if ( lp_model->isProvenOptimal() ) {
        double objVal = lp_model->getObjValue();
        std::cout <<  std::endl;
        std::cout << "- Optimal solution found! " << std::endl;
        std::cout << "- Objective value: " << objVal << std::endl;

        double * columnPrimal = lp_model->primalColumnSolution();
        objective = lp_model->objective();
        for (int i = 0; i < n_cols; i++) {
            double value = columnPrimal[i];
            std::cout << "     - x" << i << " (cost " << objective[i] << "): " << value << std::endl;
        }
        
    }else {
        std::cout << "- INFEASIBLE PROBLEM!" << std::endl;

    }

    delete col_lb;
    delete col_ub;
    delete row_lb;
    delete row_ub;
    delete objective;
    delete lp_model;

    return 0;
}