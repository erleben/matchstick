# MATCHSTIK PROX
Research related code demonstrating how anisotropic friction modeling can be incorporated into a proximal operator setting for solving contact forces with iterative methods. The code is based on the papers:

* Robert Schmidtke and Kenny Erleben. 2017. Chunked bounding volume hierarchies for fast digital prototyping using volumetric meshes. IEEE Trans. Vis. Comput. Graph. 99 (2017)
* Kenny Erleben, Rigid body contact problems using proximal operators, Proceedings of the ACM SIGGRAPH / Eurographics Symposium on Computer Animation, July 28-30, 2017, Los Angeles, California
* Kenny Erleben, Methodology for Assessing Mesh-Based Contact Point Methods, ACM Transactions on Graphics (TOG), v.37 n.3, August 2018

The code has been developed and tested using

* macOS Mojave ver 10.14.3. 
* CMake ver 3.13.3
* Xcode ver 10.1
* Macport 2.5.4 (for installing Boost, Glew and Qt)

Dependencies can be installed as follows

    sudo port install python37
    sudo port install boost @1.66.0_3+no_single-no_static+python37
    sudo port install glew @2.1.0

## How to Compile and Link

CMake should be used for generating make/solution files.

* Open the cmake gui application

* Browse the source path to the top most folder containing a CMakeLists.txt file.

* For binary destination use the same folder location but add a subfolder to contain the binary tree. You can name it whatever you want. For instance "build". Press configure button, resolve any conflicts or issues.

* If you want to have unit-testing find the cmake variable ENABLE_UNIT_TESTS and set it to true (=1)

* Press configure button again until no red variables appear. Now press generate button.

* Goto the build folder and locate your solution/make files and use these to compile and link the sub project.
