#ifndef LUTIL_H
#define LUTIL_H

#define CLOSE_PROGRAM_MACRO_ONE 1
#define CLOSE_PROGRAM_MACRO_TWO 2

#include "Textures.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_events.h>

typedef struct {
    int cache_flag_int, cache_data_int;
    bool cache_flag_bool, cache_data_bool;
    char cache_flag_char, cache_data_char;
} debug_cache_flags;

//Debugging cache for debugging
debug_cache_flags write_to_debug_cache(debug_cache_flags* debug_cache_flag_bunch);

bool loadMedia(int image_identity);

bool loadMedia_particular(int image_identity, const char* filename);

void update();

void render(SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls );

void render_userrelation(ObjectArray *chunk, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls );

void render_userrelation_N(ObjectArray *chunk, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls);
/*
I don't know why I'm creating a separate function
*/

void initial_texture_loading();

gameworld_constants load_terminal_alphabet(gameworld_constants gameworld_constants_logic);

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
     controls rendering_controls, float cardinal);

#endif
