/*

    This file is the CLIENT program entry point -
    Typical program include hierarchy is currently as follows:
    [do this later]

    Acknowledgements:
    Lazy Foo Productions - OpenGL, Loading a Texture - https://lazyfoo.net/tutorials/OpenGL/06_loading_a_texture/index.php
    Sam Lantinga - SDL_net example chat client - https://github.com/SDL-mirror/SDL_net/blob/master/chat.cpp
    Graham Trott - Dynamic arrays in C++ - https://dev.to/gtanyware/dynamic-arrays-in-c-24oj
    Markus DeVries OpenGL Tutorials

*/

#include "main.h"
#include "Utilities.h"
#include "shader_s.h"
#include "stb_image.h"
#include "Network.h"

int nudge = 0;
int findindex = 6;

#define movedistance 0.5
using namespace std;
#define game_state_off 42

bool tacked_south, tacked_north, tacked_east, tacked_west = false;

int main( int argc, char* args[] )
{

    for (int i = 0; i != 301; i++)
    {
        animation_buffer_terminate[i] = -1;
        animation_buffer[i] = -1;
        animation_targets[i] = -1;
        animation_instructions[i] = -1;
        printf("%d -> %d,%d,%d,%d\n", i, animation_instructions[i],
        animation_buffer_terminate[i], animation_buffer[i], animation_targets[i]);
    }



    bool success = true;
	//Initialize SDL
	if(!initialize_graphical_window())
	{
	 	printf( "couldn't do it\n" );
		return 0;
	}

	// Asset loading
    //_______________________________________
    initial_texture_loading();

    // Textures for a pseudo-terminal emulator
    //_______________________________________
    gameworld_constants_logic = load_terminal_alphabet(gameworld_constants_logic);

    //Initialization flag

    int i;

    // ____ Beginning of network initialization block
    /* Initialize the network */
    if ( SDLNet_Init() < 0 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize net: %s\n",
                     SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }

    if ( Mix_Init(MIX_INIT_OGG) < 0 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize mixer: %s\n",
                     SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }

    if( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
    }

    gDealerHigh = NULL;
    gDealerLow = NULL;

    gDealerHigh = Mix_LoadWAV( "Dealer.wav" );
    if( gDealerHigh == NULL )
    {
        printf( "Failed to load bitch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gDealerLow = Mix_LoadWAV( "Dealer2.wav" );
    if( gDealerLow == NULL )
    {
        printf( "Failed to load bitch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }



    /* Allocate a vector of packets for client messages */
    packets = SDLNet_AllocPacketV(4, CHAT_PACKETSIZE);
    if ( packets == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't allocate packets: Out of memory\n");
    }

    cout << "Enter the IP address to connect to:" << endl;
    cin >> ip_address_input;

    cout << "Enter a forwarded port to use:" << endl;
    cin >> port_input;

    int myInt(std::stoi(port_input));
    uint16_t myInt16(0);

    if (myInt <= static_cast<int>(UINT16_MAX) && myInt >=0) {
    myInt16 = static_cast<uint16_t>(myInt);
    }
    else {
        std::cout << "Couldn't convert string properly\n";
    }

    printf("Connecting to %s ... ", server);
    SDLNet_ResolveHost(&serverIP, server, myInt16);

    server = ip_address_input.c_str();

    printf("Connecting to %s ... ", "147.147.126.121");
    SDLNet_ResolveHost(&serverIP, server, 1338);
    if ( serverIP.host == INADDR_NONE ) {
        printf("Couldn't resolve hostname\n");
    } else {
        /* If we fail, it's okay, the GUI shows the problem */
        tcpsock = SDLNet_TCP_Open(&serverIP);
        if ( tcpsock == NULL ) {
            printf("Connect failed\n");
            return 0;
        } else {
            printf("Connected\n");
        }
    }
    /* Try ports in the range {CHAT_PORT - CHAT_PORT+10} */
    for ( i=0; (udpsock == NULL) && i<1338; ++i ) {
        udpsock = SDLNet_UDP_Open(1338+i);
    }
    if ( udpsock == NULL ) {
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
        printf("Couldn't create UDP endpoint\n");
    }

    /* Allocate the socket set for polling the network */
    socketset = SDLNet_AllocSocketSet(2);
    if ( socketset == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create socket set: %s\n",
                     SDLNet_GetError());
    }
    SDLNet_TCP_AddSocket(socketset, tcpsock);
    SDLNet_UDP_AddSocket(socketset, udpsock);

    cout << "What is your identity:" << endl;
    cin >> client_identity;
    name = client_identity.c_str();

    /* Run the GUI, handling network data */
    SendHello(name);
    done = 0;

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.3.coordinate_systems.vs", "6.3.coordinate_systems.fs");
    Shader depth_test("6.3.coordinate_systems.vs", "6.3.coordinate_systems.fs");
    Shader UI_Shader("6.3.coordinate_systems.vs", "6.3.coordinate_systems.fs");

    SDL_GL_SwapWindow( gWindow );
    game_state_systems = game_state_on;

    initialize_generic_models();

    printf("Going to purgatory...\n");
    purgatory();
    printf("Out of purgatory...\n");

    // This shader program uses 2 textures
    //_________________________________________________________________

    unsigned int shaderprogramtexture1, shaderprogramtexture2;

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    //_________________________________________________________________
    ourShader.use();
    ourShader.setInt("shaderprogramtexture1", 0);
    ourShader.setInt("shaderprogramtexture2", 1);

    SDL_AddTimer(1000, UpdateCardinals, nullptr);
    SDL_AddTimer(100, UpdateCoordinates_z, nullptr);
    SDL_AddTimer(120, UpdateCoordinates_y, nullptr);
    SDL_AddTimer(140, UpdateCoordinates_x, nullptr);
    //SDL_AddTimer(1000, falling, nullptr);

    ingame = true;
    printf("Starting game big...\n");

	while( game_state_systems != game_state_off )
		{

        glClear( GL_COLOR_BUFFER_BIT );

        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();

        camera_x_coord = -cubePositions[network_carbon_controlling][2];
        camera_y_coord = -cubePositions[network_carbon_controlling][0];
        camera_z_coord = -cubePositions[network_carbon_controlling][1];

        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(5.0f);
        view       = glm::translate(view, glm::vec3(camera_x_cardinal, camera_y_cardinal, camera_z_cardinal));
        projection = glm::ortho(glm::radians(radians), 13.5f, 2.8f, 12.8f, -40.f, 40.f);
        view       = glm::rotate(view, glm::radians(angle2), glm::vec3(0.25f, 0.0f, 0.0f));
        view       = glm::rotate(view, glm::radians(angle), glm::vec3(0.0f, camera_manip_ortho_y, 0.0f));
        view       = glm::translate(view, glm::vec3(0.f, 0.f, camera_x_coord));
        view       = glm::translate(view, glm::vec3(camera_y_coord, 0.f, 0.f));
        view       = glm::translate(view, glm::vec3(0.f, camera_z_coord, 0.f));

        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        configure_angle();

        //_____________ Send some relevant player variables to the server

        // Attempts to orient players sprites towards the camera
        //_________________________________________________________________

        normalize_flat_sprite_model();
        //_________________________________________________________________

        glBufferData(GL_ARRAY_BUFFER, sizeof(flat_sprite_model), flat_sprite_model, GL_STATIC_DRAW);

    // Update player variables
    network_switches_preceding_controls();
    //Game world viewport
	//___________________
    glViewport( viewport_size_x , viewport_size_y , SCREEN_WIDTH+viewport_magnify_x, SCREEN_HEIGHT+viewport_magnify_y );

        int player_indexes = 0;

        // This loop currently draws all geometries in the game world
        //____________________________________________________________

        for (unsigned int i = 0; i < sizeof(cubePositions)/3; i++)
        {
            if (player_indexes <= tot_players-1)
            {
                if (((Carbon*)player->get(player_indexes))->perspective_index
                    == i)
                {
                    // Textures belonging to the sprite
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, 4);
                    glBindVertexArray(VAO[3]);
                    glm::mat4 model = glm::mat4(50.0f);
                    model = glm::translate(model, cubePositions[i]);
                    ourShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    player_indexes++;
                }
            }
            else
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 2);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, 4);
                glBindVertexArray(VAO[1]);
                glm::mat4 model = glm::mat4(50.0f);
                model = glm::translate(model, cubePositions[i]);
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

            //__________________________________________________________//
            //                                                          //
            //__________________________________________________________//

            UI_Shader.use();
            // projection of the UI elements
            glm::mat4 UI_view          = glm::mat4(0.5f); // make sure to initialize matrix to identity matrix first
            glm::mat4 UI_projection    = glm::mat4(5.0f);
            UI_view       = glm::translate(UI_view, glm::vec3(0.0f, 4.0f, 39.f));
            UI_projection = glm::ortho(glm::radians(radians), 13.5f, 2.8f, 12.8f, -40.f, 40.f);

            //___________________
            // bind textures on corresponding texture units
            //___________________
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cycletexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 4);

            //___________________
            // Set UI viewport
            //___________________
            glViewport(0,-75,SCREEN_WIDTH,SCREEN_HEIGHT);

            glBufferData(GL_ARRAY_BUFFER, sizeof(UI_model), UI_model, GL_STATIC_DRAW);
            glBindVertexArray(VAO[generic_model_index_UI]);
            for (unsigned int i = 1; i < 27; i++)
            {
            // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(50.0f);
                model = glm::translate(model, glm::vec3( i*0.5f,  0.0f, 0.0f));
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            //___________________
            glBindVertexArray(VAO[generic_model_index_UI]);
            for (unsigned int i = 1; i < 27; i++)
            {
                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(50.0f);
                model = glm::translate(model, glm::vec3( i*0.5f,  0.5f, 0.0f));
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            //___________________
            if (model_boolean)
            {
                UI_Shader.setMat4("projection", UI_projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
                UI_Shader.setMat4("view", UI_view);
            }
            UI_Shader.setMat4("UI_projection", UI_projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            UI_Shader.setMat4("UI_view", UI_view);

            global_sprite_rot_angle_x = global_sprite_rot_angle_x  + -angle_rot/58.5;
            global_sprite_rot_angle_y = global_sprite_rot_angle_y  + -angle_rot/58.5;

            // Network is handled first?

            HandleNet();

            // Allocating a concrete network identity to the client
            // Set a player Network ID,
            // having processed the entire list of active clients, this assigns the last network identity on the list+1
            // later this will be replaced with something

            if (!network_carbon_controlling_set)
            {
                    printf("Clients:\n");
                //for (int x = 0; client_IP[x] != ""; x++)
                //{
               //     cout << client_IP[x] << endl;
                //}

                network_carbon_controlling = number_of_preceding_network_identities;
                network_carbon_controlling_set = true;
                printf("THIS NETWORK IDENTITY: %d\n", network_carbon_controlling);

            // Send a notification to the server in case the number of clients exceeds
            // the number of created player data structures
            // char player_index = tcpsock;
                    hello[0] = 100;
                    hello[1] = 99;
                    hello[2] = 99;
                    hello[3] = network_carbon_controlling;
                    SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
            //_____________________________

            }

             if (stepsound1)
             {
                 Mix_PlayChannel( -1, gDealerLow, 0 );
                 stepsound1 = false;
             }
             if (stepsound2)
             {
                 Mix_PlayChannel( -1, gDealerHigh, 0 );
                 stepsound2 = false;
             }

            //____________________________________________
            // Keyboard controls get sent over the network
            //____________________________________________
            network_dialling();
            network_switches_following_controls();
            SDL_GL_SwapWindow( gWindow );

for (int i = 0; i != 300; i++) {
  if (animation_buffer[i] != -1) {

    if (animation_instructions[i] == WALKNORTH) {
      cubePositions[animation_targets[i]][2] = cubePositions[animation_targets[i]][2] - 0.1f;
      animation_buffer[i] = animation_buffer[i] + 0.1f;
      if (animation_buffer[i] >= animation_buffer_terminate[i]) {

        animation_buffer[i] = -1;
        inks--;
        char coord_decode, nvalue;
        if (primitive_movement_data_encoder(coord_decode, cubePositions[animation_targets[i]][2] - 0.5f) != -999) {
                play_step();
        }
      }
    }

    if (animation_instructions[i] == MOVEEAST) {
      cubePositions[animation_targets[i]][0] = cubePositions[animation_targets[i]][0] - 0.1f;
      animation_buffer[i] = animation_buffer[i] + 0.1f;
      if (animation_buffer[i] >= animation_buffer_terminate[i]) {
            char coord_decode = primitive_movement_data_encoder(coord_decode, cubePositions[network_target][0]);animation_buffer[i] = -1;
        inks--;
        if (primitive_movement_data_encoder(coord_decode, cubePositions[animation_targets[i]][2] - 0.5f) != -999) {
                play_step();
        }
      }
    }

    if (animation_instructions[i] == MOVESOUTH) {
      cubePositions[animation_targets[i]][2] = cubePositions[animation_targets[i]][2] + 0.1f;
      animation_buffer[i] = animation_buffer[i] + 0.1f;
      if (animation_buffer[i] >= animation_buffer_terminate[i]) {
                    animation_buffer[i] = -1;
        inks--;
        char coord_decode = primitive_movement_data_encoder(coord_decode, cubePositions[network_target][0]);
        char nvalue;
        if (cubePositions[network_target][0] > 0) {
          nvalue = 0;
        } else {
          nvalue = 1;
        }
        if (primitive_movement_data_encoder(coord_decode, cubePositions[animation_targets[i]][2] - 0.5f) != -999) {
                play_step();
        }
      }
    }

    if (animation_instructions[i] == MOVEWEST) {
      cubePositions[animation_targets[i]][0] = cubePositions[animation_targets[i]][0] + 0.1f;
      animation_buffer[i] = animation_buffer[i] + 0.1f;
      if (animation_buffer[i] >= animation_buffer_terminate[i]) {
                    animation_buffer[i] = -1;
        inks--;
        char coord_decode = primitive_movement_data_encoder(coord_decode, cubePositions[network_target][0]);
        char nvalue;
        if (cubePositions[network_target][0] > 0) {
          nvalue = 0;
        } else {
          nvalue = 1;
        }

        if (primitive_movement_data_encoder(coord_decode, cubePositions[animation_targets[i]][2] - 0.5f) != -999) {
                play_step();
        }
        }
    }
  }
}
    }
	SDL_Quit();
	return 0;
}
