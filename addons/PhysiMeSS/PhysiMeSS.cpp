#include "PhysiMeSS.h"

#include <algorithm>
#include <iterator> 


static double last_update_time = -mechanics_dt;

void remove_physimess_out_of_bounds_fibres()
{
    for (auto* cell : *all_cells) {
        if (isFibre(cell) && static_cast<PhysiMeSS_Fibre*>(cell)->fail_count >= 10)
        {
            // std::cout << "I failed to place " << cell->type_name << " " 
            //           << cell->ID << " in the domain - I am deleting agent " 
            //           << std::endl;
            delete_cell(cell);
        }
    }
}

void physimess_update_cell_velocity( Cell* pCell, Phenotype& phenotype, double dt)
{
    
    double movement_threshold = pCell->custom_data["fibre_stuck_threshold"];
    if (!isFibre(pCell) && phenotype.motility.is_motile) {
	
        // Here I changed this, because here we don't have access to the old position, and I didn't want to track the old position
        // So I'm using the previous velocity, which is not exactly the same (because of Adams-Bashforth), but is a good proxy
        // if (dist(pCell->old_position, pCell->position) < movement_threshold) {
        if (norm(pCell->get_previous_velocity())*mechanics_dt < movement_threshold) {
			static_cast<PhysiMeSS_Cell*>(pCell)->stuck_counter++;
        } else {
            static_cast<PhysiMeSS_Cell*>(pCell)->stuck_counter = 0;
        }
    }
    
	if( pCell->functions.add_cell_basement_membrane_interactions )
	{
		pCell->functions.add_cell_basement_membrane_interactions(pCell, phenotype,dt);
	}
	
	pCell->state.simple_pressure = 0.0;
	pCell->state.neighbors.clear(); // new 1.8.0
    
    if (!isFibre(pCell)) {
        //First check the neighbors in my current voxel
        for (auto* neighbor: pCell->get_container()->agent_grid[pCell->get_current_mechanics_voxel_index()])
        {
            if (!isFibre(neighbor)) {
                pCell->add_potentials(neighbor);
            }
        }
        for (auto neighbor_voxel_index: pCell->get_container()->underlying_mesh.moore_connected_voxel_indices[pCell->get_current_mechanics_voxel_index()])
        {
            if(!is_neighbor_voxel(pCell, pCell->get_container()->underlying_mesh.voxels[pCell->get_current_mechanics_voxel_index()].center, pCell->get_container()->underlying_mesh.voxels[neighbor_voxel_index].center, neighbor_voxel_index))
                continue;
        
            for (auto* neighbor: pCell->get_container()->agent_grid[neighbor_voxel_index])
            {
                if (!isFibre(neighbor)) {
                    pCell->add_potentials(neighbor);
                }
            }
        }
    } else {
        // Count crosslinks
        static_cast<PhysiMeSS_Fibre*>(pCell)->X_crosslink_count = 0;
        if (static_cast<PhysiMeSS_Fibre*>(pCell)->fibres_crosslinkers.size() > 0){
            static_cast<PhysiMeSS_Fibre*>(pCell)->X_crosslink_count = static_cast<PhysiMeSS_Fibre*>(pCell)->fibres_crosslinkers.size();
        }

    }

    // std::cout << " AGENT " << pCell->type_name << " " << pCell->ID << " has " ;
    //add potentials between pCell and its neighbors
    for (auto* neighbor : static_cast<PhysiMeSS_Agent*>(pCell)->physimess_neighbors)
    {
        // std::cout << neighbor->type_name << " " << neighbor->ID << " " ;
        
        // if( this->ID == other_agent->ID )
        if( pCell != neighbor )
        { 
            if (!isFibre(pCell) && !isFibre(neighbor)) {
                //Already done above
                continue;
            } else 
            if (!isFibre(pCell) && isFibre(neighbor)) {
                static_cast<PhysiMeSS_Cell*>(pCell)->add_potentials_from_fibre(static_cast<PhysiMeSS_Fibre*>(neighbor));
            } else  if (isFibre(pCell) && !isFibre(neighbor)) {
                static_cast<PhysiMeSS_Fibre*>(pCell)->add_potentials_from_cell(static_cast<PhysiMeSS_Cell*>(neighbor));
            } else if (isFibre(pCell) && isFibre(neighbor)) {
                static_cast<PhysiMeSS_Fibre*>(pCell)->add_potentials_from_fibre(static_cast<PhysiMeSS_Fibre*>(neighbor));
            } else {
                // std::cout << " WARNING: interaction between errant cell-types has been called : " << pCell->type_name << ", " << neighbor->type_name << std::endl;
                return;
            }
        }
    }
    // std::cout << std::endl;

    if (!isFibre(pCell)) {
        
        PhysiMeSS_Cell* ppCell = static_cast<PhysiMeSS_Cell*>(pCell);
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
    }
    return;

}

void physimess_mechanics( double dt ) 
{    
    if ( PhysiCell_globals.current_time >= last_update_time + dt - 0.5 * diffusion_dt) // last_update_time + dt = next update time; - 0.5 * diffusion_dt will give the time step closest to the desired next time (assumign diffusion_dt is the smallest time step)
    {
        last_update_time = PhysiCell_globals.current_time;
        
        //#pragma omp parallel for
        // This is not parallel because we are modifying the agend grid
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            
            static_cast<PhysiMeSS_Agent*>(pC)->physimess_voxels.clear();
            if( !pC->is_out_of_domain )
            {
                static_cast<PhysiMeSS_Agent*>(pC)->register_fibre_voxels();
            }
        }

        #pragma omp parallel for
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            static_cast<PhysiMeSS_Agent*>(pC)->physimess_neighbors.clear();
            if (isFibre(pC)) {
                static_cast<PhysiMeSS_Fibre*>(pC)->fibres_crosslinkers.clear();
            }
            if( !pC->is_out_of_domain )
            {
                static_cast<PhysiMeSS_Agent*>(pC)->find_agent_neighbors();
            }
        }

        // #pragma omp parallel for
        // This is not parallel because we are modifying the agend grid
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            if( !pC->is_out_of_domain )
            {
                static_cast<PhysiMeSS_Agent*>(pC)->deregister_fibre_voxels();
            }
        }
        
        // determine and add crosslinks
        #pragma omp parallel for
        for( int i=0; i < (*all_cells).size(); i++ )
        {
            Cell* pC = (*all_cells)[i];
            if (isFibre(pC)) {
                static_cast<PhysiMeSS_Fibre*>(pC)->add_crosslinks();
            }
        }
    }
}


void fibre_agent_SVG(std::ofstream& os, PhysiCell::Cell* pC, double z_slice, std::vector<std::string> (*cell_coloring_function)(Cell*), double X_lower, double Y_lower) {

	// place a rod if it's a fibre (note fibre already renamed here)
	if (isFibre(pC) ){
    
        PhysiMeSS_Fibre* pFibre = static_cast<PhysiMeSS_Fibre*>(pC);
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
	// place a rod if it's a fibre (note fibre not yet renamed)
	if (isFibre(&C)) {
		//Write_SVG_fibre(os, cursor_x, cursor_y , 0.5*temp_cell_radius , 1.0 , colors[1] , colors[0] );
		Write_SVG_line(os, cursor_x, cursor_y-20.0 , cursor_x , cursor_y+20.0 , 4.0 , "lightskyblue" );
	}
	else {
		standard_agent_legend(os, cell_definition, cursor_x, cursor_y, cell_coloring_function, temp_cell_radius);
	}
}