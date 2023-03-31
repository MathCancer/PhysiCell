# PhysiMESS
PhysiMESS (PhysiCell MicroEnvironment Structure Simulation) is a PhysiCell add-on which allows users to simulate ECM components as agents. 

## Dedicated sample project
PhysiMeSS comes with a dedicated sample project, called **physimess-sample**. To build it, go to the root directory and use : 

```
    make physimess-sample
    make
```

## Pre-loaded examples

### Fibre_Initialisation
* mymodel_initialisation.xml and initialfibres.csv files for initialising fibres in the domain

### Fibre_Degradation 
* mymodel_fibre_degradation.xml and cells_and_fibres_attractant.csv to model one cell degrading fibres to reach attractant
* mymodel_matrix_degradation.xml and cells_and_fibres.csv to model growth of cell mass degrading matrix

### Cell_Fibre_Mechanics
* mymodel_fibremaze.xml and fibre_maze.csv to model cell moving around a maze made of fibres
* mymodel_potentials.xml and snowplough.csv to model both fibre pushing and rotation by cells
* mymodel_hinge.xml and hinge.csv to model fibres being rotated at their hinge crosslink point by a cell
