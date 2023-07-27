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

#### Library Search Paths

#### Preprocessors

#### Known Issues

## Kreator 
Editor Application using IKan Library to create Games

#### Known Issues

#### Output

## Release Tags

## Upcoming Release Tags
v<IKan Major Version>.<IKan Minor Verision>.<Kreator Major Version>.<Kreator Minor Verision>
- v1.0.0.0  : IKan    - Core Application and Layer
  - Setup Project
  - Setup Directory structure
  - File System Utils
  - Logger and Assert
  - Table
  - Timer / Profiler
  - Mouse and Keyboard Codes
  - Event Handler Classes
  - Math Library and Utils
  - Time Step
  - Layer Class
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
  
- v3.2.0.0  : IKan    - Render in frame buffer
  - Full Quad Renderer
  - Frame Buffer  
------------------------------------------------------------------------------------------------------------------
  
- v3.3.0.0  : IKan    - Render 2D Scene in Batch
  - Renderer 2D
  - Full Quad Renderer
 ------------------------------------------------------------------------------------------------------------------  
 
- v3.4.0.0  : IKan    - Render first Text
  - Char Texture
  - Text Renderer
 ------------------------------------------------------------------------------------------------------------------  
  
- v4.0.0.0  : IKan    - entt::entity sysntem (ECS)
  - Entity / Scene / Component classes
 ------------------------------------------------------------------------------------------------------------------  
  
- v4.1.0.0  : IKan    - Scene Serializer
  - SceneSerializer class (YAML Utils)
 ------------------------------------------------------------------------------------------------------------------  

- v5.0.1.0  : IKan    - ImGui Layer  | Kreator - Render Scene in Viewport
  - Kreator Application
  - Kreator Renderer Layer
  - Imgui Layer 
  - View port in Kreator and Render Scene 
 ------------------------------------------------------------------------------------------------------------------
   
- v5.1.1.1  : IKan    - ImGui Color and API | Kreator - UI Theme (Font and Color)
  - ImGui Base Theme class
  - ImGui font loader
  - Decorate the Kreator
 ------------------------------------------------------------------------------------------------------------------
  
- v6.0.2.0  : IKan    - Project and Assets | Kreator - User Prederence for Start Application
  - Get Directory from user.
  - User Preferences
  - Project Class / Manager
  - Asset class / Manaager
 ------------------------------------------------------------------------------------------------------------------
  
- v6.0.2.1  : Kreator - Kreator Title Bar
  - Kreator Title bar
 ------------------------------------------------------------------------------------------------------------------
  
- v6.1.2.2  : IKan    - Content Browser Panel Class | Kreator - Content Browser Panel Usage
  - Entire Content Browser Panel
  - Asset Editor Panel 
  - Folder Explorer

- v6.1.2.3  : Kreator - Scene Playe Pause Simulation Tool bar
  - Scene Play and Pause
  
 ------------------------------------------------------------------------------------------------------------------

- v6.2.2.4  : IKan    - Editor Console, Project Setting | Kreator - All Project, Scene, Log, Setting ... panels
  - Editor Consol for logs
  - Project Setting
  - Application Settings
  - New/Open/Save Project and Scene popups
  - About and Welcome sceen popups
 ------------------------------------------------------------------------------------------------------------------

- v6.3.2.5  : IKan    - Scene Hierarchy panel
  - Scene Hierarchy Panel
  - ECS Debug panel
 ------------------------------------------------------------------------------------------------------------------

- v7.0.3.0  : Kreator - Guizmo and Mouse Selected Entity
  - Mouse Selectable entity 
  - Imguizmo Toolbar and usage
 ------------------------------------------------------------------------------------------------------------------

- v7.1.3.0  : IKan    - Sub texture in quad component
  - Quad Componentn GUI Update for Subtexture
 ------------------------------------------------------------------------------------------------------------------

- v8.0.3.0  : IKan    - Script Integration
 ------------------------------------------------------------------------------------------------------------------

- v9.0.3.0  : IKan    - Physics 2D (Base classes)
  - Component for Physics
  - Scene Handle Physics

- v10.0.3.0  : IKan   - Mesh
 ------------------------------------------------------------------------------------------------------------------

- v11.0.3.0  : IKan   - Light
 ------------------------------------------------------------------------------------------------------------------
