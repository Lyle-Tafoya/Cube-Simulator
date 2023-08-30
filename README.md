# Cube-Simulator

An OpenGL 2.1 NxNxN Cube Twisty Puzzle Simulator
![ScreenShot](https://raw.githubusercontent.com/Lyle-Tafoya/Cube-Simulator/master/screenshot.jpg)

## Controls:

This application uses keyboard controls and is meant to feel somewhat intuitive to speed cubers who know how to touch type with a QWERTY layout keyboard.

j Rotate active top layer clockwise  
f Rotate active top layer counter-clockwise

i Rotate active right layer clockwise  
k Rotate active right layer counter-clockwise

d Rotate active left layer clockwise  
e Rotate active left layer counter-clockwise

s Rotate active bottom layer clockwise  
l Rotate active bottom layer counter-clockwise

w Rotate active back layer clockwise  
o Rotate active back layer counter-clockwise

h Rotate active front layer clockwise  
g Rotate active front layer counter-clockwise

; Rotate entire cube clockwise on y-axis  
a Rotate entire cube counter-clockwise on y-axis

p Rotate entire cube clockwise on z-axis  
q Rotate entire cube counter-clockwise on z-axis

b Rotate entire cube clockwise on x-axis  
y Rotate entire cube counter-clockwise on x-axis

= Zoom in  
\- Zoom Out

arrows Rotate camera around cube  
Backspace Reset Camera  
esc Exit application  
tab Scramble Cube

<number> Create a new cube twisty puzzle with the dimensions NxNxN  
l-shift - Move active layer out by 1  
r-shift - Move active layer in by 1

## Build Instructions

### Linux

    git clone --recursive https://github.com/Lyle-Tafoya/Cube-Simulator.git
    mkdir Cube-Simulator/build
    cd Cube-Simulator/build
    cmake ..
    make

### Windows (MSYS2)

    git clone --recursive https://github.com/Lyle-Tafoya/Cube-Simulator.git
    mkdir Cube-Simulator/build
    cd Cube-Simulator/build
    cmake .. -G'MSYS Makefiles' -DVCPKG_TARGET_TRIPLET='x64-mingw-dynamic'
    make
    
### Windows (Visual Studio)

    "Clone Repository" -> https://github.com/Lyle-Tafoya/Cube-Simulator.git
    Double click on Cube Simulator in Solution Explorer
    "Select Startup Item" -> "CubeSimulator.exe"
    "Build" -> "CubeSimulator.exe"
