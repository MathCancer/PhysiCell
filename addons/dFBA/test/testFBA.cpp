#include <iostream>
#include <vector>

#include "../src/FBA_model.h"

using namespace std;

int
main (int argc, const char *argv[])
{

    if (argc != 2)
    {
        cout << endl << "Usage: readSBML filename" << endl << endl;
        return 1;
    }
    const char *sbml_fileame = argv[1];

    FBA_model *model = new FBA_model();
    std::cout << "Reading SBML model from: " << sbml_fileame << " ";
    model->readSBMLModel(sbml_fileame);
    std::cout << "Ok!" << std::endl;
    std::cout << "Initializing LP model: ";
    model->initLpModel();
    std::cout << "Ok!" << std::endl;

    model->writeLp("iJO1366");

    for(int i=1; i<10; i++){
      std::cout << "Running FBA: ";

      float bound = -1.0 * i;
      model->setReactionLowerBound("R_EX_glc__D_e", bound);

      model->runFBA();
      std::cout << "Uppdating lower R_EX_glc__D_e bound: " << bound << std::endl;

      if (model->getSolutionStatus ())
      {
	  std::cout << "OPTIMAL SOLUTION FOUND" << std::endl;
	  float fopt = model->getObjectiveValue();
	  std::cout << "Objective value: " << fopt << std::endl;
	  /*
	  for (Reaction* rxn :model->getListOfReactions ())
	  {
	      double flux = rxn->getFluxValue();
		  if ( std::abs(flux) < 1e-7 )
		  continue;
	      std::cout << rxn->getId() << ": " << flux << std::endl;
	  }
	  */
      }
      else
      {
	  std::cout << "NO SOLUTION FOUND" << std::endl;
      }
    }

    delete model;
    return 0;
}
