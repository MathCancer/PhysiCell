# PhysiBoSS Tutorial models

In this folder we provide the models presented in the PhysiBoSS tutorials

## Manuscript

The manuscript of the paper is available [here](https://doi.org/10.1093/bib/bbae509), and the supplementary materials (containing the detailed instructions to build the model) is available [here](https://raw.githubusercontent.com/PhysiBoSS/PhysiBoSS/refs/heads/master/sample_projects_intracellular/boolean/tutorial/paper/PhysiBoSS_tutorial_supp_mat.pdf)

## Binary file

To obtain the tutorial binary file, you either need to download the pre-compiled binary using : 
```
    python beta/download_binary.py tutorial
```
or to compile the project using : 
```
    make physiboss-tutorial; make -j
```

Both of these commands needs to be run from the root directory of PhysiCell. For more information, refer to the supplementary materials of the manuscript. 

## Model XML settings 

We provide several XML files in the config folders, for the different models.
To run these models, either load them from PhysiCell studio, or execute 
``` 
    project <XML file>
```

from the PhysiCell root folder, after obtaining the binary file as described above. 

### Cell fate model

- config/simple_tnf/0_Initial.xml : Initial step of the model, with a growing tumour
- config/simple_tnf/1_Long_TNF.xml : Simulation of a long TNF treatment
- config/simple_tnf/1_Long_TNF_stochastic_time.xml : Simulation of a long TNF treatment, with cell desynchronisation
- config/simple_tnf/2_Short_TNF.xml : Simulation of pulsatile TNF treatment
- config/simple_tnf/3_Necrotic_core.xml : Simulation of a necrotic core in the tumour
- config/simple_tnf/4_Mutants.xml : Simulation of an heterogenous tumour, with a population resistant to the TNF treatment

### Cell cycle model

- config/cell_cycle/PhysiCell_settings.xml : Default simulation of the cell cycle
- config/cell_cycle/PhysiCell_settings_plk1_knockout.xml : Simulation of the cell cycle with a PLK1 inhibition
- config/cell_cycle/PhysiCell_settings_foxo3_knockout.xml : Simulation of the cell cycle with a FOXO3 inhibition
- config/cell_cycle/PhysiCell_settings_p110_knockin.xml : Simulation of the cell cycle with a p110 activation

### Differentiation model

- config/differentiation/PhysiCell_settings.xml : Default simulation of the T Cell differentiation model
- config/differentiation/PhysiCell_settings_FOXP3_2_mutant.xml : Simulation of the T Cell differentiation model with FOXP3_2 knockout in naive T cells
- config/differentiation/PhysiCell_settings_NFKB_mutant.xml : Simulation of the T Cell differentiation model with NFKB knockout in naive T cells
- config/differentiation/PhysiCell_settings_FOXP3_2_lower.xml : Simulation of the T Cell differentiation model with lower activation rate for FOXP3_2 in naive T cells
- config/differentiation/PhysiCell_settings_NFKB_lower.xml : Simulation of the T Cell differentiation model with lower activation rate for NFKB in naive T cells


## Scripts

For reproducing our analysis of the Boolean models or the PhysiBoSS models, we are providing a collection of Jupyter notebooks available in the **scripts** folder.
