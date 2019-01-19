# Perform system tests

Assuming `heterogeneity` has been compiled, run it using a config file using a single thread:
```
~/git/PhysiCell/tests/system$ ../../heterogeneity config_hetero_1core.xml
```
then verify the `final.svg` file is what we expect:
```
~/git/PhysiCell/tests/system/output_heterogenity$ diff final.svg final_1core_240min.svg
3644c3644
<    0 days, 0 hours, 0 minutes, and 13.1251 seconds
---
>    0 days, 0 hours, 0 minutes, and 13.0818 seconds
```

