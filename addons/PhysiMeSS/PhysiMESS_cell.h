#ifndef __PhysiMESS_cell_h__
#define __PhysiMESS_cell_h__

#include "PhysiMESS_agent.h"

// using namespace PhysiCell;

class PhysiMESS_Cell : public PhysiMESS_Agent 
{
    private:
    public:
    
    int stuck_counter;
    int unstuck_counter;
    
    PhysiMESS_Cell();
    ~PhysiMESS_Cell() {};
    
    void register_fibre_voxels();
    void deregister_fibre_voxels();
    
    void force_update_motility_vector(double dt_);

};

#endif