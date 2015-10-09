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

## Importing Into Blender

Currently, all objects on the map are exporting into individual files. To create
the full mesh:

1. Copy all OBJ files into "C:\objects".
2. Open a new Blender file and delete everything.
3. Open up the "Text Editor" window.
4. Create a new file (Ctrl+N).
5. Paste in this script:

```
import os
import bpy


# put the location to the folder where the objs are located here in this fashion# this line will only work on windows ie C:\objects
path_to_obj_dir = os.path.join('C:\\', 'objects')

# get list of all files in directory
file_list = sorted(os.listdir(path_to_obj_dir))

# get a list of files ending in 'obj'
obj_list = [item for item in file_list if item[-3:] == 'obj']


# loop through the strings in obj_list and add the files to the scene
for item in obj_list:
    path_to_file = os.path.join(path_to_obj_dir, item)
    bpy.ops.import_scene.obj(filepath = path_to_file)
```

6. Run the script.
7. Export the map as OBJ and you're done.

## Prepping Mesh for Navmeshing

1. Mirror the Y axis (Order matters)
2. Mirror the Z axis
3. Flip wrong faces (select object, tab, a, w, flip faces, press tab to exit). In Blender blue faces are facing the wrong way.
4. Delete walkable meshes. Meshes which do not stop player movement (bushes)
   should be removed.
5. Delete walkable areas which shouldn't be walkable (lakes.)

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

@pos in FFXI -> (100, 5, 20)
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
