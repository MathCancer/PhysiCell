
If you get an error about the libroadrunner libraries not being found at runtime, e.g.:

$ test_rr1 
dyld: Library not loaded: @rpath/libroadrunner_c_api.dylib

you need to take the necessary steps on your particular operating system to allow them
to be found.
For example, on macOS, you need to do something analogous to:
export DYLD_LIBRARY_PATH=/Users/heiland/libroadrunner/roadrunner-osx-10.9-cp36m/lib
on Linux:
export LD_LIBRARY_PATH=/Users/heiland/libroadrunner/roadrunner-osx-10.9-cp36m/lib
on Windows:
go into your system settings and ...



