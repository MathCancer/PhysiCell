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

It will be <b>this version of gcc</b> you'll need to use in the Makefile:
```
CC := /usr/local/bin/g++-7
```

### Linux

If you're a Linux user, you probably already have or know how to install/use a proper g++ environment for 
building PhysiCell. If not, contact us!

<hr> <!---------------------------------------------->

## Build: sample projects

<hr> <!---------------------------------------------->

## References
[http://www.mathcancer.org/blog/physicell-tutorials/]
