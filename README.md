
Github: https://github.com/mizquierdo97/Engine
Authors: Miquel Izquierdo, Guillermo Pinto

# Engine

Game Engine developed by Miquel Izquierdo & Guillermo Pinto, students from UPC-CITM.
This engine will allow you to create your own 3D games in a simpler way.

For the moment the engine can load and render different geometry (only support FBX files).
You can Drag and Drop FBX files to load it. Also you can load a texture which will be rendered on the geometry.


##Controls: 

- Import assets: Copy them in the Asstes folder
- Load FBXs: Click it in the assets menu
- Move Camera: W,A,S,D
- Zoom: Mouse wheel
- Rotate over a reference point: Right Click
- Rotate Camera: Right Click + Left Alt
- Center Geometry: F
- Change Gizmos: 1,2,3
- Open File Menu: Right Click on the desired asset in "Assets" menu
- Save/Load scene in Edit Menu

*All the FBX's textures should be in the same folder of the FBX

Libraries used:

-SDL

-Open GL

-ImGui

-MathGeoLib

-JSON Parson

-Assimp

-DevIL

##ChageLOG:

-0.0.3: 
Sphere can be printed on the screen.
You can change the render mode between Shaded and Wireframe.

-0.0.4:
We can export different FBX and load the meshes, alse we can see the normals of the vertexs.
More primitives are created, but we can't use yet.
User interface updated with Imgui Docking (needs improvement)

-0.0.5
We can export assets to an asset menu and load when we need it.
Camera controls improved, you can zoom with mouse wheel.

-0.1.0
New camera controls
Bugs Fixed 

-0.1.1
Import FBX to our own format.
Import images to DDS.
Added camera frustum, and frustum culling.
Change object transform in real time.
Change selected object texture.

-0.1.2
MousePicking added for GameObjects.
Guizmos for transform objects added.
Save/Load Scene.
Resource Manager.
Reference counting for resources.
Camera options added.
List of statics, accelerated with quadtree.

-0.1.3
META files added
Runtime Texture Update

-0.2.0
Change Texture/Mesh of an object
Octree Improved
Create Objects and add a component Mesh/Texture
Rename/Delete options for files (Runtime)