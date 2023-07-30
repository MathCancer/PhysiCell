#ifndef __PhysiMeSS_fibre_h__
#define __PhysiMeSS_fibre_h__

#include "PhysiMeSS_agent.h"

class PhysiMeSS_Cell;

bool isFibre(PhysiCell::Cell* pCell);

bool isFibre(PhysiCell::Cell_Definition * cellDef);

std::vector<PhysiCell::Cell_Definition*>* getFibreCellDefinitions();

class PhysiMeSS_Fibre : public PhysiMeSS_Agent
{
    private:
    public:
    std::vector<PhysiCell::Cell*> fibres_crosslinkers;
    std::vector<double> fibres_crosslink_point;
    
    double mLength;
    double mRadius;
    int X_crosslink_count;
    int fail_count;
    
    PhysiMeSS_Fibre();
    ~PhysiMeSS_Fibre() {};
    void assign_fibre_orientation() ;
    
    void check_out_of_bounds(std::vector<double>& position);
    virtual void add_potentials_from_fibre(PhysiMeSS_Fibre* other_fibre);
    virtual void add_potentials_from_cell(PhysiMeSS_Cell* cell);

    void register_fibre_voxels();
    void deregister_fibre_voxels();
    
    std::vector<double> nearest_point_on_fibre(std::vector<double> point, std::vector<double> &displacement);

    void check_fibre_crosslinks(PhysiMeSS_Fibre *fibre_neighbor);
    void add_crosslinks();

};

#endif