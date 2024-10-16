# PhysiMeSS
PhysiMeSS (PhysiCell Microenvironment Structure Simulation) is a PhysiCell add-on which allows users to simulate ECM components as agents. 

## Paper
PhysiMeSS paper is available on Gigabyte : [https://gigabytejournal.com/articles/136](https://gigabytejournal.com/articles/136). 

DOI: [10.46471/gigabyte.136](https://doi.org/10.46471/gigabyte.136).

## Dedicated sample project
PhysiMeSS comes with a dedicated sample project, called **physimess-sample**. To build it, go to the root directory and use : 

```
    make physimess-sample
    make
```

## Pre-loaded examples 
The following example directories are populated in config directory once the **physimess-sample** project is loaded as above.

### Fibre_Initialisation
The directory Fibre_Initialisation contains simple examples in which you can initialise ECM fibres in the domain. Fibres are cylindrical agents described by their centre, radius, length and orientation. The centre of each fibre is prescribed either from a csv file or at random (as per cells in PhysiCell). The other attributes can be altered via user parameters in the xml or GUI. The following default parameters are found in ```mymodel_initialisation.xml```

```
number_of_fibres = 2000
anisotropic_fibres = false
fibre_length = 75.0 (microns)
length_normdist_sd  = 0.0 (microns)
fibre_radius = 2.0 (microns)
fibre_angle = 0.0 (radians)
angle_normdist_sd = 0.0 (radians)
```

Using these parameters you can set up a domain with 2000 fibres randomly positioned and randomly aligned with a length of 75 microns and radius of 2 microns. Note, since we remove any fibres which overlap the boundaries of the domain, 1931 fibres remain after initialisation. Although fibres are cylinders, they are visualised in the domain and plot legend as lines. Note, agent names with the following strings are presumed to be fibres: **ecm**; **fibre**; **fiber**; **rod**; **matrix**.

A second xml file ```mymodel_initialisation_maze.xml``` along with the csv ```initialfibres.csv``` allows you to create a maze with horizontal and vertical fibres. Agent names **fibre_horizontal** and **fibre_vertical** are reserved for creating horizontal and vertical fibres, respectively. In this way fibre agents can be used to create walls in your domain.

### Fibre_Degradation 
The directory Fibre_Degradation contains examples which show how fibre degradation affects simulations of cell migration and cell proliferation.

* The xml file ```mymodel_fibre_degradation.xml``` with csv file ```cells_and_fibres_attractant.csv``` simulates the migration of a single cell towards an attractant through a mesh of fibres. Fibres are assigned a position, radius and length but their orientation is random. By turning fibre degradation on (or off) and adjusting parameters you can control whether the cell navigates to the attractant.

Default parameters are:

```
fibre_length = 40.0 (microns)
fibre_degradation = true 
fibre_deg_rate = 0.01 (1/min)
fibre_stuck = 10.0 (mechanics timesteps)
```

Note the parameter ```fibre_stuck``` determines how many mechanics time steps a cell needs to have been stuck before it can possibly degrade a fibre at the rate ```fibre_deg_rate```. When a fibre is degraded it is immediately removed from the domain.

* The xml file ```mymodel_matrix_degradation.xml``` with csv file ```cells_and_fibres.csv``` simulates cell proliferation and the forming of a cell mass within a mesh of fibres. The fibrous mesh is as above. By turning fibre degradation on (or off) and adjusting parameters you can control the development of the growing mass of cells.


### Cell_Fibre_Mechanics
The directory Cell_Fibre_Mechanics contains examples which demonstrate cell-fibre mechanics. 

* The xml file ```mymodel_fibremaze.xml``` with csv file  ```fibre_maze.csv``` simulates a single cell moving within a maze made of fibres towards an attractant secreting a nutrient. By adjusting parameters you can control whether the cell navigates to the attractant.

Default parameters are:

```
fibre_length = 60.0 (microns)
vel_adhesion = 0.6 
vel_contact = 0.1
cell_velocity_max = 1.0
```

* The xml file ```mymodel_pushing.xml``` with csv file  ```snowplough.csv``` simulates a single cell pushing a single free/non-crosslinked fibre out of the way to access an attractant. By turning fibre pushing on (or off) you can control whether the cell can push fibres. Note, since cells can only push non-crosslinked fibres by increasing the ```fibre_length``` in this example and thereby creating crosslinks you can prevent cells from pushing fibres.

Default parameters are:

```
fibre_length = 40.0 (microns)
fibre_pushing = true
```

* The xml file ```mymodel_rotating.xml``` with csv file ```snowplough.csv``` simulates a single cell rotating free/non-crosslinked fibres to access an attractant. By turning fibre rotating on (or off) you can control whether the cell can push fibres. Note the parameter ```fibre_sticky``` modulates how much a cell can rotate a fibre. Values less than 1.0 decrease how much the cell rotates the fibre per timestep and values greater than 1.0 increase how much the cell rotates the fibre per timestep.

Default parameters are:

```
fibre_length = 40.0 (microns)
fibre_sticky = 1.0
fibre_rotation = true
```

* The xml file ```mymodel_hinge.xml``` with csv file ```hinge.csv``` simulates two crosslinked fibres being rotated at their crosslink point (hinge) by a single cell in order for the cell to navigated towards an attractant. By turning fibre rotating on (or off)  and adjusting parameters you can control whether the cell successfully navigates towards the attractant.
