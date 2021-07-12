#include <iostream>
#include <string>

#include <sbml/SBMLTypes.h>
#include <sbml/common/extern.h>


using namespace std;
LIBSBML_CPP_NAMESPACE_USE


int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << endl << "Usage: readSBML filename" << endl << endl;
    return 1;
  }

  const char* filename   = argv[1];

  SBMLDocument* document;
  SBMLReader reader;
  
  document = reader.readSBML(filename);

  unsigned int errors = document->getNumErrors();

  cout << endl;
  cout << " Reading filename: " << filename << endl;
  cout << " Validation error(s): " << errors << endl;
  cout << endl;
//   document->printErrors(cerr);

  Model* model = document->getModel();
  int n_species = model->getNumSpecies();
  int n_reactions = model->getNumReactions();
  int n_parmas = model->getNumParameters();
  string model_id = model->getId();

  cout << endl;
  cout << " Model ID: " << model_id << endl;
  cout << " - Total species: " << n_species << endl;
  cout << " - Total reactions: " << n_reactions << endl;
  cout << " - Total parmas: " << n_parmas << endl;
  cout << endl;

  delete document;
  return errors;
}


