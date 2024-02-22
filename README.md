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
  - Secne Renderer
- UI
  - ImGui UI Helper
- Camera
  - Scene Camera
  - Editor Camera
  - Fixed Camera
- Project
  - Project Manager
- Editor Panel 
  - Panel Manager
- Asset
  - Asset Manager
- ECS 
  - Entity
  - Component
  - Scene
  - Serializer

### Detailed Road Map
- Renderer
  - Material (Asset)
  - Light
  - Shadow
  - IBL
- ECS 
  - Script Manager
- Physics (ECS and Editor Panel)
  - 3D Physics

### TODO

## Kreator 
Editor Application using IKan Library to create Games

### Features
- Renderer Layer
  - Welcome Screen
  - Title Bar
  - Open/New/Save - Project/Scene
  - Viewport
- Editor
  - Folder Explorer
  - User Preference
  - Application Setting
- UI
  - UI Helper
- Panels
  - Konsole Panel
  - Content Browser
  - Project Setting
  - Asset Viewer
  - Asset Debugger

### Detailed Road Map
- UI
  - Guizmo (ECS)
- Panels
  - Scene Hierarchy (ECS)
  - Scene Setting (ECS)

### TODO

## Releases
  - v1.0.1.0 : Create and show Window (Without Renderer Context)
  - v2.0.2.0 : Render First Quad
  - v2.1.2.0 : Render First textured Quad
  - v2.2.2.0 : Batch Renderer Quads, Circles and Lines
  - v2.3.2.0 : Text Renderer
  - v3.0.2.0 : Mesh Loader
  - v3.1.2.0 : Cameras
  - v4.0.3.0 : Project/UI/Welcome screen
  - v4.1.3.0 : Asset Manager with Project
  - v4.1.4.0 : Title Bar
  - v4.2.4.1 : Editor Console
  - v4.2.4.2 : Content Browser
  - v4.2.4.3 : Info Panels
  - v4.3.4.4 : Debug Panels
  - v5.0.5.0 : ECS 

## Outputs
![](/Kreator/Outputs/Output.png)
![](/Kreator/Outputs/WelcomeScreen.png)
![](/Kreator/Outputs/FileViewer.png)
