# GEO1004
Assignment 1 - Triangulating a polygon mesh with generalised maps

Authors: Eleni Theodoridou, Guilherme Spinoza Andreo

## What does the code do?
Takes an .obj file as input, parses it and builds the generalised map data structure.
This is done by looping through each face, making unique IDs using indexes of the elements, then defining the unique darts present in the whole structure. 
The triangulation is done using the values present in the gmap structure, where new points created using barycenter points for the faces and the edges.  

It is functional for a single volume with faces that are all convex without any holes.

The final product is 5 csv files, (torus_darts.csv, torus_vertices.csv, torus_edges.csv, torus_faces.csv, torus_volume.csv),
and a triangulated mesh in obj format (tours_triangulated.csv).

You can use MeshLab software or Blender to visualise the final OBJ file.
## How to build from command line

The repository is public and available to be cloned.
This code was executed using Clion IDE (Build #CL-213.6777.58) 2021.3.3

Check and edit the CMakelists.txt, so that it is able to find the libraries of their relative paths

```
    $ git clone https://github.com/guispinoza/geo1004_hw1
    $ cd geo1004_hw1/hw/01/cpp
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./hw01
  
```
Download with the zip file, extract and run:
```
    $ cd https://github.com/guispinoza/geo1004_hw1.git/hw/01/cpp
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./hw01

```
## Alternative build - You can also open the `cpp` folder directly in the IDE of your choice.
If the filepath in the command line 16-23 isn't working, just add the .obj files to the build folder.