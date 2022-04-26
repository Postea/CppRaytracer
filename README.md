# CppRaytracer

#### Requirements:
- *Visual Studio Developer Command Prompt* (https://docs.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2022)
- Make sure you have a compiler for *C++17*

#### Compile and execute:
- Start the *VS Developer Command Prompt*
- Create a new folder *\bin*
  > mkdir bin
- Compile the code
  > cl.exe /Zi /EHsc /Fe: bin/raytracer.exe RayTracer/*.cpp Raytracer/tools/*.cpp Raytracer/camera/*.cpp Raytracer/material/*.cpp Raytracer/material/texture/*.cpp Raytracer/sampling/*.cpp Raytracer/shape/*.cpp Raytracer/testing/*.cpp /Fo:bin/ /Fd:bin/vs140.fdb /std:c++17 /MP
- Executable at *\bin\raytracer.exe*
