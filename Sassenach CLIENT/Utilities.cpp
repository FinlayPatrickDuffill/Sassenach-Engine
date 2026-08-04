#include "Utilities.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "Textures.h"
#include <SDL.h>
#include <SDL_events.h>
#include <iostream>
#include <fstream>

int ANGLE_ROTATION_FACTOR_UTIL = 2;
int FULL_ROTATION_UTIL = 142.f;
int metric, image_memory_position = 0;

//File loaded texture
LTexture gLoadedTexture;

bool loadMedia(int image_identity)
{
    //Load texture
    if( !gLoadedTexture.loadTextureFromFile( "assets/floor.png", 0 ) )
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }
    return true;
}

//Load a particular media file is the idea
bool loadMedia_particular(int image_identity, const char* filename)
{
    // printf("loadMedia_particular called -  image identity is: %d \n", image_identity);
    //Load texture
    if( !gLoadedTexture.loadTextureFromFile( filename, image_identity ) )
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }
    return true;
}

void update()
{

}

trigonometry_rendering trig;

void render(SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls)
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;
    //Rendering a floor
    set_angle(rendering_controls.angle_direction);
    gLoadedTexture.render_trigonometry( trig.angle_flag, x, y, trig );
}

void render_userrelation(ObjectArray *chunk, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls)
{
    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;
    //Rendering a floor
    // set_angle(rendering_controls.angle_direction);
    gLoadedTexture.render_trigonometry_userrelation(rendering_controls, chunk, user_perspective, trig.angle_flag, x, y, trig );
    //Update screen
}

void render_userrelation_N(ObjectArray *chunk, ObjectArray *peripheral_chunk_N, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls)
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;
    //Rendering a floor
    // set_angle(rendering_controls.angle_direction);
    gLoadedTexture.render_trigonometry_userrelation(rendering_controls, chunk, user_perspective, trig.angle_flag, x, y, trig );
    //Update screen
}

void initial_texture_loading()
{
       if( !loadMedia_particular(1, "assets/player2.png"))
    {
        add_to_log( "Couldn't load a texture\n" );
    }
    else
    {
        image_memory_position++;
        add_to_log("Floor texture loaded\n");
    }

    // initial asset loading
    if( !loadMedia_particular(2, "assets/floor2.png") )
    {
        add_to_log( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        add_to_log("Floor texture loaded\n");
    }
    if( !loadMedia_particular(3, "assets/floor3.png") )
    {
        add_to_log( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        add_to_log("Floor texture loaded\n");
    }
    if( !loadMedia_particular(4, "assets/player.png") )
    {
        add_to_log( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        add_to_log("Player texture loaded\n");
    }
        if( !loadMedia_particular(5, "assets/floor4.png") )
    {
        add_to_log( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        add_to_log("Floor texture loaded\n");
    }
        if( !loadMedia_particular(5, "assets/somemid.png") )
    {
        add_to_log( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        add_to_log("Floor texture loaded\n");
    }
}

gameworld_constants load_terminal_alphabet(gameworld_constants gameworld_constants_logic)
{
std::string terminal_alphabet_filepath = "assets/alphabet/letter_";
const char* terminal_alphabet_filepath_cchar = "assets/alphabet/letter_x";
char terminal_alphabet_appended_letter = 'a';
// printf("%s \n", player_datum_begin);
for (int i=0;i!=26;i++)
    {
        terminal_alphabet_filepath= "assets/alphabet/letter_";
        terminal_alphabet_filepath += terminal_alphabet_appended_letter;
        terminal_alphabet_filepath += ".png";
        terminal_alphabet_appended_letter++;
        terminal_alphabet_filepath_cchar = terminal_alphabet_filepath.c_str();
        if( !loadMedia_particular(6, terminal_alphabet_filepath_cchar) )
            {

            }
        else
            {
                image_memory_position++;
                printf("Alphabet loaded\n");
            }
        }
        // inform DevIL where the end of the IDs on the heap loaded into memory to represent an alphabet is
        gameworld_constants_logic.termination_of_loaded_alphabet_in_memory = image_memory_position;
        // update the gameworld constants
    return gameworld_constants_logic;
}

void render_chunks_primitive_layer(
     ObjectArray *peripheral_chunk_SW,
     ObjectArray *peripheral_chunk_WN,
     ObjectArray *peripheral_chunk_N,
     ObjectArray *peripheral_chunk_NE,
     ObjectArray *peripheral_chunk_E,
     ObjectArray *peripheral_chunk_ES,
     ObjectArray *peripheral_chunk_W,
     ObjectArray *peripheral_chunk_S,
     ObjectArray *chunk,
     user_perspective debug_user_perspective,
     SDL_Renderer* renderer, SDL_Window* gWindow,
     controls rendering_controls, float cardinal)
{


        	if (cardinal >= (FULL_ROTATION_UTIL*0.875)/ANGLE_ROTATION_FACTOR_UTIL)
            {
		         render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
            else if (cardinal >= (FULL_ROTATION_UTIL*0.75)/ANGLE_ROTATION_FACTOR_UTIL)
            {
                 render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
                 render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
            else if (cardinal >= (FULL_ROTATION_UTIL*0.685)/ANGLE_ROTATION_FACTOR_UTIL)
            {
                 render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
                 render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
            else if (cardinal >= (FULL_ROTATION_UTIL*0.5)/ANGLE_ROTATION_FACTOR_UTIL)
            {
                 render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
                 render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
            else if (cardinal >= (FULL_ROTATION_UTIL*0.375)/ANGLE_ROTATION_FACTOR_UTIL)
            {
                 render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
                 render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
                 else if (cardinal >= (FULL_ROTATION_UTIL*0.25)/ANGLE_ROTATION_FACTOR_UTIL)
            {
                 render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
                 render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
            else if (cardinal >= (FULL_ROTATION_UTIL*0.125)/ANGLE_ROTATION_FACTOR_UTIL)
            {
                 render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
                 render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
            else
            {
		         render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
		         render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
        	     render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
            }
}

