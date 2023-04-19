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
In these examples we demonstrate the cell-fibre mechanics. 

* In ```mymodel_fibremaze.xml``` which uses  ````fibre_maze.csv``` we simulate a single cell moving around a maze made of fibres towards an attractant secreting a nutrient.

Default parameters are:

```
fibre_length = 60.0 (microns)
vel_adhesion = 0.6 
vel_contact = 0.1
cell_velocity_max = 1.0
```

* In ```mymodel_pushing.xml``` which uses  ```snowplough.csv``` we simulate a single cell pushing a single free/non-crosslinked fibre out of the way to access an attractant. 

Default parameters are:

```
fibre_length = 40.0 (microns)
fibre_pushing = true
```

* In ```mymodel_rotating.xml``` which uses  ```snowplough.csv``` we simulate a single cell rotating free/non-crosslinked fibres to acces an attractant. 

Default parameters are:

```
fibre_length = 40.0 (microns)
fibre_sticky = 1.0
fibre_rotation = true
```

Note the parameter ```fibre_sticky`` modulates how much a cell can rotate a fibre. Values less than 1.0 decrease how much the cell rotates the fibre per timestep and values greater than 1.0 increase how much the cell rotates the fibre per timestep.

* In ```mymodel_hinge.xml``` which uses ```hinge.csv``` we simulate two fibres being rotated at their hinge crosslink point by a single cell in order for the cell to access an attractant.
