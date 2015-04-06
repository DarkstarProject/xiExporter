XiExporter
------------------
Export FFXI maps into OBJ format.

This is a work in progress. Maps are currently exported as many individual OBJ files.

Import all of the obj files into a 3D modeling program like Blender to construct the full map. View the main.cpp file for settings.

## Building

This must be installed in order to build:
http://www.microsoft.com/en-us/download/confirmation.aspx?id=6812

You'll then need to add the d3dx9.h lib and dll to the linker in Visual Studio.

## Converting Mesh to Darkstar

1. Flip the Z axis
2. Flip the Y axis
2. Flip wrong faces. In Blender blue faces are facing the wrong way.

## RecastDemo Settings

Use these settings when creating a navmesh.

* Tilesize 64

