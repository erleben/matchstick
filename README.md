# MATCHSTIK PROX
Research related to computational modeling on projects for contact, biomechanics, soft robots and more

## Install Tools

Make sure you got latest version of CMake installed on your system and your favorite IDE/compiler.

## Install Dependencies

Before compiling and linking one needs to install Boost, Glew, and Qt

### Dependencies on Apple


Dependencies can be easy installed using macport (you need to install macport if you do not already have it)

    sudo port install boost@1.66.0_3+no_single-no_static+python36+universal
    sudo port install glew@1.9.0_0+universal

### Dependencies on Linux

#### Ubuntu based versions

Install dependencies:

    $ sudo apt-get install build-essential cmake cmake-curses-gui libboost-all-dev libglew-dev libxi-dev libxmu-dev --assume-yes

This has been tested on a Precise64 Virtual Box and results in the following versions:

- gcc (Ubuntu/Linaro 4.6.3-1ubuntu5)
- cmake version 2.8.7
- libglut.so.3.9.0, libGLU.so.1.3.08004, libGLEW.so.1.6.0, libGL.so.1.2
- Boost 1.46.1
- libXi.so.6.1.0, libXmu.so.6.2.0

#### Dependencies on Windows

Unknown as of this writing

## How to Compile and Link

CMake should be used for generating make/solution files.

* Open the cmake gui application

* Browse the source path to the top most folder containing a CMakeLists.txt file.

* For binary destination use the same folder location but add a
subfolder to contain the binary tree. You can name it whatever you
want. For instance "build". Press configure button, resolve any conflicts or issues.

* If you want to have unit-testing find the cmake variable ENABLE_UNIT_TESTS and set it to true (=1)

* Press configure button again until no red variables appear. Now press generate button.

* Goto the build folder and locate your solution/make files and use these to compile and link the sub project.


# Known Issues and Wanted Features

## ID 1

Add proper ProxEngine params for gravity and damping parameters so
they too use the set_parameter interface


## ID 2

PROX_GUI loads cfg-files twice. The
reason is that first load_from_config reads in the cfg file in order
to set up application logic. Later prox engine loads the cfg-files
again when trying to set its parameters from a cfg-file.

This seems a little "wasteful" to parse the same files
twice... However, it make prox engine have a clean interface with
minimal usage of data types.

## ID3

Find a more gentle way to handle Qt in CMakeLists.txt. Currently one have to edit

    SET( CMAKE_PREFIX_PATH "/Users/kenny/Qt/5.3/clang_64/" ${TMP_CMAKE_PREFIX_PATH} )

To match own system depending installation.

