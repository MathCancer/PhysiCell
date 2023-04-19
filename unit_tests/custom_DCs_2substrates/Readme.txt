Test manually setting Dirichlet nodes on voxels for 2 different substrates.
The range of values for the nodes will be very different for each substrate.

After compiling a sample project in the root directory, just copy the relevant
files in this folder to the appropriate folder at the root:

cp Makefile ../..
cp main.cpp ../..
#cp config/PhysiCell_settings.xml  ../../config
cp config/config*.xml  ../..
cp custom_modules/custom.*  ../../custom_modules

cd ../..
make
test_custom_DCs 
