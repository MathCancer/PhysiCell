# Perform system tests

Assuming `heterogeneity` has been compiled, run it using a config file using a single thread:
```
~/git/PhysiCell/tests/system$ ../../heterogeneity config_hetero_1core.xml
```
then verify the `final.svg` file is what we expect:
```
~/git/PhysiCell/tests/system/output_heterogenity$ diff final.svg final_1core_240min.svg
...
```
The only difference should be the amount of time used in the computation.
