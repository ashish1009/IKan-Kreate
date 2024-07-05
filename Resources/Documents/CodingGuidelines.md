#  Coding Guidelines and File format

The "IKan-Kreate" uses a set of guidelines and best practices for writing C++ code. These coding standards aim to 
improve code quality, readability, maintainability, and portability. The guidelines cover various aspects of C++
programming, including naming conventions, code organization, comments, error handling, and more.

## File name format
- `PascalCase`: File names should use PascalCase, which means the first letter of each word is capitalized and there are 
                no spaces or underscores between words.
- `Meaningful and Descriptive Names`: File names should be meaningful and descriptive, reflecting the purpose or content 
                                      of the file.
- `Use of Standard File Extensions`: Use standard file extensions appropriate for the file's content or purpose. 
                                     For example, .cpp for C++ source files, .hpp for C++ header files etc.

 Here are a few examples that demonstrate file names:
  - C++ Source File: MyClass.cpp
  - C++ Header File: MyClass.hpp
  - Resource File: MyResource.resx

## Naming Conventions:
  - Use PascalCase for type names and camelCase for variable and function names.
  - Prefix member variables with "m_".
  - Use descriptive and meaningful names that reflect the purpose of the entity.
  
  Examples
  - `Type Names`: class MyClass, struct MyStruct, enum MyEnum
  - `Variable Names`: int myVariable, float myFloat
  - `Function Names`: void myFunction(), int calculateSum()
  - `Member Variables`: int m_myMemberVariable, std::string m_name
  - `Static Variables`: int s_staticVariable, std::string s_name
  - `Globals Static`: s_GlobalStatic
  - `Enum Name`: enum class EnumName { EnumValue, OtherValue}
  - `Constants`: const uint32_t ConstData
  - `Pointers`: int32_t* pdataPointer
  - `Constant Pointers`: const char* pConsntPointer
  - `Globals`: g_globalVariable
  - `Global Constants`: ConstGlobal

## Code Organization:
  - Organize code into logical modules and namespaces.
  - Use header files (.hpp) for declarations and source files (.cpp) for definitions.
  - Place include guards in header files to prevent multiple inclusion.
  - Follow a consistent order for including standard and third-party headers.

## Coding Style:
  - Use Allman style braces with opening braces on a new line.
  - Limit line length to 120 characters.
  - Use meaningful indentation and whitespace to enhance code readability.

## Error Handling:
  - Use exceptions for handling exceptional conditions, rather than error codes.
  - Use RAII (Resource Acquisition Is Initialization) to manage resource allocation and deallocation.

## Comments and Documentation:
  - Use meaningful comments to explain the purpose, behavior, and usage of code.
  - Document public APIs using XML comments for generating documentation.
  - Class responsibility should be added in comment
