#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "Textures.h"

controls rendering_controls;
SDL_Event e;
SDL_Renderer* renderer;
float increment;

//contexts
SDL_Window* gWindow;
//OpenGL context
SDL_GLContext gContext;

bool initGL()
{
    //Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    //Enable texturing
    glEnable( GL_TEXTURE_2D );
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    //Initialize DevIL
    ilInit();
    ilClearColour( 255, 255, 255, 000 );
    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }
    return true;
}

bool initialize_graphical_window()
{
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
	 	printf( "SDL could not initialize! SDL_INIT_VIDEO" );
		success = false;
	}
	else if  ( SDL_Init(SDL_INIT_TIMER) < 0 )
    {
		printf( "SDL could not initialize! SDL_INIT_TIMER" );
		success = false;
	}
	else
	{
	    printf( "Initialized video\n" );
		printf( "Initialized timer\n" );
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "Sassenach", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}
				//Initialize OpenGL
				if( !initGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}


		glewExperimental = GL_TRUE;
		glutInitDisplayMode(GLUT_RGB);
		GLenum status = glewInit();
		if (status != GLEW_OK) {
		    std::cerr << "GLEW Error: " << glewGetErrorString(status) << std::endl;
		}
	}
	return success;
}

void renderer_loop() {


}

#endif // MAIN_H_INCLUDED
