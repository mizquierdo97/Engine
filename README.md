# Engine

Game Engine developed by Miquel Izquierdo & Guillermo Pinto, students from UPC-CITM.
This engine will allow you to create your own 3D games in a simpler way.

For the moment the engine can load and render different geometry (only support FBX files).
You can Drag and Drop FBX files to load it. Also you can load a texture which will be rendered on the geometry.


##Controls: 

- Load FBXs and Textures: Drag and drop the file. Then select on the assets menu
- Move Camera: W,A,S,D
- Zoom: Mouse wheel
- Rotate over a reference point: Right Click
- Rotate Camera: Right Click + Left Alt
- Center Geometry

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