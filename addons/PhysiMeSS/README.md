# PhysiMESS
PhysiMESS (PhysiCell MicroEnvironment Structure Simulation) is a PhysiCell add-on which allows users to simulate ECM components as agents. 

## Dedicated sample project
PhysiMeSS comes with a dedicated sample project, called **physimess-sample**. To build it, go to the root directory and use : 

```
    make physimess-sample
    make
```

## Pre-loaded examples 
The following basic examples are populated in config directory once the **physimess-sample** project is loaded.

### Fibre_Initialisation
A simple example in which you can initialise fibres in the domain. Fibres are cylindrical agents described by their centre, radius, length and orientation. The centre of each fibre is prescribed either from a csv file or at random (as per cells in PhysiCell). The other attributes can be altered via user parameters in the xml or GUI. The following are default parameters found in ```mymodel_initialisation.xml```

```
number_of_fibres = 2000
anisotropic_fibres = false
fibre_length = 75.0 (microns)
length_normdist_sd  = 0.0 (microns)
fibre_radius = 2.0 (microns)
fibre_angle = 0.0 (radians)
angle_normdist_sd = 0.0 (radians)
```

Using these parameters you can set up a domain with 2000 fibres randomly positioned and randomly aligned with a length of 75 microns and radius of 2 microns. Note since we remove any fibres which overlap the boundaries of the domain 1931 fibres remain after initialisation. Although fibres are cylinders, they are visualised in the domain as lines.

A second xml file ```mymodel_initialisation_maze.xml``` along with the csv ```initialfibres.csv``` allows the user to create a maze with horizontal and vertical fibres.

### Fibre_Degradation 
In these examples we show how fibre degradation affects simulations of cell migration and cell proliferation.

* Cell migration towards an attractant through fibres uses xml file ```mymodel_fibre_degradation.xml``` with csv file ```cells_and_fibres_attractant.csv```. Fibres are assigned a position, radius and length but their orientation is random.

* Cell proliferation within a fibrous mesh uses xml file ```mymodel_matrix_degradation.xml``` with csv file ```cells_and_fibres.csv```. The fibrous mesh is as above.

Default parameters are:

```
fibre_length = 40.0 (microns)
fibre_degradation = true 
fibre_deg_rate = 0.01 (1/min)
fibre_stuck = 10.0 (mechanics timesteps)
```

Note the parameter ```fibre_stuck``` determines how many mechanics time steps a cell needs to have been stuck before it can possibly degrade a fibre at the rate ```fibre_deg_rate```.


### Cell_Fibre_Mechanics
* mymodel_fibremaze.xml and fibre_maze.csv to model cell moving around a maze made of fibres
* mymodel_potentials.xml and snowplough.csv to model both fibre pushing and rotation by cells
* mymodel_hinge.xml and hinge.csv to model fibres being rotated at their hinge crosslink point by a cell
