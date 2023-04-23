#ifndef __PhysiMESS_fibre_h__
#define __PhysiMESS_fibre_h__

#include "PhysiMESS_agent.h"

bool isFibre(PhysiCell::Cell* pCell);

bool isFibre(PhysiCell::Cell_Definition * cellDef);

class PhysiMESS_Fibre : public PhysiMESS_Agent
{
    private:
    public:
    std::vector<PhysiCell::Cell*> fibres_crosslinkers;
    std::vector<double> fibres_crosslink_point;
    
    double mLength;
    double mRadius;
    int X_crosslink_count;
    int fail_count;
    
    PhysiMESS_Fibre();
    ~PhysiMESS_Fibre() {};
    void assign_fibre_orientation() ;
    
    void check_out_of_bounds(std::vector<double>& position);
    
    void register_fibre_voxels();
    void deregister_fibre_voxels();
    
    std::vector<double> nearest_point_on_fibre(std::vector<double> point, std::vector<double> &displacement);

    void check_fibre_crosslinks(PhysiMESS_Fibre *fibre_neighbor);
    void add_crosslinks();

};

#endif