# Quickstart Guide to PhysiCell 

Nov 3, 2017

## Download

We currently provide 3 options for downloading PhysiCell:

1) Downloaded source code as a .zip file from http://physicell.mathcancer.org/ 
(the link there takes you to the latest release on sourceforge).
<!-- https://sourceforge.net/projects/physicell/files/PhysiCell/PhysiCell%201.2.1 -->

2) We have recently begun migrating our code to GitHub. So, you can `git clone` or download a .zip from 
https://github.com/MathCancer/PhysiCell. This will be our master branch of PhysiCell and therefore
should always be deployable, but keep in mind it is not a release version. (We're still deciding on whether to
continue using sourceforge or use GitHub for our releases).

For more detailed information, see Section 3 of the 
[User Guide](https://github.com/MathCancer/PhysiCell/blob/master/documentation/User_Guide.pdf) 
and/or also http://www.mathcancer.org/blog/physicell-tutorials/.

<hr> <!---------------------------------------------->

## Build: the basics

PhysiCell is written in C++ and should build on any of the three major operating systems. 
The one <b>requirement is that your compiler support OpenMP</b>. If, during your build (make) process, you get
a message like: `error: unsupported option '-fopenmp'`, you'll know your Makefile is trying to use a compiler
that doesn't support OpenMP. You may need to install an OpenMP-supported compiler and/or edit the Makefile to use it.

### Windows

The currently preferred way to use PhysiCell on Windows is to install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/) which will let you use 
a version of GCC that supports OpenMP. For more detailed information, 
[see our blog post](http://www.mathcancer.org/blog/setting-up-a-64-bit-gcc-environment-on-windows).

(Sometime in the near future, we plan to provide an option to use native Windows and a recent 
[Microsoft Visual C++](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads) compiler,
using [CMake](https://cmake.org/download/) to build PhysiCell. If you want to be a beta-tester, contact us).

### OSX

Unfortunately, the C++ compiler provided by the latest version of XCode on OSX does not support OpenMP.
To resolve this, we recommend using the `brew` package manager to install a recent version of `gcc`. Follow the [brew 
installation instructions](https://docs.brew.sh/Installation.html) (Note: this will still require that you have 
[XCode](https://developer.apple.com/xcode/) installed. See the link related to homebrew in the References below
for more details).

After installing brew, type `brew install gcc` from a Terminal command line. This 
should install a recent version of gcc/g++ (supporting OpenMP) into `/usr/local/bin`. You can verify this with:
```
$ ls -l /usr/local/bin/g++*
lrwxr-xr-x  1 heiland  admin  29 Oct  3 14:38 /usr/local/bin/g++-7@ -> ../Cellar/gcc/7.2.0/bin/g++-7
```

Set the following environment variable in your Terminal's shell, e.g., in the bash shell: 
```
export PHYSICELL_CPP=/usr/local/bin/g++-7
```
and the Makefile will use it. You probably want to permanently define this env var in your `~/.bash_profile` file also.

### Linux

If you're a Linux user, you probably already have or know how to install/use a proper g++ environment for 
building PhysiCell. If not, contact us!

<hr> <!---------------------------------------------->

## Build: sample projects

In the current release of PhysiCell, we provide four sample projects to help you get started. Three
of the projects are 2D models (<i>biorobots, anti-cancer biorobots, and cancer heterogeneity</i>); the fourth project
is a 3D model (<i>cancer immunology</i>). The procedure to build and execute each of the sample projects follows the same
pattern. For example, from your Terminal, in the root PhysiCell directory/folder:
```
$ make biorobots-sample
$ make
```

<!-- Note: the first `make` command silently copies over project-specific files, including the Makefile. The 
second `make` command attempts to compile the (new) code. Since the Makefile is being overwritten, any edits you
may have done to the previous Makefile (e.g., changing `CC` to point to `g++-7` on OSX) will be lost. You'll need
to edit it again or find another workaround. -->

Assuming the project builds without errors, you should now have an executable called `biorobots` which you can execute:
```
$ ./biorobots
```
This will begin the simulation, write information to your terminal, and generate output files of types `.svg`, `.xml`, and `.mat`. More about those below. You can hit `Control-c` to kill the simulation early, if you want.

For the remaining three example projects provided with PhysiCell, you would follow similar steps, but first clean out the previous simulation's output and prepare for the new one:
```
$ make data-cleanup   # optionally, delete all output data (otherwise, you can manuall copy to another directory to keep)
$ make reset
$ make cancer-biorobots-sample
$ make
$ ./cancer_biorobots
```
and
```
$ make data-cleanup
$ make reset
$ make heterogeneity-sample
$ make 
$ ./heterogeneity
```
and
```
$ make data-cleanup
$ make reset
$ make cancer-immune-sample
$ make
$ ./cancer_immune_3D
```

<hr> <!---------------------------------------------->

## Visualizing Output

PhysiCell does not currently provide a GUI for visualizing output results. Our approach, at least for now,
is to suggest and offer guidance on using other tools, e.g. your browser, [ImageMagick](https://www.imagemagick.org), 
[MATLAB](https://www.mathworks.com/products/matlab.html), [Octave](https://octave.sourceforge.io/), 
[ParaView](https://www.paraview.org/), and more.

### Browser

At a bare minimum, you should be able to use your browser to `File -> Open` any .svg (scalable vector graphics) file 
that your simulation generates. PhysiCell simulates transmitted light microscopy to create virtual H&E 
(hematoxylin and eosin) images for the .svg files. Even for 3D models, 2D cell images (.svg files) are generated, by 
default, using a slice through the Z=0 plane, as depicted in the following image (from the cancer-immune-sample project).

![alt text](https://github.com/rheiland/PhysiCell/blob/master/documentation/images/cancer_immune_snapshot00000574_small.png "SVG slice from 3D cancer-immune-sample project")

### MATLAB/Octave

If you have access to MATLAB (or Octave), we already have a detailed tutorial on how to visualize the
MultiCellDS digital snapshots (.xml and .mat files) at http://www.mathcancer.org/blog/working-with-physicell-snapshots-in-matlab/.

### ImageMagick

If you are able to install ImageMagick (with SVG support) on your computer, you will have access to several image processing command line
tools that will let you filter and arrange images. For example, the following commands:
```
$ montage -geometry +0+0 -tile 4x1 snapshot00000100.svg snapshot00000200.svg snapshot00000300.svg snapshot00000400.svg tmp.png
$ convert -resize 15% tmp.png out.png
```
will generate a tiled horizontal sequence of images:

![alt text](https://github.com/rheiland/PhysiCell/blob/master/documentation/images/cancer_immune_seq4x1_small.png "ImageMagick can tile images")

ImageMagick will also let you generate an animated gif of your results, e.g.:
```
$ convert snapshot000034*.svg foo.gif
$ magick animate foo.gif    # may be huge, if original SVGs were; downsize in following steps
$ convert foo.gif -coalesce tmp.gif
$ identify snapshot00003471.svg    # get size of a single image (e.g. 1500x1605)
$ convert -size 1500x1605 tmp.gif -resize 20% small.gif
$ magick animate small.gif
```

### ParaView

If you install ParaView, you can visualize and interact with output from 3D models. We will be providing a detailed
tutorial for this.

![alt text](https://github.com/rheiland/PhysiCell/blob/master/documentation/images/PhysiCell_ParaView_quickstart.png "ParaView w clipping planes and barchart")

<hr> <!---------------------------------------------->

## Community Support

**we need an email list**

<hr> <!---------------------------------------------->

## References

* http://physicell.mathcancer.org/
* http://www.mathcancer.org/blog/setting-up-a-64-bit-gcc-environment-on-windows
* http://www.mathcancer.org/blog/setting-up-gcc-openmp-on-osx-homebrew-edition/
* http://www.mathcancer.org/blog/physicell-tutorials/
* http://www.mathcancer.org/blog/working-with-physicell-snapshots-in-matlab/
* https://github.com/MathCancer/PhysiCell/blob/master/documentation/User_Guide.pdf
