XiExporter
------------------

Export FFXI maps into OBJ format.

This is a work in progress. Maps are currently exported into many individual OBJ files.

## Building

You must install DirectX 9 SDK. Here is the link:
http://www.microsoft.com/en-us/download/confirmation.aspx?id=6812

You'll then need to add the d3dx9.h, lib, and dll to the linker in Visual Studio.

## Running

From the cmd prompt, enter in the xiExporter exe and id of the zone you want to
be exported.

```
xiExporter.exe {zoneId}
```

All the maps objects will be exported to your current directory.
Add a regex for mesh name to filters.txt (ensure it's in same directory as exe) to include/exclude objects (useful for filtering out flora).

## Importing Into Blender
Delete all objects in current scene (press [A] key twice > Del)
File > Import > Wavefront (.obj)
In bottom left pane uncheck split by group if you want to remove whole objects instead of individual meshes.

## Prepping Mesh for Navmeshing

1. Flip wrong faces (select object, tab, a, w, flip faces, press tab to exit). In Blender blue faces are facing the wrong way.
2. Delete walkable meshes. Meshes which do not stop player movement (bushes)
   should be removed.
3. Delete walkable areas which shouldn't be walkable (lakes.)

### Flipping Objects in Batches

```
import bpy
scn = bpy.context.scene
sel = bpy.context.selected_objects
meshes = [o for o in sel if o.type == 'MESH']

for obj in meshes:
    scn.objects.active = obj
    bpy.ops.object.editmode_toggle()
    bpy.ops.mesh.select_all(action='SELECT')

    bpy.ops.mesh.flip_normals() # just flip normals
    # bpy.ops.mesh.normals_make_consistent(inside=True) # or recalculate inside
    # bpy.ops.mesh.normals_make_consistent(inside=False) # or recalculate outside

    bpy.ops.object.mode_set()
```

Copy this into a file tab and click run. This will flip all the faces of every
object you have selected.

### Checking Coordinates

To validate the map has been rotated correctly, jump into the game and compare
points from the game via @pos and points in Blender. The y and z coordinates from FFXI
should be swapped and the new z should be reversed.

Example:

!pos in FFXI -> (100, 5, 20)
Formula: (x, z, -y)
So the coordinates in Blender should be (100, 20, -5).

Easiest way to test this is by moving the 3D cursor to the position. In Blender
press N and scroll down to 3D Cursor, then input the coordinates.


## RecastDemo Settings

Use these settings when creating a navmesh.

* Tile Size 256
* Cell Size 0.40


## Setting up Recast Editor

This assumes you're using Ubuntu 14.04.

```
git pull recastnavigation
sudo apt-get install libsdl1.2-dev
sudo apt-get install premake4
cd recastnavigation/RecastDemo
premake4 gmake
cd Build/gmake
make
cd RecastDemo/Bin
./RecastDemo
```

Under sample select tile mesh.
Move your obj files into the Meshes folder and import into the editor.
Configure the navmesh settings (tilesize 256, agent radius 0.5)
Build then save
In the RecastDemo/Bin folder rename the new file `all_tiles_navmesh.bin` to
`name.nav` and you're done.
