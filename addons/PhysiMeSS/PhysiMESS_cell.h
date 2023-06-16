#ifndef __PhysiMESS_cell_h__
#define __PhysiMESS_cell_h__

#include "PhysiMESS_agent.h"
#include "PhysiMESS_fibre.h"

class PhysiMESS_Fibre;

// using namespace PhysiCell;

class PhysiMESS_Cell : public PhysiMESS_Agent 
{
    private:
    public:
    
    int stuck_counter;
    int unstuck_counter;
    
    PhysiMESS_Cell();
    ~PhysiMESS_Cell() {};
    
    void add_potentials_from_fibre(PhysiMESS_Fibre* fibre);
    
    void register_fibre_voxels();
    void deregister_fibre_voxels();
    
    void force_update_motility_vector(double dt_);
    
    virtual void degrade_fibre(PhysiMESS_Fibre* pFibre); 

};

#endif