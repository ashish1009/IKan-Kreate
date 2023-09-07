# IKan-Kreate
IKan-Kreate is a revolutionary game engine designed to empower game developers on the macOS platform. 
Leveraging the power of OpenGL, this project aims to provide a versatile and robust framework that enables developers 
to unleash their creativity and build visually stunning and immersive gaming experiences.

![](/Resources/Logo/i_kan.png)

## Getting Started
XCode 14.3 is recommended

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/ashish1009/IKan-Kreate`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

## IKan Library
IKan Library can be used separately independent to develop your own Editor/Game

### How to Use the library
`{...} directory where IKan Engine is cloned`
- Link the libikan.a in your project...
- Include the IKanHeaders.h to use all the interface files that are important

#### Header Search Paths
`{...} directory where IKan Engine is cloned`
- {...}/Interface
- {...}/Vendors/SpdLog/include
- {...}/Vendors/Glm
- {...}/Vendors/GLFW/GLFW/include
- {...}/Vendors/Freetype/include
- {...}/Vendors/UI/ImGui
- {...}/Vendors/EnTT
- {...}/Vendors/Yaml/Yaml

#### Library Search Paths
Add the following header search paths 
- libz.tbd
- libbz2.1.0.8

#### Preprocessors
- IK_ENABLE_LOG     : For Enable Logs

#### Known Issues

## Kreator 
Editor Application using IKan Library to create Games

#### Known Issues

#### Output

## Release Tags
v(IKan Major Version).(IKan Minor Verision).(Kreator Major Version)(Kreator Minor Verision)

- v1.0.0.0  : IKan    - Core Application and Layer
  - Setup Project
  - Setup Directory structure
  - File System Utils
  - Logger and Assert
  - Timer / Profiler
  - Mouse and Keyboard Codes
  - Event Handler Classes
  - Math Library and Utils
  - Time Step
  - Application
  ------------------------------------------------------------------------------------------------------------------

- v2.0.0.0  : IKan    - Window and Events 
  - Window interface and implementation
  - Renderer Context
  - Input Handler
  ------------------------------------------------------------------------------------------------------------------

- v3.0.0.0  : IKan    - Render first Color Quad 
  - Renderer API class
  - Shader / String Utils
  - Vertex Buffer / Index Buffer
  - Pipeline
  - Camera (Scene / Editor / Fixed)
------------------------------------------------------------------------------------------------------------------  

- v3.1.0.0  : IKan    - Render first Texture Quad 
  - Image File Texture loader  
  - Sub Texture Loader
------------------------------------------------------------------------------------------------------------------  

- v3.2.0.0  : IKan    - Full Screen quad
  - Full Quad Renderer
------------------------------------------------------------------------------------------------------------------

- v3.3.0.0  : IKan    - Render Scene with camera 
------------------------------------------------------------------------------------------------------------------

- v3.4.0.0  : IKan    - Render in frame buffer
  - Frame Buffer  
------------------------------------------------------------------------------------------------------------------

- v3.5.0.0  : IKan    - Render 2D Scene in Batch
 ------------------------------------------------------------------------------------------------------------------  

- v3.6.0.0  : IKan    - Render first Text
  - Char Texture
  - Text Renderer
 ------------------------------------------------------------------------------------------------------------------  

- v3.7.0.0  : IKan    - Layer
  - Kreator App and Layer
 ------------------------------------------------------------------------------------------------------------------  

- v4.0.1.0  : IKan    - ImGui Layer  | Kreator - Render Scene in Viewport
  - ImGui Layer 
  - Kreator Application
  - Kreator Renderer Layer
  - View port in Kreator and Render Scene 
  
 ------------------------------------------------------------------------------------------------------------------
- v5.0.1.0  : IKan    - entt::entity sysntem (ECS)
  - Entity / Scene / Component classes
 ------------------------------------------------------------------------------------------------------------------  
  
- v5.1.1.0  : IKan    - Asset Base class and Serializer
 ------------------------------------------------------------------------------------------------------------------  
  
- v6.0.2.0  : IKan    - Project Classes
            : Kreator - Create New Project
 ------------------------------------------------------------------------------------------------------------------  

- v6.1.2.0  : IKan    - Render Scene With ECS
 ------------------------------------------------------------------------------------------------------------------  

- v6.2.2.0  : IKan    - Scene Serializer
  - SceneSerializer class (YAML Utils)
 ------------------------------------------------------------------------------------------------------------------  

- v7.0.2.0  : IKan    - ImGui Theme
 ------------------------------------------------------------------------------------------------------------------  

- v7.0.3.0  : Kreator - Kreator Title Bar
- v7.0.3.1  : Kreator - Kreator Welcome Screen
- v7.0.3.2  : Kreator - Folder Explorer to open Project and create new
 ------------------------------------------------------------------------------------------------------------------

- v8.0.4.0  : IKan    - Editor Console Panel
              Kreator - Editor Consol for logs
 ------------------------------------------------------------------------------------------------------------------

- v8.1.4.1  : IKan    - Editor Console, Project Setting | Kreator - All Project, Scene, Log, Setting ... panels
  - Project Setting
  - Application Settings
 ------------------------------------------------------------------------------------------------------------------

- v8.1.4.2  : Kretor  - Content Browser Panel Class
              Kretor  - Asset Editor Panel 
 ------------------------------------------------------------------------------------------------------------------

- v8.1.5.0  : Kreator - Scene Hierarchy panel
 ------------------------------------------------------------------------------------------------------------------

- v8.2.5.1  : IKan    - ImGuizmo 
              Kreator - Guizmo Toolbar
 ------------------------------------------------------------------------------------------------------------------

- V8.2.5.2  : Kreator - Scene Playe Pause Simulation Tool bar
 ------------------------------------------------------------------------------------------------------------------

- V9.0.6.0  : Mesh - Loader
 ------------------------------------------------------------------------------------------------------------------

- V9.1.6.1  : Mesh From Scene
 ------------------------------------------------------------------------------------------------------------------

- V9.2.6.2  : Mesh Scene hierarchy panel
 ------------------------------------------------------------------------------------------------------------------

- V9.3.6.3  : Mesh Mouse Selection
 ------------------------------------------------------------------------------------------------------------------

- V10.0.7.0 : 3D Physics
 ------------------------------------------------------------------------------------------------------------------

- V10.0.8.0 : Clean Editor Mini Viewport Bug Fixes
 ------------------------------------------------------------------------------------------------------------------

## TODO  
- Grid
- Selected Mesh Highlight
- Joints
- Light
- Materials
- Sub texture in quad component
- Script Integration
- Multiple Selection of Entity
- Collider for Submesh
 ------------------------------------------------------------------------------------------------------------------

#### Output
![](/Kreator/Outputs/Kreator_Welcome.png)
 ------------------------------------------------------------------------------------------------------------------
![](/Kreator/Outputs/Kreator_FolderExplorer.png)
 ------------------------------------------------------------------------------------------------------------------
![](/Kreator/Outputs/Kreator.png)
