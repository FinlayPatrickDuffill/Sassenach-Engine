#ifndef LEVENTS_H_INCLUDED
#define LEVENTS_H_INCLUDED

#define FRAME_DELAY_MACRO 6
#define MAX_FRAME_DELAY_MACRO 60
#define MIN_FRAME_DELAY_MACRO 5
int ANGLE_ROTATION_FACTOR = 2;
int FULL_ROTATION = 142.f;
// debugging tool to toggle flat terrains

// State of the main menu
int main_menu_state_systems;
int game_state_systems;

#define world_death_zbound 6
#define main_menu_state_begin_game 42
#define main_menu_state_idle 0
//
#define game_state_off 1
#define game_state_on 2

// I don't know why this happens, it's a macro until I can find out how to calculate it
#define chunk_z_offset_a 930

#include <SDL.h>
#include <SDL_events.h>
#include "Network.h"
#include "Textures.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

int frame_delay;
int FOV = 10;

bool gravity_simulation_flag_independent = false;

// callback for gravity to save on arithmetic budget
Uint32 my_callback(Uint32 interval, void *param) {
    //printf("falling :\n", (bool*)param);
    gravity_simulation_flag_independent = true;
    return interval; // Return 0 to stop the timer, or return 'interval' to repeat
}

SDL_TimerCallback gravity_simulation_flag(bool *gravity_on);

// Initializing game structures
// Information about the player's position on the map to render trigonometry
user_perspective debug_user_perspective;
gameworld_constants gameworld_constants_logic;

// trying to store the angle in a different place because I can't refactor
typedef struct screen_perspective{
    float screen_angle, screen_controls_cardinal_rotation;
// if the screen is upside down invert the controls
//    bool screen_controls_inversion;
// no longer does anything
} screen_perspective;

screen_perspective user_screen;

std::string first_numberstring(std::string const & str)
    {
      char const* digits = "0123456789";
      std::size_t const n = str.find_first_of(digits);
      if (n != std::string::npos)
      {
        std::size_t const m = str.find_first_not_of(digits, n);
        return str.substr(n, m != std::string::npos ? m-n : m);
      }
      return std::string();
    }

//open a server file containing data about the position and other attributes of players
void parse_player_data_from_record(const char* name_of_player_data_file, user_perspective user_perspective, config_file_terminologies file_terminologies);

controls control_handling_on_map(ObjectArray* active_chunk, SDL_Event e, const Uint8* keyboardState, controls rendering_controls, SDL_Window* gWindow, SDL_Renderer* renderer);

user_perspective main_rendering_logic_loop(user_perspective,SDL_Event e, const Uint8* keyboardState, controls rendering_controls, SDL_Window* gWindow, SDL_Renderer* renderer);

//moving cardinal direction flags to their own function for ease of comparisons
controls function_moving_up(gameworld_constants constants_to_init, controls rendering_controls){
    if (frame_delay >= MIN_FRAME_DELAY_MACRO)
    {
         rendering_controls.movement_test = 1.f;
    }
    return rendering_controls;
}

controls function_moving_down(gameworld_constants constants_to_init, controls rendering_controls){
    if (frame_delay >= MIN_FRAME_DELAY_MACRO)
    {
         rendering_controls.movement_test = 2.f;
    }
    return rendering_controls;
}

controls function_moving_right(gameworld_constants constants_to_init, controls rendering_controls){
    if (frame_delay >= MIN_FRAME_DELAY_MACRO)
    {
         rendering_controls.movement_test = 3.f;
    }
    return rendering_controls;
}

controls function_moving_left(gameworld_constants constants_to_init, controls rendering_controls){
    if (frame_delay >= MIN_FRAME_DELAY_MACRO)
    {
         rendering_controls.movement_test = 4.f;
    }
    return rendering_controls;
}

bool Terrain_IsSolid(string terrain_value)
{
    if (terrain_value == "DEBUG_TERRAIN_WALL")
    {
        return true;
    }
    if (terrain_value == "DEBUG_TERRAIN_FLOOR")
    {
        return true;
    }
    return false;
}


controls control_handling_on_menu(gameworld_constants constants_to_init, SDL_Event e, const Uint8* keyboardState, controls rendering_controls, SDL_Window* gWindow, SDL_Renderer* renderer) {

while( SDL_PollEvent( &e ))
				{
			//User requests quit
				switch( e.type ) {
				case SDL_QUIT:
					rendering_controls.quitting_game = true;
					break;
				case SDL_KEYDOWN:
				if (keyboardState[SDL_SCANCODE_J]) {
                    main_menu_state_systems = main_menu_state_begin_game;
                    // printf("Preparing game...\n");
                }
				if (keyboardState[SDL_SCANCODE_K]) {
                    main_menu_state_systems = main_menu_state_begin_game;
                    // printf("Preparing game...\n");
                }
                break;
			}
		}
	return rendering_controls;
}

controls check_collision_wall_vertical(ObjectArray* chunk, user_perspective user_perspective,
                               controls rendering_controls, int x_direction,
                                int y_direction, int z_direction)
{
    // The player can move unless there's a wall
    rendering_controls.inertia = false;
    string terrain_value;
    int terrain_check_x, terrain_check_y, terrain_check_z;
    int goingto_x, goingto_y, goingto_z;

        goingto_y = -(user_perspective.Coord_y+y_direction);
        goingto_x = -(user_perspective.angle_reference_x+x_direction-1);
        goingto_z = -user_perspective.Coord_z+z_direction;

    for (int checks = 0; checks != chunk->chunk_size_eval_increment; checks++)
    {
        terrain_check_x = ((Terrain*)chunk->get(checks))->variables[terrain_variable_coord_x];
        terrain_check_y = ((Terrain*)chunk->get(checks))->variables[terrain_variable_coord_y];
        terrain_check_z = ((Terrain*)chunk->get(checks))->variables[terrain_variable_coord_z];
    //Checking if the terrain is adjacent to the player

        if ( goingto_z == terrain_check_z )
//Checking if the terrain is a wall
        {
        if ( goingto_y == terrain_check_y )
       //Checking if the terrain is a wall
        {

    if ( goingto_x == terrain_check_x )
    {
       /* printf("a thing is below xyz %d %d %d \n",
                terrain_check_x, terrain_check_y,
                 terrain_check_z); */
        terrain_value = ((Terrain*)chunk->get(checks))->getName();
        if (Terrain_IsSolid(terrain_value))
        {
              rendering_controls.inertia = true;
              return rendering_controls;
        }
    }

    //Checking if the terrain is a wall

        }
/* if  ( goingto_y == terrain_check_y )
 {
       if  ( goingto_z == terrain_check_z)
       {
            terrain_value = ((Terrain*)chunk->get(checks))->getName();
                   if (terrain_value == "DEBUG_TERRAIN_WALL")
                    {
                        printf("Collision\n");
                        rendering_controls.inertia = true;
                        return rendering_controls;
                }
            }
        }*/
        }
    }

    return rendering_controls;
}

controls check_collision_wall(ObjectArray* chunk, user_perspective user_perspective,
                               controls rendering_controls, int x_direction,
                                int y_direction, int z_direction)
{
    // The player can move unless there's a wall
    rendering_controls.inertia = false;
    string terrain_value;
    int terrain_check_x, terrain_check_y, terrain_check_z;
    int goingto_x, goingto_y, goingto_z;

        goingto_y = -(user_perspective.Coord_y+y_direction-1);
        goingto_x = -(user_perspective.angle_reference_x+x_direction-2);
        goingto_z = -user_perspective.Coord_z;

        // I don't know why the fuck this happens
        if (y_direction == -1) {
            goingto_y--; goingto_y--;
            goingto_x--; goingto_x--;
        }
        if (x_direction == -1) {
            goingto_y--; goingto_y--;
            goingto_x--; goingto_x--;
        }

    printf("Going to xyz %d %d %d\n", goingto_x, goingto_y, goingto_z);


    for (int checks = 0; checks != chunk->chunk_size_eval_increment; checks++)
    {
        terrain_check_x = ((Terrain*)chunk->get(checks))->variables[terrain_variable_coord_x];
        terrain_check_y = ((Terrain*)chunk->get(checks))->variables[terrain_variable_coord_y];
        terrain_check_z = ((Terrain*)chunk->get(checks))->variables[terrain_variable_coord_z];
    //Checking if the terrain is adjacent to the player
        if ( goingto_x == terrain_check_x )
    //Checking if the terrain is a wall
        {
 if  ( goingto_y == terrain_check_y )
 {
       if  ( goingto_z == terrain_check_z)
       {
            terrain_value = ((Terrain*)chunk->get(checks))->getName();
                   if (terrain_value == "DEBUG_TERRAIN_WALL")
                    {
                        add_to_log( "Collision\n ");
                        rendering_controls.inertia = true;
                        return rendering_controls;
                    }
                }
            }
        }
    }
    return rendering_controls;
}


controls control_handling_on_map(ObjectArray* active_chunk,
                                 gameworld_constants constants_to_init,
                                 SDL_Event e, const Uint8* keyboardState,
                                  controls rendering_controls, SDL_Window* gWindow,
                                   SDL_Renderer* renderer)
{
    rendering_controls.movement_test = 5.f;
    if (user_screen.screen_angle <= FULL_ROTATION/ANGLE_ROTATION_FACTOR)
    {
        user_screen.screen_controls_cardinal_rotation = 4.f;
    }
    if (user_screen.screen_angle <= (FULL_ROTATION*0.75)/ANGLE_ROTATION_FACTOR)
    {
        user_screen.screen_controls_cardinal_rotation = 3.f;
    }
    if (user_screen.screen_angle <= (FULL_ROTATION*0.5)/ANGLE_ROTATION_FACTOR)
    {
        user_screen.screen_controls_cardinal_rotation = 2.f;
    }
    if (user_screen.screen_angle <= (FULL_ROTATION*0.25)/ANGLE_ROTATION_FACTOR)
    {
        user_screen.screen_controls_cardinal_rotation = 1.f;
    }
         while( SDL_PollEvent( &e ))
				{
					//User requests quit
					switch( e.type ) {
					case SDL_QUIT:
					    game_state_systems = game_state_off;
						rendering_controls.quitting_game = true;
						break;
					case SDL_KEYDOWN:
                    if (keyboardState[SDL_SCANCODE_D]) {
                        printf("screen angle is %f cardinal rotation is %f\n", user_screen.screen_angle, user_screen.screen_controls_cardinal_rotation);
                    	rendering_controls.angle_direction = 1.f;
                    	render(renderer, gWindow, rendering_controls);
                        user_screen.screen_angle = user_screen.screen_angle + 1.f;
                        // printf("Screen angle: %f\n",user_screen.screen_angle);
                        if   (user_screen.screen_angle >= FULL_ROTATION / ANGLE_ROTATION_FACTOR)
                    	{
                    	    user_screen.screen_angle = 0.f;
                    	}
                    }
                    if (keyboardState[SDL_SCANCODE_Q]) {
                        game_state_systems = game_state_off;
                    	rendering_controls.quitting_game = 1.f;
                    	//printf("Quitting...\n");
                    	dynamic_screenprint_function(150.f, 120.f, gameworld_constants_logic, "quitting", 0);
                    }
                    // Jumping
                    if (keyboardState[SDL_SCANCODE_J]) {
                    	rendering_controls.movement_test = 6.f;
                    	//printf("Hovering...\n");
                    }
                    // the falling control is useful and will soon be brought back
                    if (keyboardState[SDL_SCANCODE_K]) {
                    	rendering_controls.movement_test = 7.f;
                    	//printf("Falling...\n");
                    }
                    // toggle flat terrains
                    if (keyboardState[SDL_SCANCODE_V]) {
                    if (rendering_controls.cycle == false)
                    {
                        if 	(rendering_controls.flat_terrain_toggle == true)
                            {
                                rendering_controls.flat_terrain_toggle = false;
                                rendering_controls.cycle = true;
                            }
                    }
                    if 	(rendering_controls.cycle == false)
                    {
                            if (rendering_controls.flat_terrain_toggle == false)
                            {
                                rendering_controls.flat_terrain_toggle = true;
                                rendering_controls.cycle = true;
                            }
                    }
                         rendering_controls.cycle = false;
                    }
                    // the falling control is useless and will soon be removed
                    //if (keyboardState[SDL_SCANCODE_K]) {
                    	//rendering_controls.movement_test = 7.f;
                    	//printf("Falling...\n");
                    	//dynamic_screenprint_function(150.f, 120.f, gameworld_constants_logic, "falling");
                    //}
                    if (keyboardState[SDL_SCANCODE_A]) {
                        add_to_log( "screen angle is %f cardinal rotation is" );
                    	rendering_controls.angle_direction = 2.f;
                    	render(renderer, gWindow, rendering_controls);
                    	user_screen.screen_angle = user_screen.screen_angle - 1.f;
                    	//printf("Screen angle: %f\n",user_screen.screen_angle);
                        if   (user_screen.screen_angle <= 0.f)
                    	{
                    	    user_screen.screen_angle = FULL_ROTATION / ANGLE_ROTATION_FACTOR;
                    	}
                    }
                    if (keyboardState[SDL_SCANCODE_U]) {
                    	if (frame_delay <= MAX_FRAME_DELAY_MACRO)
                        {
                    	   frame_delay++;
                    	   add_to_log( "Target frame delay increased to %d...\n");
                    	}
                    }
                    if (keyboardState[SDL_SCANCODE_O]) {
                    	if (rendering_controls.FOV <= MAX_FRAME_DELAY_MACRO)
                        {
                    	   rendering_controls.FOV++;
                    	   add_to_log( "FOV increased ...\n" );
                    	}
                    }
                    if (keyboardState[SDL_SCANCODE_P]) {
                    	if (rendering_controls.FOV >= MIN_FRAME_DELAY_MACRO)
                        {
                    	   rendering_controls.FOV--;
                    	   add_to_log( "FOV decreased to %d...\n" );
                    	}
                    }
                    if (keyboardState[SDL_SCANCODE_B]) {
                          rendering_controls.movement_test = 20.f;
                    }
                    if (keyboardState[SDL_SCANCODE_I]) {
                        if (frame_delay >= MIN_FRAME_DELAY_MACRO)
                        {
                           frame_delay--;
                           printf("Target frame delay decreased to %d...\n", frame_delay);
                        }
                    }
                    // Needs heavy refactoring
                    if (keyboardState[SDL_SCANCODE_UP]) {
                    if (user_screen.screen_controls_cardinal_rotation == 2.f)
                    {
                        rendering_controls = function_moving_up(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 1.f)
                    {
                        rendering_controls = function_moving_right(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 4.f)
                    {
                        rendering_controls = function_moving_down(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 3.f)
                    {
                        rendering_controls = function_moving_left(constants_to_init, rendering_controls);
                    }
                    }
                    if (keyboardState[SDL_SCANCODE_DOWN]) {
                    if (user_screen.screen_controls_cardinal_rotation == 4.f)
                    {
                        rendering_controls = function_moving_up(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 3.f)
                    {
                        rendering_controls = function_moving_right(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 2.f)
                    {
                        rendering_controls = function_moving_down(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 1.f)
                    {
                        rendering_controls = function_moving_left(constants_to_init, rendering_controls);
                    }
                    }
                    if (keyboardState[SDL_SCANCODE_RIGHT]) {
                    if (user_screen.screen_controls_cardinal_rotation == 1.f)
                    {
                        rendering_controls = function_moving_up(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 4.f)
                    {
                        rendering_controls = function_moving_right(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 3.f)
                    {
                        rendering_controls = function_moving_down(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 2.f)
                    {
                        rendering_controls = function_moving_left(constants_to_init, rendering_controls);
                    }
                    }
                    if (keyboardState[SDL_SCANCODE_LEFT]) {
                    if (user_screen.screen_controls_cardinal_rotation == 3.f)
                    {
                        rendering_controls = function_moving_up(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 2.f)
                    {
                        rendering_controls = function_moving_right(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 1.f)
                    {
                        rendering_controls = function_moving_down(constants_to_init, rendering_controls);
                    }
                    if (user_screen.screen_controls_cardinal_rotation == 4.f)
                    {
                        rendering_controls = function_moving_left(constants_to_init, rendering_controls);
                    }
                }
                break;
                default:
                break;
			}
		}
    return rendering_controls;
}

user_perspective game_control_response(gameworld_constants gameworld_constants_logic, ObjectArray *chunk, user_perspective user_perspective, controls rendering_controls){

            int formula_a;
            string id_logic;
            if (rendering_controls.movement_test == 1.f)
            {
                rendering_controls = check_collision_wall(chunk, user_perspective,
                rendering_controls, 0, -1, 0);

                if (rendering_controls.inertia == false)
                {
                    user_perspective.unlabelled_user_perspective_data[1] = user_perspective.unlabelled_user_perspective_data[1]-1.f;
                    user_perspective.angle_reference_x = user_perspective.angle_reference_x + 1;
                }
            }
            if (rendering_controls.movement_test == 2.f)
            {
                rendering_controls = check_collision_wall(chunk, user_perspective,
                rendering_controls, 0, 1, 0);

                if (rendering_controls.inertia == false)
                {
                    user_perspective.unlabelled_user_perspective_data[1] = user_perspective.unlabelled_user_perspective_data[1]+1.f;
                    user_perspective.angle_reference_x = user_perspective.angle_reference_x - 1;
                }
            }
            if (rendering_controls.movement_test == 20.f)
            {
            	((Terrain*)chunk->get(1363))->variables[terrain_variable_coord_x]++;
            	((Terrain*)chunk->get(1333))->variables[terrain_variable_coord_x]++;
            	printf("Testing a dynamic terrain... xyz is %d %d %d\n",
                    ((Terrain*)chunk->get(1363))->variables[terrain_variable_coord_x],
                    ((Terrain*)chunk->get(1363))->variables[terrain_variable_coord_y],
                    ((Terrain*)chunk->get(1363))->variables[terrain_variable_coord_z]);
            	rendering_controls.movement_test = 0.f;
			}
            if (rendering_controls.movement_test == 3.f)
            {
                rendering_controls = check_collision_wall(chunk, user_perspective,
                rendering_controls, 1, 0, 0);

                if (rendering_controls.inertia == false)
                {
                     user_perspective.unlabelled_user_perspective_data[2] = user_perspective.unlabelled_user_perspective_data[2]+1.f;
                     user_perspective.Coord_y = user_perspective.Coord_y - 1;
                     printf("y Position: %f\n",user_perspective.unlabelled_user_perspective_data[2]);
                 }
            }
            if (rendering_controls.movement_test == 4.f)
            {
                rendering_controls = check_collision_wall(chunk, user_perspective,
                rendering_controls, -1, 0, 0);

                if (rendering_controls.inertia == false)
                {
                     user_perspective.unlabelled_user_perspective_data[2] = user_perspective.unlabelled_user_perspective_data[2]-1.f;
                     user_perspective.Coord_y = user_perspective.Coord_y + 1;
                     printf("y Position: %f\n",user_perspective.unlabelled_user_perspective_data[2]);
                }
            }
            if (rendering_controls.movement_test == 6.f)
            {
                 formula_a = (((user_perspective.unlabelled_user_perspective_data[2])*gameworld_constants_logic.chunk_row_width)+user_perspective.unlabelled_user_perspective_data[1])-((user_perspective.Coord_z*930)-930);
                 //printf("Formula %d | Chunk scope %d\n",formula_a, chunk->chunk_size_eval_increment);
                 printf("%f, %f, %f",user_perspective.unlabelled_user_perspective_data[1],user_perspective.unlabelled_user_perspective_data[2],user_perspective.unlabelled_user_perspective_data[3]);
                 if (user_perspective.Coord_z >= 4-chunk->chunk_enum) {
                     user_perspective.unlabelled_user_perspective_data[3] = user_perspective.unlabelled_user_perspective_data[3]-3.f;
                     user_perspective.Coord_z = user_perspective.Coord_z-3.f;
                     printf("rise z Position: %f\n",user_perspective.unlabelled_user_perspective_data[2]);
                 }
            }
            if (rendering_controls.movement_test == 7.f)
            {

            }
            if (gravity_simulation_flag_independent)
            {
                 rendering_controls = check_collision_wall_vertical(chunk, user_perspective,
                 rendering_controls, 0, 0, -1);
            if (rendering_controls.inertia == false)
                 {
                      user_perspective.unlabelled_user_perspective_data[3] = user_perspective.unlabelled_user_perspective_data[3]+1.f;
                      user_perspective.Coord_z = user_perspective.Coord_z+1.f;
                 }
               gravity_simulation_flag_independent = false;
            }

            // fall out of the game world return to the menu
            if (user_perspective.Coord_z == world_death_zbound)
            {
                 user_perspective.death_state = true;
            }
/*
            rendering_controls = check_collision_wall_vertical(chunk, user_perspective,
            rendering_controls, 0, 0, -1);
            if (rendering_controls.inertia == false)
            {
                      user_perspective.unlabelled_user_perspective_data[3] = user_perspective.unlabelled_user_perspective_data[3]+1.f;
                      user_perspective.Coord_z = user_perspective.Coord_z+1.f;
                      printf("fall z Position: %f\n",user_perspective.unlabelled_user_perspective_data[2]);
                      printf("The description of terrain beneath -> %f,%f,0 is %s,\n",user_perspective.unlabelled_user_perspective_data[1] , user_perspective.unlabelled_user_perspective_data[2] , ((Terrain*)chunk->get(((user_perspective.unlabelled_user_perspective_data[2])*gameworld_constants_logic.chunk_row_width)+user_perspective.unlabelled_user_perspective_data[1]))->getName());
            }
            */
            /* old gravity
            formula_a = (((user_perspective.unlabelled_user_perspective_data[2])*gameworld_constants_logic.chunk_row_width)+user_perspective.unlabelled_user_perspective_data[1])-((user_perspective.Coord_z*930)-930);
            id_logic = ((Terrain*)chunk->get(formula_a))->getName();
            // printf("gravity %d\n", formula_a);
            if (id_logic != "DEBUG_TERRAIN_WALL")
            {
                formula_a = (((user_perspective.unlabelled_user_perspective_data[2])*gameworld_constants_logic.chunk_row_width)+user_perspective.unlabelled_user_perspective_data[1])-((user_perspective.Coord_z*930)-930);
                id_logic = ((Terrain*)chunk->get(formula_a))->getName();
                if (id_logic == "AIR")
                {
                      user_perspective.unlabelled_user_perspective_data[3] = user_perspective.unlabelled_user_perspective_data[3]+1.f;
                      user_perspective.Coord_z = user_perspective.Coord_z+1.f;
                }
             }
             formula_a = (((user_perspective.unlabelled_user_perspective_data[2])*gameworld_constants_logic.chunk_row_width)+user_perspective.unlabelled_user_perspective_data[1]-((user_perspective.Coord_z*930)-930));
                id_logic = ((Terrain*)chunk->get(formula_a))->getName();
                if (id_logic == "DEBUG_TERRAIN_WALL")
                {
                      user_perspective.unlabelled_user_perspective_data[3] = user_perspective.unlabelled_user_perspective_data[3]-1.f;
                      user_perspective.Coord_z = user_perspective.Coord_z-1.f;
                }
                */
           return user_perspective;
}

user_perspective main_rendering_logic_loop(gameworld_constants gameworld_constants_logic, ObjectArray *chunk, user_perspective user_perspective, SDL_Event e, const Uint8* keyboardState, controls rendering_controls, SDL_Window* gWindow, SDL_Renderer* renderer)
{

            // Object detection is perhaps too general of a descriptor
            // Respond to game controls
            user_perspective = game_control_response(gameworld_constants_logic, chunk, user_perspective, rendering_controls);

			// render_userrelation_N(chunk, user_perspective, renderer, gWindow, rendering_controls);
			return user_perspective;
}

void game_logic(){


}

user_perspective parse_label_from_record(user_perspective user_perspective, config_file_terminologies file_terminologies, std::ifstream &myfile){
            string line;
            size_t len;
            size_t pos;
            bool isFound;
//            printf("Size of const char array in file terminologies struct: %d ...\n",sizeof(file_terminologies.item_of_terminology));

            int stof_buffer = 0;
            int i = 1;
            string player_datum_end;
            string llin;

            //remove "EOB" here and change it to the terminology to indicate the end of a user block
            player_datum_end = "EOB";
            while (getline(myfile, line)) {
            isFound = line.find(player_datum_end) != string::npos;
                        if (isFound)
            {
                len = player_datum_end.length();
                pos = 0;
                line.erase(pos,len);
                cout << "Found end of block" << line << endl;
                //Check data is stored
                for (int i= 1; i!=5; i++)
                {
//                     printf("Stored data :-> %f \n", user_perspective.unlabelled_user_perspective_data[i]);
                }
                return user_perspective;
            }
            else
            {
                    cout << "Data reads -> " << line << endl;
//                    // printf("\nTerminology %d : %s", i, file_terminologies.item_of_terminology[i]);
                    llin = first_numberstring(line);
                    cout << "Value -> " << llin << endl;
                    stof_buffer = std::stof(llin);
                    std:: cout<< "stof Value ->" << stof_buffer << std::endl;
 //                   printf("stof Value -> %d \n", stof_buffer);
                    user_perspective.unlabelled_user_perspective_data[i] = stof_buffer;
                    i++;
                    // user_perspective.unlabelled_user_perspective_data[i] = file_terminologies.item_of_terminology[i];
            }
       }
       return user_perspective;
}

void parse_player_data_from_record(const char* name_of_player_data_file, user_perspective user_perspective, config_file_terminologies file_terminologies)
{
            cout << "Running a hardcoded server-side config file parser '('parse_player_data_from_record')..." << endl;
            ifstream myfile(name_of_player_data_file);
                if (!myfile.is_open()) {
            // print error message and return
            cerr << "Failed to open file: " << name_of_player_data_file << endl;
            }
            string line;
            size_t len;
            size_t pos;
            bool isFound;

            //set default terms to search for in the config file
            file_terminologies = set_default_config_file_terminologies(file_terminologies);

 /*           //list all of the terms that will be searched for in the config file
            cout << "Terminology:" << endl;
            for (int listing = 0; listing!= number_of_config_file_terminologies; listing++)
            {
 //                 printf("%s\n",file_terminologies.item_of_terminology[listing]);
            } */

            string player_datum_begin = "Player";
            while (getline(myfile, line)) {
            isFound = line.find(player_datum_begin) != string::npos;
                        if (isFound)
            {
                len = player_datum_begin.length();
                pos = 0;
                line.erase(pos,len);
                cout << "Label found for a player" << line << endl;
                user_perspective = parse_label_from_record(user_perspective, file_terminologies, myfile);
            }
            else
            {
                cout << "A line in the config file contains bad syntax..." << endl;
//                printf("User perspective data: %f %f \n", user_perspective.unlabelled_user_perspective_data[1], user_perspective.unlabelled_user_perspective_data[2]);
            }
            };

}

// redundant maybe
user_perspective parse_player_data_from_record_particular(const char* name_of_player_data_file, const char* name_of_player_to_find, user_perspective user_perspective, config_file_terminologies file_terminologies)
{

            cout << "Running a hardcoded server-side config file parser '('parse_player_data_from_record')..." << endl;
            ifstream myfile(name_of_player_data_file);
                if (!myfile.is_open()) {
            // print error message and return
            cerr << "Failed to open file: " << name_of_player_data_file << endl;
            }
            string line;
            size_t len;
            size_t pos;
            bool isFound;

            //set default terms to search for in the config file
            file_terminologies = set_default_config_file_terminologies(file_terminologies);

/*            //list all of the terms that will be searched for in the config file
            cout << "Terminology:" << endl;
            for (int listing = 0; listing!= number_of_config_file_terminologies; listing++)
            {
//                  printf("%s\n",file_terminologies.item_of_terminology[listing]);
            }
*/

            string player_datum_begin = "Player";
            while (getline(myfile, line)) {
            isFound = line.find(player_datum_begin) != string::npos;
                        if (isFound)
            {
                isFound = line.find(name_of_player_to_find) != string::npos;
                if (isFound)
                {
                    len = player_datum_begin.length();
                    pos = 0;
                    line.erase(pos,len);
                    cout << "Label found for a player" << line << endl;
                    user_perspective = parse_label_from_record(user_perspective, file_terminologies, myfile);
                }
            }
            else
            {
                cout << "A line in the config file contains bad syntax..." << endl;
//              printf("User perspective data: %f %f \n", user_perspective.unlabelled_user_perspective_data[1], user_perspective.unlabelled_user_perspective_data[2]);
            }
            };
            return user_perspective;

}

#endif // LEVENTS_H_INCLUDED
