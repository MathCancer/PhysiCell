## Resume a simulation from a previous checkpoint

* provide a MultiCellDS file (output000*N.xml) as a starting point to continue a simulation
* does not handle intracellular model state
* does not currently recover "output*_graph.txt" data; rather relies on the next mechanics step to recreate the information
* if a model relies on global data generated in custom C++ code that is not captured in the MultiCellDS data, it cannot be recovered
* an agent's `is_movable` flag is not currently saved in the MultiCellDS (.mat), so it will not be recovered

Normal simulation:

```
project config/cycle_phase_3cells_custom_vecs.xml    # writes output to /output
```


Resume a simulation:

```
Usage options: 
- no args: will try to use   ./config/PhysiCell_settings.xml
- 1 arg: will try to use it as the config file
- 5 args to resume a simulation:
 <.xml config file> <resume_folder> <resume_file> <next_full_index> <next_svg_index>

 e.g. project config/PhysiCell_settings.xml  output  output00000042.xml  43 43

```

```
project config/cycle_phase_3cells_custom_vecs.xml output output00000048.xml 49 49
```


