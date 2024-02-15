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
IKan Library can be used separately independent to develop your own Editor/Game

### How to Use the library
`{...} directory where IKan Engine is cloned`
- Link the libikan.a in your project...
- Include the IKHeaders.h to use all the interface files that are important

#### Header Search Paths
`{...} directory where IKan Engine is cloned`
- {...}/Interface
- {...}/Vendors/SpdLog/include
- {...}/Vendors/Glm

### Features
- Base
  - Buffer
- Utilities
  - String
  - Maths
  - YAML
- Core
  - Logger
  - Profiler
  - Application
  - Layer
  - Event and Input Handler
  - Window 
- Renderer
  - Graphics APIs
    - Context
    - Shader
    - Buffers
    - Pipeline
    - Texture
    - Framebuffer
  - Renderer API
  - Batch Renderer
  - Font Loader
  - Text Renderer
  - Mesh
- UI
  - ImGui UI Helper
- Camera
  - Scene Camera
  - Editor Camera
  - Fixed Camera
- Project
  - Project Manager

### Detailed Road Map
- Asset
  - Asset Manager
- Renderer
  - Material (Asset)
  - Secne Renderer (ECS)
  - Light
  - Shadow
  - IBL
- Editor Panel 
  - Panel Manager
- ECS (Panels)
  - Entity
  - Component
  - Scene
  - Serializer
  - Script Manager
- Physics (ECS and Editor Panel)
  - 3D Physics

### TODO

## Kreator 
Editor Application using IKan Library to create Games

### Features
- Renderer Layer

### Detailed Road Map
- Renderer Layer
  - Title Bar
  - Welcome Screen
  - Open/New/Save - Project/Scene
- Editor
  - Folder Explorer
  - User Preference
  - Application Setting
  - Asset Viewer
- UI
  - UI Helper
  - Guizmo
- Panles
  - Scene Hierarchy
  - Content Browser
  - Scene Setting
  - Konsole Panel
  - Project Setting
  - Asset Information

### TODO

## Releases
  - v1.0.1.0 : Create and show Window (Without Renderer Context)
  - v2.0.2.0 : Render First Quad
  - v2.1.2.0 : Render First textured Quad
  - v2.2.2.0 : Batch Renderer Quads, Circles and Lines
  - v2.3.2.0 : Text Renderer
  - v3.0.2.0 : Mesh Loader
  - v3.1.2.0 : Cameras
  - v4.0.3.0 : Project/UI
## Outputs
![](/Kreator/Outputs/Output.png)
