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
    sudo port install qt5 @5.12.0

## How to Compile and Link

CMake should be used for generating make/solution files.

* Open the cmake gui application

* Browse the source path to the top most folder containing a CMakeLists.txt file.

* For binary destination use the same folder location but add a subfolder to contain the binary tree. You can name it whatever you want. For instance "build". Press configure button, resolve any conflicts or issues.

* If you want to have unit-testing find the cmake variable ENABLE_UNIT_TESTS and set it to true (=1)

* Press configure button again until no red variables appear. Now press generate button.

* Goto the build folder and locate your solution/make files and use these to compile and link the sub project.

# Getting Overview of the Code

The actual matchstick model i simplemented in a single file

[prox_matchstick_model.h](https://github.com/diku-dk/matchstick/blob/master/PROX/SIMULATION/PROX/PROX/include/prox_matchstick_model.h)

In this file the method "compute_coefficients_of_friction" and "compute_contact_frame" gives the core functionality that generates the coefficient and orientation of the anisotropic friction cone.

These functions are used respectively in the files 

[prox_get_friction_coefficient_vector.h](https://github.com/diku-dk/matchstick/blob/master/PROX/SIMULATION/PROX/PROX/include/prox_get_friction_coefficient_vector.h)
[prox_get_jacobian_matrix.h](https://github.com/diku-dk/matchstick/blob/master/PROX/SIMULATION/PROX/PROX/include/prox_get_jacobian_matrix.h)

The first file implements a function that assembles a vector of friction coefficients and the other file implements a function that assembles a Jacobian matrix. These two functions are invoked in a time-stepper before invoking the contact force solver. A time stepper implementation can be seen in this file

[prox_moreau_time_stepper.h](https://github.com/diku-dk/matchstick/blob/master/PROX/SIMULATION/PROX/PROX/include/steppers/prox_moreau_time_stepper.h)

The stepper invokes a prox solver function after having assembled all the data needed that defines the contact force problem. A typical Gauss-Seidle type of solver can be seen in the file

[prox_gauss_seidel_solver.h](https://github.com/diku-dk/matchstick/blob/master/PROX/SIMULATION/PROX/PROX/include/solvers/prox_gauss_seidel_solver.h)

Here the solver is simply one function that iterates over the contact points and invokes a proximal operator first on the normal block of the contact problem and then the friction block of the contact. 

This code supports different types of steppers, solvers and sub-solvers (proximal operators). Template binders are used to setup the specific combination based on settings given in the cfg files located in the bin-folder.

## Some Design Notes

Current implementation assumes that the friction cone model only depends on positional information and that such information does not change during the prox solver invokation. This allows the model is be instanciated once in the assembly functions get_friction_coefficient_vector and get_jacobian_matrix. If a more implicit time-stepping scheme is chosen then positions and orientations could potentially change while iteration. This would be the case in a primal dual Newton type of approach. Such as the one presented in 

* Miles Macklin, Kenny Erleben, Matthias Muller, Nuttapong Chentanez, Stefan Jeschke, and Viktor Makoviychuk: Non-Smooth Newton Methods for Deformable Multi-Body Dynamics, ACM TOG, 2019

This essential mean the friction cones could be changing while iterating in the contact solver.
