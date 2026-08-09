/*

    This file is the CLIENT program entry point -
    Typical program include hierarchy is currently as follows:
    [do this later]

    Acknowledgements:
    Lazy Foo Productions - OpenGL, Loading a Texture - https://lazyfoo.net/tutorials/OpenGL/06_loading_a_texture/index.php
    Sam Lantinga - SDL_net example chat client - https://github.com/SDL-mirror/SDL_net/blob/master/chat.cpp
    Graham Trott - Dynamic arrays in C++ - https://dev.to/gtanyware/dynamic-arrays-in-c-24oj

*/

#include <stdio.h>
#include <string>

// This include hierarchy is mandatory
#include "LUtil.h"
#include "LTexture.h"
#include "LEvents.h"
#include "LMapLoading.h"

#include "SDL.h"
#include "SDL_net.h"
#include "Sassenach_server.h"

#define movedistance 0.5

#include <iostream>;
using namespace std;

char primitive_movement_data_encoder(char coord_encode, float movement_encoding) {
      if (movement_encoding > 0)
      {
          for (float yu = 0; yu != movement_encoding; yu = yu + movedistance)
            {
                coord_encode++;
            }
      }
if (movement_encoding < 0)
      {
          for (float yu = 0; yu != movement_encoding; yu = yu - movedistance)
            {
                coord_encode++;
            }
      }
return coord_encode;
}

// Array for storing players
ObjectArray* player = new ObjectArray();

// important for server-side data structures
char max_number_of_clients_engaged = 0;
char network_carbon_controlling = 0;
bool brickpurchasing = false;

void print_datastream_as_characters(char* datastream) {
    for (int i = 0; i != 512; i++)
    {
            printf("%d|", datastream[i]);
    }
    printf("\n");
}

SDL_Event e;
//contexts
SDL_Window* gWindow;
//OpenGL context
SDL_GLContext gContext;
user_perspective debug_user_perspective;
gameworld_constants gameworld_constants_logic;


// Chunk
ObjectArray* active_chunk = new ObjectArray();

static TCPsocket servsock = NULL;
static SDLNet_SocketSet socketset = NULL;

static struct {
    int active;
    TCPsocket sock;
    IPaddress peer;
    Uint8 name[256+1];
} people[100];

void HandleServer(void)
{
    TCPsocket newsock;
    int which;
    unsigned char data;

    newsock = SDLNet_TCP_Accept(servsock);
    if ( newsock == NULL ) {
        return;
    }

    /* Look for unconnected person slot */
    for ( which=0; which<CHAT_MAXPEOPLE; ++which ) {
        if ( ! people[which].sock ) {
            break;
        }
    }
    if ( which == CHAT_MAXPEOPLE ) {
        /* Look for inactive person slot */
        for ( which=0; which<CHAT_MAXPEOPLE; ++which ) {
            if ( people[which].sock && ! people[which].active ) {
                /* Kick them out.. */
                data = CHAT_BYE;
                SDLNet_TCP_Send(people[which].sock, &data, 1);
                SDLNet_TCP_DelSocket(socketset,
                        people[which].sock);
                SDLNet_TCP_Close(people[which].sock);
                SDL_Log("Killed inactive socket %d\n", which);
                break;
            }
        }
    }
    if ( which == CHAT_MAXPEOPLE ) {
        /* No more room... */
        data = CHAT_BYE;
        SDLNet_TCP_Send(newsock, &data, 1);
        SDLNet_TCP_Close(newsock);
#ifdef DEBUG
        SDL_Log("Connection refused -- chat room full\n");
#endif
    } else {
        /* Add socket as an inactive person */
        people[which].sock = newsock;
        people[which].peer = *SDLNet_TCP_GetPeerAddress(newsock);
        SDLNet_TCP_AddSocket(socketset, people[which].sock);
#ifdef DEBUG
        SDL_Log("New inactive socket %d\n", which);
#endif
    }
}

/* Send a "new client" notification */
void SendNew(int about, int to)
{
    char data[512];
    char hardcode[7] = "shards";
    int n;
    printf("New instruction\n");

    n = strlen((char *)people[about].name)+1;
    data[0] = CHAT_ADD;
    data[CHAT_ADD_SLOT] = about;
    memcpy(&data[CHAT_ADD_HOST], &people[about].peer.host, 4);
    memcpy(&data[CHAT_ADD_PORT], &people[about].peer.port, 2);
    data[CHAT_ADD_NLEN] = n;
    memcpy(&data[CHAT_ADD_NAME], people[about].name, n);
    memcpy(&data[CHAT_ADD_NAME], people[about].name, n);
    printf("%c%c%c%c %s\n",data[CHAT_ADD_NAME], data[CHAT_ADD_NAME+1],
            data[CHAT_ADD_NAME+2], data[CHAT_ADD_NAME+3]);

    data[501] =
    SDLNet_TCP_Send(people[to].sock, data, CHAT_ADD_NAME+n);
}

void HandleClient(int which)
{
    char data[512];
    char hardcode[]  = "shards";
    int i;

    /* Has the connection been closed? */
    if ( SDLNet_TCP_Recv(people[which].sock, data, 512) <= 0 ) {
        SDL_Log("Closing socket %d (was%s active)\n",
                which, people[which].active ? "" : " not");
        /* Notify all active clients */
        if ( people[which].active ) {
            people[which].active = 0;
            data[0] = CHAT_DEL;
            data[CHAT_DEL_SLOT] = which;
            for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                if ( people[i].active ) {
                    SDLNet_TCP_Send(people[i].sock,"hello",CHAT_DEL_LEN);
                }
            }
        }
        SDLNet_TCP_DelSocket(socketset, people[which].sock);
        SDLNet_TCP_Close(people[which].sock);
        people[which].sock = NULL;
    } else {
        switch (data[0]) {
            case CHAT_HELLO: {
                /* Yay!  An active connection */
                memcpy(&people[which].peer.port,
                        &data[CHAT_HELLO_PORT], 2);
                memcpy(people[which].name,
                        &data[CHAT_HELLO_NAME], 256);
                people[which].name[256] = 0;
                SDL_Log("Activating socket %d (%s)\n",
                        which, people[which].name);
                printf("New connection from a client with name %s\n",
                        people[which].name);

                if (which > -1)
                {
                    printf("Full client list now looks like:\n");
                    for (int x = 0; x != which+1; x++)
                    {
                        printf("(%d) %s\n", x, people[x].name);
                    }
                }

                /* Notify all active clients */
                for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                    if ( people[i].active ) {
                        SendNew(which, i);
                    }
                }

                /* Notify about all active clients */
                people[which].active = 1;
                for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                    if ( people[i].active ) {
                        SendNew(i, which);
                    }
                }
            }
            case 5: {
                // Command to update a player-controlled Carbon's cardinal rotation
                // Counter-clockwise
                // XXXOP - X control, O target, P instruction
                if (data[0] == 5 && data[1] == 9)
                    {
                        {
                            data[0] = 5;
                            data[1] = 8;
                        }
                    }
                    for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                if ( people[i].active ) {
                    SDLNet_TCP_Send(people[i].sock,data,5);
                    }

                    // Not very descriptive -

                    if (data[2] == 1)
                    {
                        ((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] = 1.f;
                    }
                    if (data[2] == 2)
                    {
                        ((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] = 2.f;
                    }
                    if (data[2] == 3)
                    {
                        ((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] = 3.f;
                    }
                    if (data[2] == 4)
                    {
                        ((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] = 4.f;
                    }

                }
            }
            break;
            case 12: {
            if (data[1] == 9)
            {
                char coord_decode = 0;
                coord_decode = primitive_movement_data_encoder(coord_decode, ((Carbon*)player->get(data[3]))->var[carbon_coord_z]);
                data[2] = coord_decode;
                // printf("Trying to update clients z\n");
                printf("[%d,[%d,[%d,[%d,[%d %d\n", data[0], data[1], data[2],
                data[3], data[4], data[5]);
            }
            if (data[1] == 8)
            {
                char coord_decode = 0;
                coord_decode = primitive_movement_data_encoder(coord_decode, ((Carbon*)player->get(data[3]))->var[carbon_coord_x]);
                data[2] = coord_decode;
            }

            if (data[1] == 7)
            {
                char coord_decode = 0;
                coord_decode = primitive_movement_data_encoder(coord_decode, ((Carbon*)player->get(data[3]))->var[carbon_coord_y]);
                data[2] = coord_decode;
            }

            for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                if ( people[i].active ) {
                    SDLNet_TCP_Send(people[i].sock,data,5);
                    }
                }
            }
break;
            case 100:  {
           // Receiver for a notification to the server in case the number of clients exceeds
           // the number of created player data structures
           // char player_index = tcpsock;
            if (data[1] == 99 && data[2] == 99)
            {
                if (data[3] >= max_number_of_clients_engaged)
                {
                    max_number_of_clients_engaged = data[3];
                    printf("max number of clients engaged %d\n", max_number_of_clients_engaged);
                    player->add(new Carbon("PLAYER"));
                    data[0] = 45;
                    data[1] = 50;
                    data[2] = 50;
                    data[3] = network_carbon_controlling;
                    for ( i=0; i<max_number_of_clients_engaged; ++i ) {
                    if ( people[i].active ) {
                       SDLNet_TCP_Send(people[i].sock,data,6);
                       }
                    }
                    for (int i= 0; i <= max_number_of_clients_engaged; i++)
                    {
                        ((Carbon*)player->get(i))->var[carbon_coord_x] = -0.5;
                        ((Carbon*)player->get(i))->var[carbon_coord_y] = -0.5;
                        ((Carbon*)player->get(i))->var[carbon_coord_z] = 0.5;
                    }
                }
            }
        }
        break;

        case 13: {

            // Someone has requested data about the map

            printf("Someone has requested data about the map\n");

        }
        break;

        case 20: {
        if (data[1] == 90 && data[2] == 90)
        {
            printf("Under arrest shore\n");
            // Polling the server for the max player count to
            // know how many Carbon classes to initialize with
            data[0] = 20;
            data[1] = 89;
            data[2] = 90;
            data[3] = CHAT_MAXPEOPLE;
            printf("%d\n", data[3]);
            for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                if ( people[i].active ) {
                    SDLNet_TCP_Send(people[i].sock,data,5);
                }
            }
        }
    }
break;
            case 9: {

            float newcoord;

            if (data[5] == 1)
            {
                newcoord = -data[4]*0.5;
            }
            else
            {
                newcoord = data[4]*0.5;
            }

                // North is [9][9][9]
                if (data[0] == 9 && data[1] == 9
                    && data[2] == 9)
                {
                    printf("request to move a carbon north\n");
                        data[0] = 9;
                        data[1] = 8;
                        data[2] = 8;

                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 4.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 1.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 2.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 3.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5;
                        }

                }

                // South is [9][9][10]
                if (data[0] == 9 && data[1] == 9
                    && data[2] == 10)
                {
                    printf("request to move a carbon south\n");
                    if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5 < 0.1){
                        data[0] = 9;
                        data[1] = 8;
                        data[2] = 10;

                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 2.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 3.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 4.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 1.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5;
                        }
                    }
                }

                // East is [9][9][11]
                if (data[0] == 9 && data[1] == 9
                    && data[2] == 11)
                {
                    printf("request to move a carbon east\n");
                    if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5 < -0.1) {
                        data[0] = 9;
                        data[1] = 8;
                        data[2] = 11;

                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 3.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 4.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 1.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 2.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5;
                        }
                    }
                }

                // West is [9][9][12]
                if (data[0] == 9 && data[1] == 9
                    && data[2] == 12)
                {
                    if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5 < -0.1) {
                         printf("request to move a carbon west\n");
                         data[0] = 9;
                         data[1] = 8;
                         data[2] = 12;

                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 1.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]+0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 2.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]-0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 3.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_x] = ((Carbon*)player->get(data[3]))->var[carbon_coord_x]-0.5;
                        }
                            if (((Carbon*)player->get(data[3]))->var[carbon_cardinal_direction] == 4.f)
                        {
                            if (((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5 < 0.1)
                            ((Carbon*)player->get(data[3]))->var[carbon_coord_y] = ((Carbon*)player->get(data[3]))->var[carbon_coord_y]+0.5;
                        }
                    }
                }

                // Down (z) is [9][9][13]
                if (data[0] == 9 && data[1] == 9
                    && data[2] == 13)
                {
                    printf("request to move a carbon down\n");
                    data[0] = 9;
                    data[1] = 8;
                    data[2] = 13;
                }

                // Up (z) is [9][9][14]
                if (data[0] == 9 && data[1] == 9
                    && data[2] == 14)
                {
                    printf("request to move a carbon up\n");
                    data[0] = 9;
                    data[1] = 8;
                    data[2] = 14;
                }

                printf("Player x:%f y:%f z:%f\n",((Carbon*)player->get(data[3]))->var[carbon_coord_x],
                       ((Carbon*)player->get(data[3]))->var[carbon_coord_y],((Carbon*)player->get(data[3]))->var[carbon_coord_z]);

                for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
                if ( people[i].active ) {
                    SDLNet_TCP_Send(people[i].sock,data,5);
                    }
                }
            }
            break;
            default: {

            }
            break;
        }
    }
}

static void cleanup(int exitcode)
{
    if ( servsock != NULL ) {
        SDLNet_TCP_Close(servsock);
        servsock = NULL;
    }
    if ( socketset != NULL ) {
        SDLNet_FreeSocketSet(socketset);
        socketset = NULL;
    }
    SDLNet_Quit();
    SDL_Quit();
    exit(exitcode);
}

int main(int argc, char *argv[])
{

    printf("How many people can join the server\n");
    cin >> CHAT_MAXPEOPLE;

    IPaddress serverIP;
    int i;

    (void) argc;
    (void) argv;

    printf("Initializing SDL...");
    /* Initialize SDL */
    if ( SDL_Init(0) < 0 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize SDL: %s\n",
                     SDL_GetError());
        exit(1);
    }

// ___________
    active_chunk ->peripheral_offset_x = 0;
    active_chunk ->peripheral_offset_y = 0;

    active_chunk->chunk_enum = 0;
    active_chunk = describe_chunk_new("new_terrain,0,0,0.txt", active_chunk);

// ___________

    printf("Initializing SDL_net...");

    /* Initialize the network */
    if ( SDLNet_Init() < 0 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize net: %s\n",
                     SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }

    /* Initialize the channels */
    for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
        people[i].active = 0;
        people[i].sock = NULL;
    }

    /* Allocate the socket set */
    socketset = SDLNet_AllocSocketSet(CHAT_MAXPEOPLE+1);
    if ( socketset == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create socket set: %s\n",
                     SDLNet_GetError());
        cleanup(2);
    }

    /* Create the server socket */
    SDLNet_ResolveHost(&serverIP, NULL, 1338);
    SDL_Log("Server IP: %x, %d\n", serverIP.host, serverIP.port);
    servsock = SDLNet_TCP_Open(&serverIP);
    if ( servsock == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create server socket: %s\n",
                     SDLNet_GetError());
        cleanup(2);
    }
    SDLNet_TCP_AddSocket(socketset, servsock);

    /* Loop, waiting for network events */
    for ( ; ; ) {

        /* Wait for events */
        SDLNet_CheckSockets(socketset, ~0);

        /* Check for new connections */
        if ( SDLNet_SocketReady(servsock) ) {
            HandleServer();
        }

        /* Check for events on existing clients */
        for ( i=0; i<CHAT_MAXPEOPLE; ++i ) {
            if ( SDLNet_SocketReady(people[i].sock) ) {
                HandleClient(i);
            }
        }
        brickpurchasing = false;

    }
    cleanup(0);

    /* Not reached, but fixes compiler warnings */
    return 0;
}

