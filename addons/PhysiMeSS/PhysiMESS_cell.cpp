#include "PhysiMESS_cell.h"

PhysiMESS_Cell::PhysiMESS_Cell()
{
    stuck_counter = 0;
    unstuck_counter = 0;
}


void PhysiMESS_Cell::register_fibre_voxels() 
{
    //a cell will be in one voxel
    int voxel = this->get_container()->underlying_mesh.nearest_voxel_index(this->position);
    physimess_voxels.push_back(voxel);

}

void PhysiMESS_Cell::deregister_fibre_voxels() {

    //only do this for fibres
    return;
}


// !!! PHYSIMESS CODE BLOCK START !!! //
void PhysiMESS_Cell::force_update_motility_vector(double dt_) {

    if (!this->phenotype.motility.is_motile) {
        this->phenotype.motility.motility_vector.assign(3, 0.0);
        return;
    }

    // force cell to update its motility because it is stuck
    // choose a uniformly random unit vector
    double temp_angle = 6.28318530717959 * PhysiCell::UniformRandom();
    double temp_phi = 3.1415926535897932384626433832795 * PhysiCell::UniformRandom();

    double sin_phi = sin(temp_phi);
    double cos_phi = cos(temp_phi);

    if (this->phenotype.motility.restrict_to_2D) {
        sin_phi = 1.0;
        cos_phi = 0.0;
    }

    std::vector<double> randvec;
    randvec.resize(3, sin_phi);

    randvec[0] *= cos(temp_angle); // cos(theta)*sin(phi)
    randvec[1] *= sin(temp_angle); // sin(theta)*sin(phi)
    randvec[2] = cos_phi; //  cos(phi)

    // if the update_bias_vector function is set, use it
    /*if (functions.update_migration_bias) {
    functions.update_migration_bias(this, phenotype, dt_);
    }*/

    //phenotype.motility.motility_vector *= -1.0;//phenotype.motility.migration_bias_direction; // motiltiy = bias_vector
    //phenotype.motility.motility_vector *= phenotype.motility.migration_bias; // motility = bias*bias_vector

    double one_minus_bias = 1.0;// - phenotype.motility.migration_bias;

    axpy(&(this->phenotype.motility.motility_vector), one_minus_bias,randvec); // motility = (1-bias)*randvec + bias*bias_vector

    normalize(&(this->phenotype.motility.motility_vector));

    this->phenotype.motility.motility_vector *= this->phenotype.motility.migration_speed;

    return;
}
