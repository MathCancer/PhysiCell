# Quickstart Guide to PhysiCell 

October 2017
 
** re-do README.txt and copy old README to Release_history?

<b>WARNING:</b> If you're a savvy software developer and are accustomed to just typing `make` whenever you 
see a `Makefile`, DON'T. For the current version of PhysiCell, we are trying to make it easy for
users to build example projects using a 2-step build process: `make <example project>` followed by `make project`. More
details below.

## Download

We currently provide 3 options for downloading PhysiCell:

1) Downloaded source code as a .zip file from http://physicell.mathcancer.org/ 
(the link there takes you to the latest release on sourceforge).
<!-- https://sourceforge.net/projects/physicell/files/PhysiCell/PhysiCell%201.2.1 -->

2) We have recently begun migrating our code to GitHub. So, you can `git clone` or download a .zip from 
https://github.com/MathCancer/PhysiCell. Keep in mind, this will be our master branch of PhysiCell and therefore
should always be deployable, but keep in mind it is not a release version. (We're still deciding on whether to
continue using sourceforge or to use GitHub for our releases).

3) If you'd rather not build the code yourself, we provide a virtual appliance (.ova) file (~400MB)
at the [1.2.1 release on sourceforge](https://sourceforge.net/projects/physicell/files/PhysiCell/PhysiCell%201.2.1/). 
You can use whatever virtualization application that can import that appliance, e.g. 
[VirtualBox](http://www.oracle.com/technetwork/server-storage/virtualbox/downloads/index.html).


For more detailed information, see Section 3 of the 
[User Guide](https://github.com/MathCancer/PhysiCell/blob/master/documentation/User_Guide.pdf) 
and/or also http://www.mathcancer.org/blog/physicell-tutorials/.

<hr> <!---------------------------------------------->

## Build: the basics

PhysiCell is written in C++ and should build on any of the three major operating systems. 
The one <b>requirement is that your compiler support OpenMP</b>. If, during your build (make) process, you get
a message like: `error: unsupported option '-fopenmp'`, you'll know your Makefile is trying to use a compiler
that doesn't support OpenMP. You may need to edit the Makefile to reference the desired C++ compiler.

### Windows

There are (at least) two options for building PhysiCell on Windows:

1) Install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/) which will let you use 
a version of GCC that supports OpenMP. For more detailed information, 
[see our blog](http://www.mathcancer.org/blog/setting-up-a-64-bit-gcc-environment-on-windows).

2) Use native Windows (not MinGW) and a recent 
[Microsoft Visual C++](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads) compiler.
Unless you're a proficient Windows developer, we recommend [installing CMake](https://cmake.org/download/) to 
build PhysiCell using this option.

### OSX

Unfortunately, the C++ compiler provided by the latest version of XCode on OSX does not support OpenMP.
To resolve this, we recommend using the `brew` package manager to install a recent version of `gcc`. Follow the [brew 
installation instructions](https://docs.brew.sh/Installation.html) (Note: this will still require that you have 
[XCode](https://developer.apple.com/xcode/) installed].

After installing brew, you should be able to open a Terminal and type `brew install gcc` from the command line. This 
should install a recent version of gcc/g++ (supporting OpenMP) into `/usr/local/bin`. You can verify this with:
```
$ ls -l /usr/local/bin/g++*
lrwxr-xr-x  1 heiland  admin  29 Oct  3 14:38 /usr/local/bin/g++-7@ -> ../Cellar/gcc/7.2.0/bin/g++-7
```

It will be <b>this version of gcc</b> you'll need to reference in the Makefile, e.g.:
```
CC := /usr/local/bin/g++-7
```

### Linux

If you're a Linux user, you probably already have or know how to install/use a proper g++ environment for 
building PhysiCell. If not, contact us!

<hr> <!---------------------------------------------->

## Build: sample projects

In the current release of PhysiCell, we provide four sample projects to help get you started using PhysiCell. Three
of the projects are 2D models (<i>biorobots, anti-cancer biorobots, and cancer heterogeneity</i>); the fourth project
is a 3D model (<i>cancer immunology</i>). The procedure to build and execute each of these projects follows the same
pattern, for example, from your Terminal, in the root PhysiCell directory/folder:
```
$ make biorobots-sample
$ make project
```

Note: the first `make` command silently copies over project-specific files, including the Makefile. The 
second `make` command attempts to compile the (new) code. Since the Makefile is being overwritten, any edits you
may have done to the previous Makefile (e.g., changing `CC` to point to `g++-7` on OSX) will be lost. You'll need
to edit it again or find another workaround.

Assuming the project builds without errors, you should now have an executable called `biorobots`. To keep your
root directory tidy, we recommend creating a project-specific subdirectory and running the project from there since
several output files will be generated. For example (actual command syntax may vary slightly on Windows):
```
$ mkdir run-biorobots1
$ cd run-biorobots1
$ ../biorobots
```
will generate several output files of types `.svg`, `.xml`, and `.mat`. More about those below.

For the remaining three example projects provided with PhysiCell, you would follow similar steps:
```
$ cd ..   # make sure you're in the root directory of PhysiCell
$ make cancer-biorobots-sample
$ make project
$ mkdir run-cancer-biorobots1
$ cd run-cancer-biorobots1
$ ../cancer_biorobots
```
and
```
$ cd ..   # make sure you're in the root directory of PhysiCell
$ make heterogeneity-sample
$ make project
$ mkdir run-heterogeneity1
$ cd run-heterogeneity1
$ ../heterogeneity
```
and
```
$ cd ..   # make sure you're in the root directory of PhysiCell
$ make cancer-immune-sample
$ make project
$ mkdir run-cancer-immune1
$ cd run-cancer-immune1
$ ../cancer_immune_3D
```

<hr> <!---------------------------------------------->

## Visualizing Output

PhysiCell does not currently provide a GUI for visualizing output results. Our approach, at least for now,
is to suggest and offer guidance on using other tools, e.g. your browser, [ImageMagick](https://www.imagemagick.org), 
[MATLAB](https://www.mathworks.com/products/matlab.html), [Octave](https://octave.sourceforge.io/), 
[ParaView](https://www.paraview.org/) (more generally, [VTK](https://www.vtk.org/)), and more.

At a bare minimum, you should be able to use your browser to `File -> Open` any .svg (scalable vector graphics) file 
that your simulation generates. PhysiCell simulates transmitted light microscopy to create virtual H&E 
(hematoxylin and eosin) images for the .svg files. Even for 3D models, 2D cell images (.svg files) are generated, by 
default, as slices through the Z=0 plane.

![alt text](https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png "Logo Title Text 1")

<hr> <!---------------------------------------------->

## Community Support

**we need an email list**

<hr> <!---------------------------------------------->

## References

* http://physicell.mathcancer.org/
* http://www.mathcancer.org/blog/setting-up-a-64-bit-gcc-environment-on-windows
* http://www.mathcancer.org/blog/setting-up-gcc-openmp-on-osx-homebrew-edition/
* http://www.mathcancer.org/blog/physicell-tutorials/
* https://github.com/MathCancer/PhysiCell/blob/master/documentation/User_Guide.pdf
