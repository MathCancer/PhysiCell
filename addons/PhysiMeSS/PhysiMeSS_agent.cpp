#include "PhysiMeSS_agent.h"

#include <algorithm>
// #include "PhysiMeSS_fibre.h"

PhysiMeSS_Agent::PhysiMeSS_Agent()
{
    physimess_neighbors.clear();
    physimess_voxels.clear();
}

std::list<int> PhysiMeSS_Agent::find_agent_voxels() {

    // this code is for creating a list of all voxels which either contain the agent
    // or are neighboring voxels of the voxel containing the agent 
    std::list<int> all_agent_voxels_to_test;
    
    for (int voxel: physimess_voxels) 
    {    
        all_agent_voxels_to_test.push_back(voxel);
        
        for (auto side_voxel : this->get_container()->underlying_mesh.moore_connected_voxel_indices[voxel]) 
        {
            all_agent_voxels_to_test.push_back(side_voxel);
        }
    }
    // get rid of any duplicated voxels
    all_agent_voxels_to_test.sort();
    all_agent_voxels_to_test.unique();

    return all_agent_voxels_to_test;
}

void PhysiMeSS_Agent::find_agent_neighbors() {

    // this code is for finding all neighbors of an agent: first we call find_agent_voxels
    // to create a list of all the voxels to test, then we search for agents in those voxels
    
    std::list<int> voxels_to_test = this->find_agent_voxels();

    //std::cout << "Agent " << pCell->ID << " is tested in voxels: " ;
    for (int voxel: voxels_to_test) 
    {
        //std::cout << voxel << " " ;
        for (auto* neighbor : this->get_container()->agent_grid[voxel])
        {
            // do not include the neighbor if it is the agent itself or if it is in the list already
            if (this != neighbor){
                if (std::find(physimess_neighbors.begin(), physimess_neighbors.end(), neighbor) == physimess_neighbors.end()) {
                    physimess_neighbors.push_back(neighbor);
                }
            }
        }
    }
    //std::cout << std::endl;
}