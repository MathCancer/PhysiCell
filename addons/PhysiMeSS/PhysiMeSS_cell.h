#ifndef __PhysiMeSS_cell_h__
#define __PhysiMeSS_cell_h__

#include "PhysiMeSS_agent.h"
#include "PhysiMeSS_fibre.h"

class PhysiMeSS_Fibre;

class PhysiMeSS_Cell : public PhysiMeSS_Agent 
{
    private:
    public:
    
    int stuck_counter;
    int unstuck_counter;
    
    PhysiMeSS_Cell();
    ~PhysiMeSS_Cell() {};
    
    virtual void add_potentials_from_fibre(PhysiMeSS_Fibre* fibre);
    
    void register_fibre_voxels();
    void deregister_fibre_voxels();
    
    void force_update_motility_vector(double dt_);
    
    virtual void degrade_fibre(PhysiMeSS_Fibre* pFibre); 

};

#endif