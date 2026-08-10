Sassenach Engine is a barebones multiplayer isometric (2.5D) game engine I'm building to learn SDL2, OpenGL graphics and networking inspired by SS13.
I'll eventually work on a guide for compilation using MinGW g++ code compiler on Code::Blocks which is what I've been using. I've included a binary in this repository that should work on Windows 10.

To build Sassenach you'll need to link against the following libraries:
-lSDL2
-lDevIL
-lILU
-lOpenGL32
-lglew32
-lglu32
-lfreeglut
-lSDL2_image
-lSDL2_net
-lSDL2_test

You'll also need to include glm as a search directory.
