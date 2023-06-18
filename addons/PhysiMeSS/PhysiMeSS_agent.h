#ifndef __PhysiMeSS_agent_h__
#define __PhysiMeSS_agent_h__


#include <list>

#include "../../core/PhysiCell.h"

// using namespace PhysiCell;

class PhysiMeSS_Agent : public PhysiCell::Cell 
{
    private:
    public:
    
    std::vector<PhysiCell::Cell*> physimess_neighbors; 
    std::list<int> physimess_voxels;
    
    PhysiMeSS_Agent();
    virtual ~PhysiMeSS_Agent(){};
    
    virtual void register_fibre_voxels() {};
    virtual void deregister_fibre_voxels() {};
    
    std::list<int> find_agent_voxels();
    void find_agent_neighbors();
    
};

#endif