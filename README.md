# IKan-Kreate
IKan-Kreate is a workspace for a revolutionary game engine designed to empower game developers on the macOS platform. 
Leveraging the power of OpenGL, this project aims to provide a versatile and robust framework that enables developers 
to unleash their creativity and build visually stunning and immersive gaming experiences.

![](/Resources/Logo/IKan.png)

## Getting Started
XCode 14.3 is recommended

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/ashish1009/IKan-Kreate`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

## IKan Library
IKan Library can be used independently to develop your own Editor/Game

### How to Use the library
`{...} directory where IKan Engine is cloned`
  - Link the libikan.a in your project...
  - Link libz.tbd framwork
  - Link libbz.2.1.0.8.tbd framwork
  - Include the IKHeaders.h to use all the interface files that are important

### Preprocessors
  - `IK_ENABLE_LOG` : To enable logger in Engine 
  - `IK_ENABLE_ASSERT` : To enable assert in Engine 

### Header Search Paths
`{...} directory where IKan Engine is cloned`
  - {...}/Interface
  - {...}/Vendors/SpdLog/include
  - {...}/Vendors/Glm
  - {...}/Vendors/UI/ImGui

### Libraries
  - 

## Kreator
Kreator is an editor used to develop games using IKan Library and other submodules

## Release
v {IKanVersion}.{IKanSubVersion}.{KreatorVersion}.{KreatorSubVersion}
- v1.0.1.0 : Application and layer
- v1.1.1.0 : GLFW Window
- v1.2.1.0 : ImGui UI Layer
- v1.3.1.1 : Renderer Command Queue
- v2.0.1.1 : Batch 2D Renderer
- v2.1.1.1 : Text Renderer
- v2.2.1.1 : Camera Rendering

## Road Map
- Graphic APIs
    - ~ Texture
      - Image Loader
    - Framebuffer
- Viewports and Framebuffers
- Mesh Loader
- Mesh Materials
- Project
- Assets
  - Asset registry
  - Asset manager
- Editor UI
  - Welcome screen
  - Title bar
  - Editor panels
    - Content browser panel
    - Asset panel
    - Settings
    - Log panel
- ECS
  - Scene
  - Component
  - Entity
  - Serializer
  - Scene hierarchy panel
- Scene [3D]
    - Scene Renderer
    - Lights
    - Materials
    - Physics
- Advance Viewports
  - Guizmo
  - Mouse and key controlled ECS
    - Add / Delete / Duplicate / Undo / Red
  - Icons
  - Helper 
    - Mini viewport
    - Secondary viewport
    - Camera Axis helper
- Camera controller 
- Scene [2D]
  - Renderer
  - Physics 
  - Lights 
  - Materials
- Advance Open GL
  - Advance light
  - Instancing
- Mutli Threading
- Scripts
- Partical system

## TODO

## Output
- Kreator

![](/Kreator/Outputs/Kreator.png)
