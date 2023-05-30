#include "PhysiMeSS.h"

#include <algorithm>
#include <iterator> 


static double last_update_time = -mechanics_dt;

void remove_physimess_out_of_bounds_fibres()
{
    for (auto* cell : *all_cells) {
        if (isFibre(cell) && static_cast<PhysiMESS_Fibre*>(cell)->fail_count >= 10)
        {
            std::cout << "I failed to place " << cell->type_name << " " 
                      << cell->ID << " in the domain - I am deleting agent " 
                      << std::endl;
            delete_cell(cell);
        }
    }
}

void add_potentials_cell_to_fibre(PhysiMESS_Cell* pCell, PhysiMESS_Fibre* pFibre)
{
    
    double distance = 0.0;
    pFibre->nearest_point_on_fibre(pCell->position, pCell->displacement);
    for (int index = 0; index < 3; index++) {
        distance += pCell->displacement[index] * pCell->displacement[index];
    }
    distance = std::max(sqrt(distance), 0.00001);
    /*if( this->phenotype.motility.is_motile) {
        std::cout << " determining distance from " << this->type_name << " " << this->ID << " to "
                    << (*other_agent).type_name << " " << (*other_agent).ID
                    << "   the distance is " << distance << std::endl;
    }*/

    // as per PhysiCell
    static double simple_pressure_scale = 0.027288820670331;

    // check distance relative repulsion and adhesion distances
    // cell should repel from a fibre if it comes within cell radius plus fibre radius (note fibre radius ~2 micron)
    double R = pCell->phenotype.geometry.radius + pFibre->mRadius;
    // cell should feel adhesion over
    double max_interactive_distance =
            pCell->phenotype.mechanics.relative_maximum_adhesion_distance * pCell->phenotype.geometry.radius +
            pFibre->phenotype.mechanics.relative_maximum_adhesion_distance *
            pFibre->mRadius;

    // First Repulsion as per PhysiCell
    double temp_r = 0;
    if (distance > R) {
        temp_r = 0;
    } else {
        // temp_r = 1 - distance/R;
        temp_r = -distance; // -d
        temp_r /= R; // -d/R
        temp_r += 1.0; // 1-d/R
        temp_r *= temp_r; // (1-d/R)^2

        // add the relative pressure contribution NOT SURE IF NEEDED
        pCell->state.simple_pressure += (temp_r / simple_pressure_scale);

        double effective_repulsion = sqrt(pCell->phenotype.mechanics.cell_cell_repulsion_strength *
                                            pFibre->phenotype.mechanics.cell_cell_repulsion_strength);
        temp_r *= effective_repulsion;
    }

    if (fabs(temp_r) < 1e-16) { return; }
    temp_r /= distance;

    axpy(&(pCell->velocity), temp_r, pCell->displacement);

    //Then additional repulsion/adhesion as per Cicely's code
    double fibre_adhesion = 0;
    double fibre_repulsion = 0;
    if (distance < max_interactive_distance) {
        const std::vector<double> previous_velocity = pCell->get_previous_velocity();
        double cell_velocity_dot_fibre_direction = 0.;
        for (unsigned int j = 0; j < 3; j++) {
            cell_velocity_dot_fibre_direction += pFibre->state.orientation[j] * previous_velocity[j];
        }
        double cell_velocity = 0;
        for (unsigned int j = 0; j < pCell->velocity.size(); j++) {
            cell_velocity += previous_velocity[j] * previous_velocity[j];
        }
        cell_velocity = std::max(sqrt(cell_velocity), 1e-8);

        double p_exponent = 1.0;
        double q_exponent = 1.0;
        double xi = fabs(cell_velocity_dot_fibre_direction) / (cell_velocity);
        double xip = pow(xi, p_exponent);
        double xiq = pow((1 - xi * xi), q_exponent);

        fibre_adhesion = parameters.doubles("vel_adhesion") * xip *
                            (1 - cell_velocity / parameters.doubles("cell_velocity_max"));

        fibre_repulsion = parameters.doubles("vel_contact") * xiq;

        axpy(&(pCell->velocity), fibre_adhesion, pFibre->state.orientation);
        naxpy(&(pCell->velocity), fibre_repulsion, previous_velocity);

        // Fibre degradation by cell - switched on by flag fibre_degradation
        double stuck_threshold = parameters.doubles("fibre_stuck_time");
        double pressure_threshold = parameters.doubles("fibre_pressure_threshold");
        if (parameters.bools("fibre_degradation") && (pCell->stuck_counter >= stuck_threshold
                                                      || pCell->state.simple_pressure > pressure_threshold)) {
            if (pCell->stuck_counter >= stuck_threshold){
                std::cout << "Cell " << pCell->ID << " is stuck at time " << PhysiCell_globals.current_time
                          << " near fibre " << pFibre->ID  << std::endl;;
            }
            if (pCell->state.simple_pressure > pressure_threshold){
                std::cout << "Cell " << pCell->ID << " is under pressure of " << pCell->state.simple_pressure << " at "
                          << PhysiCell_globals.current_time << " near fibre " << pFibre->ID  << std::endl;;
            }
            pCell->displacement *= -1.0/distance;
            double dotproduct = dot_product(pCell->displacement, pCell->phenotype.motility.motility_vector);
            if (dotproduct >= 0) {
                double rand_degradation = UniformRandom();
                double prob_degradation = parameters.doubles("fibre_degradation_rate");
                if (pCell->state.simple_pressure > pressure_threshold){
                    prob_degradation *= pCell->state.simple_pressure;
                }
                if (rand_degradation <= prob_degradation) {
                    //std::cout << " --------> fibre " << (*other_agent).ID << " is flagged for degradation " << std::endl;
                    // (*other_agent).parameters.degradation_flag = true;
                    pFibre->flag_for_removal();
                    std::cout << "Degrading fibre agent " << pFibre->ID << " using flag for removal !!" << std::endl;
                    pCell->stuck_counter = 0;
                }
            }
        }
    }
}

void add_potentials_fibre_to_cell(PhysiMESS_Fibre* pFibre, PhysiMESS_Cell* other_agent) 
{
    // PhysiMESS_Fibre* pFibre = static_cast<PhysiMESS_Fibre*>(pCell);
    // fibres only get pushed or rotated by motile cells
    if (!other_agent->phenotype.motility.is_motile || pFibre->X_crosslink_count >= 2) {
        return;
    }

    double distance = 0.0;
    pFibre->nearest_point_on_fibre(other_agent->position, pFibre->displacement);
    for (int index = 0; index < 3; index++) {
        distance += pFibre->displacement[index] * pFibre->displacement[index];
    }
    distance = std::max(sqrt(distance), 0.00001);
    // fibre should only interact with cell if it comes within cell radius plus fibre radius (note fibre radius ~2 micron)
    double R = pFibre->phenotype.geometry.radius + pFibre->mRadius;
    if (distance <= R) {
        std::vector<double> point_of_impact(3, 0.0);
        for (int index = 0; index < 3; index++) {
            point_of_impact[index] = (*other_agent).position[index] - pFibre->displacement[index];
        }
        // cell-fibre pushing only if fibre no crosslinks
        if (pFibre->X_crosslink_count == 0) {
            //fibre pushing turned on
            if (parameters.bools("fibre_pushing")) {
                // as per PhysiCell
                static double simple_pressure_scale = 0.027288820670331;
                // temp_r = 1 - distance/R;
                double temp_r = 0;
                temp_r = -distance;
                temp_r /= R;
                temp_r += 1.0;
                temp_r *= temp_r;
                // add the relative pressure contribution NOT SURE IF NEEDED
                pFibre->state.simple_pressure += (temp_r / simple_pressure_scale);

                double effective_repulsion = sqrt(pFibre->phenotype.mechanics.cell_cell_repulsion_strength *
                                                    (*other_agent).phenotype.mechanics.cell_cell_repulsion_strength);
                temp_r *= effective_repulsion;

                if (fabs(temp_r) < 1e-16) { return; }
                temp_r /= distance;
                naxpy(&pFibre->velocity, temp_r, pFibre->displacement);
            }

            // fibre rotation turned on (2D)
            if (parameters.bools("fibre_rotation")) {
                std::vector<double> old_orientation(3, 0.0);
                for (int i = 0; i < 2; i++) {
                    old_orientation[i] = pFibre->state.orientation[i];
                }

                double moment_arm_magnitude = sqrt(
                        point_of_impact[0] * point_of_impact[0] + point_of_impact[1] * point_of_impact[1]);
                double impulse = parameters.doubles("fibre_sticky")*(*other_agent).phenotype.motility.migration_speed * moment_arm_magnitude;
                double fibre_length = 2 * pFibre->mLength;
                double angular_velocity = impulse / (0.5 * fibre_length * fibre_length);
                double angle = angular_velocity;
                pFibre->state.orientation[0] = old_orientation[0] * cos(angle) - old_orientation[1] * sin(angle);
                pFibre->state.orientation[1] = old_orientation[0] * sin(angle) + old_orientation[1] * cos(angle);
                normalize(&pFibre->state.orientation);
            }
        }

        // fibre rotation around other fibre (2D only and fibres intersect at a single point)
        if (parameters.bools("fibre_rotation") && pFibre->X_crosslink_count == 1) {
            double distance_fibre_centre_to_crosslink = 0.0;
            std::vector<double> fibre_centre_to_crosslink(3, 0.0);
            for (int i = 0; i < 2; i++) {
                fibre_centre_to_crosslink[i] = pFibre->fibres_crosslink_point[i]-pFibre->position[i];
                distance_fibre_centre_to_crosslink += fibre_centre_to_crosslink[i]*fibre_centre_to_crosslink[i];
            }
            distance_fibre_centre_to_crosslink = sqrt(distance_fibre_centre_to_crosslink);

            std::vector<double> old_orientation(3, 0.0);
            for (int i = 0; i < 2; i++) {
                old_orientation[i] = pFibre->state.orientation[i];
            }
            double moment_arm_magnitude = sqrt(
                    point_of_impact[0] * point_of_impact[0] + point_of_impact[1] * point_of_impact[1]);
            double impulse = parameters.doubles("fibre_sticky")*(*other_agent).phenotype.motility.migration_speed * moment_arm_magnitude;
            double fibre_length = 2 * pFibre->mLength;
            double angular_velocity = impulse / (0.5 * fibre_length * fibre_length);
            double angle = angular_velocity;
            pFibre->state.orientation[0] = old_orientation[0] * cos(angle) - old_orientation[1] * sin(angle);
            pFibre->state.orientation[1] = old_orientation[0] * sin(angle) + old_orientation[1] * cos(angle);
            normalize(&pFibre->state.orientation);
            pFibre->position[0] = pFibre->fibres_crosslink_point[0]-distance_fibre_centre_to_crosslink*pFibre->state.orientation[0];
            pFibre->position[1] = pFibre->fibres_crosslink_point[1]-distance_fibre_centre_to_crosslink*pFibre->state.orientation[1];
        }
    }

    return;
}
void add_potentials_fibre_to_fibre(PhysiMESS_Fibre* pFibre, PhysiMESS_Fibre* other_fibre) 
{
    /* probably want something here to model tension along fibres
        * this will be strong tension along the fibre for those fibres with a crosslink
        * and weak tension with background ECM */
    return;
}

void physimess_update_cell_velocity( Cell* pCell, Phenotype& phenotype, double dt)
{
    
    // !!! PHYSIMESS CODE BLOCK START !!! //
    double movement_threshold = 0.05;
    if (!isFibre(pCell) && phenotype.motility.is_motile) {
	
        // Here I changed this, because here we don't have access to the old position, and I didn't want to track the old position
        // So I'm using the previous velocity, which is not exactly the same (because of Adams-Bashforth), but is a good proxy
        // if (dist(pCell->old_position, pCell->position) < movement_threshold) {
        if (norm(pCell->get_previous_velocity())*mechanics_dt < movement_threshold) {
			static_cast<PhysiMESS_Cell*>(pCell)->stuck_counter++;
        } else {
            static_cast<PhysiMESS_Cell*>(pCell)->stuck_counter = 0;
        }
    }
    // !!! PHYSIMESS CODE BLOCK END !!! //
    
    
	if( pCell->functions.add_cell_basement_membrane_interactions )
	{
		pCell->functions.add_cell_basement_membrane_interactions(pCell, phenotype,dt);
	}
	
	pCell->state.simple_pressure = 0.0;
	pCell->state.neighbors.clear(); // new 1.8.0

    // !!! PHYSIMESS CODE BLOCK START !!! //
    
    if (!isFibre(pCell)) {
        //First check the neighbors in my current voxel
        std::vector<Cell*>::iterator neighbor;
        std::vector<Cell*>::iterator end = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].end();
        for(neighbor = pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()].begin(); neighbor != end; ++neighbor)
        {
            if (!isFibre(*neighbor)) {
                pCell->add_potentials(*neighbor);
            }
        }
        std::vector<int>::iterator neighbor_voxel_index;
        std::vector<int>::iterator neighbor_voxel_index_end = 
            pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].end();

        for( neighbor_voxel_index = 
            pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()].begin();
            neighbor_voxel_index != neighbor_voxel_index_end; 
            ++neighbor_voxel_index )
        {
            if(!is_neighbor_voxel(pCell, pCell->get_container()->underlying_mesh.voxels[pCell->get_current_mechanics_voxel_index()].center, pCell->get_container()->underlying_mesh.voxels[*neighbor_voxel_index].center, *neighbor_voxel_index))
                continue;
            end = pCell->get_container()->agent_grid[*neighbor_voxel_index].end();
            for(neighbor = pCell->get_container()->agent_grid[*neighbor_voxel_index].begin();neighbor != end; ++neighbor)
            {
                if (!isFibre(*neighbor)) {
                    pCell->add_potentials(*neighbor);
                }
            }
        }
    } else {
        // Count crosslinks
        static_cast<PhysiMESS_Fibre*>(pCell)->X_crosslink_count = 0;
        if (static_cast<PhysiMESS_Fibre*>(pCell)->fibres_crosslinkers.size() > 0){
            static_cast<PhysiMESS_Fibre*>(pCell)->X_crosslink_count = static_cast<PhysiMESS_Fibre*>(pCell)->fibres_crosslinkers.size();
        }

    }

    // std::cout << " AGENT " << pCell->type_name << " " << pCell->ID << " has " ;
    //add potentials between pCell and its neighbors
    // std::vector<Cell*>::iterator neighbor;
    // std::vector<Cell*>::iterator end = static_cast<PhysiMESS_Agent*>(pCell)->physimess_neighbors.end();
    // //std::cout << pCell->state.neighbors.size() << " neighbors: " ;
    // for(neighbor = static_cast<PhysiMESS_Agent*>(pCell)->physimess_neighbors.begin(); neighbor != end; ++neighbor) {
    for (auto* neighbor : static_cast<PhysiMESS_Agent*>(pCell)->physimess_neighbors)
    {
        // std::cout << (*neighbor)->type_name << " " << (*neighbor)->ID << " " ;
        
        // if( this->ID == other_agent->ID )
        if( pCell != neighbor )
        { 
            if (!isFibre(pCell) && !isFibre(neighbor)) {
                //Already done above
                continue;
            } else 
            if (!isFibre(pCell) && isFibre(neighbor)) {
                add_potentials_cell_to_fibre(static_cast<PhysiMESS_Cell*>(pCell), static_cast<PhysiMESS_Fibre*>(neighbor));
            } else  if (isFibre(pCell) && !isFibre(neighbor)) {
                add_potentials_fibre_to_cell(static_cast<PhysiMESS_Fibre*>(pCell), static_cast<PhysiMESS_Cell*>(neighbor));
            } else if (isFibre(pCell) && isFibre(neighbor)) {
                add_potentials_fibre_to_fibre(static_cast<PhysiMESS_Fibre*>(pCell), static_cast<PhysiMESS_Fibre*>(neighbor));
            } else {
                std::cout << " WARNING: interaction between errant cell-types has been called : " << pCell->type_name << ", " << neighbor->type_name << std::endl;
                return;
            }
        }
    }
    // std::cout << std::endl;

    if (!isFibre(pCell)) {
        
        PhysiMESS_Cell* ppCell = static_cast<PhysiMESS_Cell*>(pCell);
        int stuck_threshold = 10;
        int unstuck_threshold = 1;

        if (ppCell->stuck_counter == stuck_threshold){
            // std::cout << "!HELP! cell " << pCell->ID << " gets stuck at time "
            // << PhysiCell_globals.current_time << std::endl;
            ppCell->stuck_counter = 0;
            ppCell->unstuck_counter = 1;
        }

        if (1 <= ppCell->unstuck_counter && ppCell->unstuck_counter < unstuck_threshold+1) {
            /*std::cout << " getting unstuck at time "
            << PhysiCell_globals.current_time << std::endl;*/
            ppCell->unstuck_counter++;
            ppCell->force_update_motility_vector(dt);
            ppCell->velocity += phenotype.motility.motility_vector;
        }
        else {
            pCell->update_motility_vector(dt);
            pCell->velocity += phenotype.motility.motility_vector;
        }

        if(ppCell->unstuck_counter == unstuck_threshold+1){
            ppCell->unstuck_counter = 0;
        }
        // !!! PHYSIMESS CODE BLOCK END !!! //
}
    return;

}

void physimess_mechanics( double dt ) 
{    
    static double dt_tolerance = 0.001 * dt; 
    if(fabs(((PhysiCell_globals.current_time - last_update_time)) - dt) < dt_tolerance)
    {
        last_update_time = PhysiCell_globals.current_time;
        
        #pragma omp parallel for
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            
            static_cast<PhysiMESS_Agent*>(pC)->physimess_voxels.clear();
            if( !pC->is_out_of_domain )
            {
                static_cast<PhysiMESS_Agent*>(pC)->register_fibre_voxels();
            }
        }

        #pragma omp parallel for
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            static_cast<PhysiMESS_Agent*>(pC)->physimess_neighbors.clear();
            if (isFibre(pC)) {
                static_cast<PhysiMESS_Fibre*>(pC)->fibres_crosslinkers.clear();
            }
            if( !pC->is_out_of_domain )
            {
                static_cast<PhysiMESS_Agent*>(pC)->find_agent_neighbors();
            }
        }

        #pragma omp parallel for
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            if( !pC->is_out_of_domain )
            {
                static_cast<PhysiMESS_Agent*>(pC)->deregister_fibre_voxels();
            }
        }
        
        // determine and add crosslinks
        #pragma omp parallel for
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            if (isFibre(pC)) {
                static_cast<PhysiMESS_Fibre*>(pC)->add_crosslinks();
            }
        }
    }
}


void fibre_agent_SVG(std::ofstream& os, PhysiCell::Cell* pC, double z_slice, std::vector<std::string> (*cell_coloring_function)(Cell*), double X_lower, double Y_lower) {
	// !!! PHYSIMESS CODE BLOCK START !!! //
	// place a rod if it's a fibre (note fibre already renamed here)
	if (isFibre(pC) ){
    
        PhysiMESS_Fibre* pFibre = static_cast<PhysiMESS_Fibre*>(pC);
		int crosslinks = pFibre->X_crosslink_count;
        if (crosslinks >= 3){
			// if fibre has cross-links different colour than if not
			Write_SVG_line(os, (pC->position)[0] - (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] - (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							(pC->position)[0] + (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] + (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							4.0, "darkblue");
		}
		else if (crosslinks == 2){
			// if fibre has cross-links different colour than if not
			Write_SVG_line(os, (pC->position)[0] - (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] - (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							(pC->position)[0] + (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] + (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							4.0, "blue");
		}
		else if (crosslinks == 1){
			// if fibre has cross-links different colour than if not
			Write_SVG_line(os, (pC->position)[0] - (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] - (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							(pC->position)[0] + (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] + (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							4.0, "steelblue");
		}
		else {
    		Write_SVG_line(os, (pC->position)[0] - (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] - (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							(pC->position)[0] + (pFibre->mLength) * (pC->state.orientation)[0] - X_lower,
							(pC->position)[1] + (pFibre->mLength) * (pC->state.orientation)[1] - Y_lower,
							4.0, "lightskyblue");
		}

	}
	else{
        standard_agent_SVG(os, pC, z_slice, cell_coloring_function, X_lower, Y_lower);
	}
}

void fibre_agent_legend(std::ofstream& os, Cell_Definition* cell_definition, double& cursor_x, double& cursor_y, std::vector<std::string> (*cell_coloring_function)(Cell*), double temp_cell_radius) {
	
	// switch to the cell type 
	Cell C; 
	C.convert_to_cell_definition( *cell_definition );
	
	// get the colors using the current coloring function 
	std::vector<std::string> colors = cell_coloring_function(&C); 	

	// place the label 
	// !!! PHYSIMESS CODE BLOCK START !!! //
	// place a rod if it's a fibre (note fibre not yet renamed)
	if (isFibre(&C)) {
		//Write_SVG_fibre(os, cursor_x, cursor_y , 0.5*temp_cell_radius , 1.0 , colors[1] , colors[0] );
		Write_SVG_line(os, cursor_x, cursor_y-20.0 , cursor_x , cursor_y+20.0 , 4.0 , "lightskyblue" );
	}
	else {
		standard_agent_legend(os, cell_definition, cursor_x, cursor_y, cell_coloring_function, temp_cell_radius);
	}
	// !!! PHYSIMESS CODE BLOCK END !!! //
}