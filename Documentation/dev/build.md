Building Visocyte
=================

This page describes how to build and install Visocyte. It covers building for development, on both Unix-type systems (Linux, HP-UX, Solaris, Mac), and Windows.

Visocyte depends on several open source tools and libraries such as Python, Qt, CGNS, HDF5, etc. Some of these are included in the Visocyte source itself (e.g. HDF5), while others are expected to be present on the machine on which Visocyte is being built (e.g. Python, Qt, CGNS).

Adapted from the [Visocyte wiki](http://www.visocyte.org/Wiki/Visocyte:Build_And_Install) which has more complete but dated instructions.

Prerequisites
=============
* The Visocyte build process requires [CMake](http://www.cmake.org/) version 3.10 or higher and a working compiler. On Unix-like operating systems, it also requires Make, while on Windows it requires Visual Studio. The Ninja build system speeds up compilation on most systems substantially.
* Building Visocyte's user interface requires [Qt](http://www.qt.io/download-open-source/), version 5.6+ (5.9.\* is recommended, 5.7.\*, 5.8\*, and 5.10 also work). To compile Visocyte, either the LGPL or commercial versions of Qt may be used. Also note that on Windows you need to choose a Visual Studio version to match binaries available for your Qt version.
* For Windows builds, unix-like environments such as Cygwin, or MinGW are not officially supported.

Download And Install CMake
--------------------------
CMake is a tool that makes cross-platform building simple. On several systems it will probably be already installed. If it is not, please use the following instructions to install it.

There are several precompiled binaries available at the [CMake download page](https://cmake.org/download/). Download version 3.10 or later.

Add CMake to your PATH environment variable if you downloaded an archive and not an installer.

Download And Install Qt
--------------------------
Visocyte uses Qt as its GUI library. Qt is required whenever the Visocyte client is built with a GUI.

* [Download a release](http://download.qt.io/official_releases/qt/).
    - For binaries, use the latest stable version of qt-PLATFORM-opensource-VERSION.[tar.gz or zip or dmg or exe]. When downloading binaries, ensure that your compiler version matches the Qt compiler indicated. Version 5.6+ supports Visual Studio 2015.
    - For source code, use the latest stable version of qt-everywhere-opensource-src-VERSION.[tar.gz or zip or dmg].

Compiler and Build Tool
-----------------------
Linux Ubuntu/Debian (16.04):

* sudo apt install
    - build-essential cmake git python-dev mesa-common-dev mesa-utils freeglut3-dev libhdf5-serial-dev autoconf libtool bison flex
* sudo python get-pip.py
    - sudo -H pip install numpy
    - sudo -H pip install Mako
* sudo apt-get install ninja-build
    - ninja is a speedy replacement for make, highly recommended.

**Note for Ubuntu 16.04**. The official Qt 5.9.1 binaries downloaded from `qt.io`
are linked against a different version of `libprotobuf` than what Visocyte uses,
which causes runtime errors in Visocyte. To avoid this, you can move the file
`libqgtk3.so` out from `Qt5.9.1/plugins/platformthemes`. This platform theme is
linked against a different `libprotobuf` than Visocyte. Moving it causes it not
to be loaded, thereby avoiding the runtime errors with no negative effects on
running Visocyte.

Windows:

* Visual Studio 2015 Community Edition
* Use "VS2015 x64 Native Tools Command Prompt" to configure with CMake and to build with ninja.
* Get [Ninja Build](https://ninja-build.org/). Unzip the binary and put it on your path.

Optional Additions
------------------

### Download And Install ffmpeg (.avi) movie libraries

When the ability to write .avi files is desired, and writing these files is not supported by the OS, Visocyte can attach to an ffmpeg library. This is generally true for Linux. Ffmpeg library source code is found here: [6](http://www.ffmpeg.org/)

### MPI
To run Visocyte in parallel, an [MPI](http://www-unix.mcs.anl.gov/mpi/) implementation is required. If an MPI implementation that exploits special interconnect hardware is provided on your system, we suggest using it for optimal performance. Otherwise, on Linux/Mac, we suggest either [OpenMPI](http://www.open-mpi.org/) or [MPICH](http://www.mpich.org/). On Windows, we suggest [Microsoft MPI](https://msdn.microsoft.com/en-us/library/bb524831.aspx).

### Python
In order to use scripting, [Python](http://www.python.org/) is required (versions 2.7, 3.6, and 3.7.1 are supported). Python is also required for VisocyteWeb builds.

### OSMesa
Off-screen Mesa can be used as a software-renderer for running Visocyte on a server without hardware OpenGL acceleration.


Retrieve the Source
-------------------
* [Install Git](git/README.md) -
  Git 1.7.2 or greater is required for development

* [Develop Visocyte](git/develop.md) - Create a fork and checkout the source code.
    - A useful directory structure is Visocyte/src for the git source tree, Visocyte/build for the build tree, and if needed, Visocyte/install for the install directory.
    - If you wish to contribute bug fixes or features to Visocyte, please follow the instructions in [git/develop.md] for creating a fork and setting up the repository for contributing.

Run CMake
---------
* `cd Visocyte/build`
* `cmake-gui ../src` or `ccmake ../src`
* Generator: choose `ninja` for the ninja build system.
* There are several CMake setting you may consider changing:

| Variable | Value | Description |
| -------- | ----- | ------------|
| VISOCYTE_ENABLE_PYTHON | ON | Add python scripting support |
| BUILD_TESTING | ON/OFF | Build tests if you are contributing to Visocyte |


Build
-----
* `cd Visocyte/build`
* `ninja`

Visocyte will be in `bin/visocyte.exe`

Other Variations
----------------
[VisocyteWeb](http://kitware.github.io/visocyteweb/docs/guides/os_mesa.html) uses Visocyte as a server, so doesn't require the QT gui. It configures OSMesa instead.
