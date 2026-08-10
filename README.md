Sassenach Engine is a barebones multiplayer isometric (2.5D) game engine I'm building to learn SDL2, OpenGL graphics and networking inspired by SS13.
I'll eventually work on a guide for compilation using MinGW g++ code compiler on Code::Blocks which is what I've been using. I've included a binary in this repository that should work on Windows 10.

To build Sassenach you'll need to link against the following libraries (in order of inclusion):
-lSDL2 -lDevIL -lILU -lOpenGL32 -lglew32 -lglu32 -lfreeglut -lSDL2_image -lSDL2_net -lSDL2_test

You should include the following files in your project when compiling: main.cpp, Maploading.cpp, Textures.cpp, Utilities.cpp, Camera.h, clientside_ally_with_info.h, Configuration.h, Events.h, Generic_Models.h, Events.h, Geometries.h, Initialize_Bullshit.h, Logging.h, main.h, MapLoading.h, Network.h, Network_client_macros.h, Sound_Initializer.h, Textures.h, Utilities.h

Or alternatively perhaps main.cpp and main.h will suffice, I've been too lazy to check

You'll also need to download and include glm as a search directory.
