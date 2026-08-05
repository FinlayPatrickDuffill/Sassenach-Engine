#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_test.h>
#include "Textures.h"
#include "Network_client_macros.h"
#include "Generic_Models.h"
#include "Events.h"
#include "Camera.h"
#include "clientside_ally_with_info.h"

#define MOVENORTH 1
#define MOVEEAST 2
#define MOVESOUTH 3
#define MOVEWEST 4

Mix_Chunk* gDealerHigh;
Mix_Chunk* gDealerLow;
bool stepsound1 = false;
bool stepsound2 = false;

bool isNotMultipleOfHalf(double num) {
    // Get the remainder of num divided by 0.5
    double remainder = std::abs(std::fmod(num, 0.5));

    // Define a small tolerance for precision inaccuracies
    const double epsilon = 0.00001;

    // A multiple of 0.5 will have a remainder close to 0.0 or close to 0.5
    bool isMultiple = (remainder < epsilon) || (std::abs(remainder - 0.5) < epsilon);

    return !isMultiple;
}

bool isNotMultipleOfHalfScaled(double num) {
    // Round to the nearest 10th to discard floating point noise
    long long scaled = std::round(num * 10);

    // 0.5 multiplied by 10 becomes 5
    return (scaled % 5) != 0;
}

float animation_buffer[300];
float animation_buffer_terminate[300];
int animation_targets[300];
int animation_instructions[300];
int inks = 0;

int step = 0;

//keyboard -
const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

// world space positions of every object in the chunk why not
glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f),
// Reserved for players_______________//
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3( 0.5f,  0.0f, 0.0f),
        glm::vec3( 1.0f,  0.0f, 0.0f),
        glm::vec3( 1.5f,  0.0f, 0.0f),
        glm::vec3( 2.0f,  0.0f, 0.0f),
        glm::vec3( 2.5f,  0.0f, 0.0f),
        glm::vec3( 3.0f,  0.0f, 0.0f),
//____________________________________//
        glm::vec3( 0.0f,  0.0f, 0.5f),
        glm::vec3( 0.5f,  0.0f, 0.5f),
        glm::vec3( 1.0f,  0.0f, 0.5f),
        glm::vec3( 1.5f,  0.0f, 0.5f),
        glm::vec3( 2.0f,  0.0f, 0.5f),
        glm::vec3( 2.5f,  0.0f, 0.5f),
        glm::vec3( 3.0f,  0.0f, 0.5f),
//____________________________________//
        glm::vec3( 0.0f,  0.0f, 1.0f),
        glm::vec3( 0.5f,  0.0f, 1.0f),
        glm::vec3( 1.0f,  0.0f, 1.0f),
        glm::vec3( 1.5f,  0.0f, 1.0f),
        glm::vec3( 2.0f,  0.0f, 1.0f),
        glm::vec3( 2.5f,  0.0f, 1.0f),
        glm::vec3( 3.0f,  0.0f, 1.0f),
//____________________________________//
        glm::vec3( 0.0f,  0.0f, 1.5f),
        glm::vec3( 0.5f,  0.0f, 1.5f),
        glm::vec3( 1.0f,  0.0f, 1.5f),
        glm::vec3( 1.5f,  0.0f, 1.5f),
        glm::vec3( 2.0f,  0.0f, 1.5f),
        glm::vec3( 2.5f,  0.0f, 1.5f),
        glm::vec3( 3.0f,  0.0f, 1.5f),
//____________________________________//
        glm::vec3( 0.0f,  0.0f, 2.0f),
        glm::vec3( 0.5f,  0.0f, 2.0f),
        glm::vec3( 1.0f,  0.0f, 2.0f),
        glm::vec3( 1.5f,  0.0f, 2.0f),
        glm::vec3( 2.0f,  0.0f, 2.0f),
        glm::vec3( 2.5f,  0.0f, 2.0f),
        glm::vec3( 3.0f,  0.0f, 2.0f),
//____________________________________//
        glm::vec3( 0.0f,  0.0f, 2.5f),
        glm::vec3( 0.5f,  0.0f, 2.5f),
        glm::vec3( 1.0f,  0.0f, 2.5f),
        glm::vec3( 1.5f,  0.0f, 2.5f),
        glm::vec3( 2.0f,  0.0f, 2.5f),
        glm::vec3( 2.5f,  0.0f, 2.5f),
        glm::vec3( 3.0f,  0.0f, 2.5f),
//____________________________________//
        glm::vec3( 0.0f,  0.0f, 3.0f),
        glm::vec3( 0.5f,  0.0f, 3.0f),
        glm::vec3( 1.0f,  0.0f, 3.0f),
        glm::vec3( 1.5f,  0.0f, 3.0f),
        glm::vec3( 2.0f,  0.0f, 3.0f),
        glm::vec3( 2.5f,  0.0f, 3.0f),
        glm::vec3( 3.0f,  0.0f, 3.0f),
//____________________________________//
};

// Array for storing sotragerfd
ObjectArray* animation_buffering = new ObjectArray();

// Array for storing players
ObjectArray* player = new ObjectArray();

#define movedistance 0.5
char primitive_movement_data_encoder(char coord_encode, float movement_encoding) {

      if (movement_encoding > 0)
      {
          for (float yu = 0; yu != movement_encoding; yu = yu + movedistance)
            {
                coord_encode++;
                if (yu > 100)
                {
                    return -999;
                }
                if (yu < -100)
                {
                    return -999;
                }
            }
      }
if (movement_encoding < 0)
      {
          for (float yu = 0; yu != movement_encoding; yu = yu - movedistance)
            {
                coord_encode++;
                if (yu > 100)
                {
                    return 0;
                }
                if (yu < -100)
                {
                    return 0;
                }
            }
      }
     // printf("coord encode %d\n", coord_encode);
return coord_encode;
}

#define code_50_movement_north 50

int number_of_preceding_network_identities = -1;
bool network_carbon_controlling_set = false;
char network_carbon_controlling = 0;
char network_control = 0;
char network_target = -1;

// will this session use a network?
bool network_enabled_game = false;

IPaddress ip;
bool done = false;
IPaddress serverIP;

// Variables to do with sending/receiving data over the network
//__________________________
// char data: Sends labelled data to the network ("TCPRecv")
// to request to initiate player activity

    const char* name;
    const char* server;

    float float_data_from_serverside;

    char hello[1+1+256];
    char finalsend[1+1+256];
    char datareceiver[1+1+256];
    int n;

    Uint16 port_output;

/* This is really easy.  All we do is monitor connections */

/* Global variables */
static TCPsocket tcpsock = NULL;
static UDPsocket udpsock = NULL;
static SDLNet_SocketSet socketset = NULL;
static UDPpacket **packets = NULL;

static struct {
    int active;
    TCPsocket sock;
    IPaddress peer;
    Uint8 name[256+1];
} people[CHAT_MAXCLIENTS];

#define FONT_LINE_HEIGHT    (FONT_CHARACTER_SIZE + 2)

void SendHello(const char *name)
{
    IPaddress *myip;
    char hello[1+1+256];
    int i, n;

    /* No people are active at first */
    for ( i=0; i<CHAT_MAXCLIENTS; ++i ) {
        people[i].active = 0;
    }
    if ( tcpsock != NULL ) {
        /* Get our chat handle */
        if ( (name == NULL) &&
             ((name=getenv("CHAT_USER")) == NULL) &&
             ((name=getenv("USER")) == NULL ) ) {
            name="Unknown";
        }
        printf("Using name '%s'\n", name);

        /* Construct the packet */
        hello[0] = CHAT_HELLO;
        myip = SDLNet_UDP_GetPeerAddress(udpsock, -1);
        memcpy(&hello[CHAT_HELLO_PORT], &myip->port, 2);
        if ( strlen(name) > 255 ) {
            n = 255;
        } else {
            n = strlen(name);
        }
        hello[CHAT_HELLO_NLEN] = n;
        strncpy(&hello[CHAT_HELLO_NAME], name, n);
        hello[CHAT_HELLO_NAME+n++] = 0;

        /* Send it to the server */
        SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
    }
    else
    {
        printf("TCP is empty, can't send hello\n");
    }
}

void SendBuf(char *buf, int len)
{
    int i;

    /* Redraw the prompt and add a newline to the buffer */
    buf[len++] = '\n';

    /* Send the text to each of our active channels */
    for ( i=0; i < CHAT_MAXCLIENTS; ++i ) {
        if ( people[i].active ) {
            if ( len > packets[0]->maxlen ) {
                len = packets[0]->maxlen;
            }
            memcpy(packets[0]->data, buf, len);
            packets[0]->len = len;
            SDLNet_UDP_Send(udpsock, i, packets[0]);
        }
    }
}

#define WALKNORTH 1

int HandleServerData(Uint8 *data)
{
    int used = 0;

    switch (data[0]) {
        case CHAT_ADD: {
            Uint8 which;
            IPaddress newip;

            /* Figure out which channel we got */
            which = data[CHAT_ADD_SLOT];
            if ((which >= CHAT_MAXCLIENTS) || people[which].active) {
                /* Invalid channel?? */
                break;
            }

            newip.host=SDLNet_Read32(&data[CHAT_ADD_HOST]);
            newip.port=SDLNet_Read16(&data[CHAT_ADD_PORT]);

            memcpy(people[which].name, &data[CHAT_ADD_NAME], 256);
            people[which].name[256] = 0;
             people[which].active = 1;

            printf(
            "* New client on %d from %d.%d.%d.%d:%d (%s)\n", which,
            (newip.host>>24)&0xFF, (newip.host>>16)&0xFF,
            (newip.host>>8)&0xFF, newip.host&0xFF,
            newip.port, people[which].name);

            newip.host = SDL_SwapBE32(newip.host);
            newip.port = SDL_SwapBE16(newip.port);

            int n;

            client_IP_number++;

            SDLNet_UDP_Bind(udpsock, which, &newip);

            number_of_preceding_network_identities++;

        }
        used = CHAT_ADD_NAME+data[CHAT_ADD_NLEN];
        break;

        case 20: {

        // printf("Wathe\n");
        // printf("[%d][%d][%d][%d][%d] > %d\n", data[0], data[1], data[2], data[3], data[4], ingame);

        if (!ingame)
        {
            printf(" ingoyme[%d][%d][%d][%d][%d] > %d\n", data[0], data[1], data[2], data[3], data[4], ingame);
            if (data[1] == 89 && data[2] == 90)
            {
                printf("[%d][%d][%d][%d][%d] > %d\n", data[0], data[1], data[2], data[3], data[4], ingame);
                for ( int x = 0; x != data[3]; x++)
                {
                    printf("Trying to create a new player\n");
                    player->add(new Carbon("PLAYER"));
                    ((Carbon*)player->get(x))->perspective_index = x;
                    ((Carbon*)player->get(x))->movement_map_x = 0;
                    ((Carbon*)player->get(x))->movement_map_y = 0;
                    ((Carbon*)player->get(x))->movement_map_z = 0;
                    tot_players++;
                }
                stuck_in_purgatory = false;
            }

        }
        /*
            if (data[1] == 50 && data[2] == 50)
            {
                    printf("Trying to create a new player\n");
                    player->add(new Carbon("PLAYER"));
                    tot_players++;
            }*/

        }
        break;

        case 12: {

            if (data[1] == 9)
            {
               float_data_from_serverside = (data[2]*0.5);
               network_target = data[3];
               network_control = 85;
               ((Carbon*)player->get(network_target))->movement_map_z = data[2];
            }

            if (data[1] == 8)
            {
               float_data_from_serverside = (data[2]*0.5);
               network_target = data[3];
               network_control = 86;
               ((Carbon*)player->get(network_target))->movement_map_x = data[2];
            }

            if (data[1] == 7)
            {
               float_data_from_serverside = (data[2]*0.5);
               network_target = data[3];
               network_control = 87;
               ((Carbon*)player->get(network_target))->movement_map_y = data[2];
            }

        }
        break;
        case 9: {
                // printf("%d%d%d%d\n", data[0], data[1], data[2],
                //       data[3]);

                // 1 of these is passed back to the server -

            // North is [9][9][8]
            if (data[0] == 9 && data[1] == 8 && data[2] == 8)
                       {
                       //    printf("Proceeding with movement");
                           network_control = 50;
                           network_target = data[3];
                       }

            // East is [9][9][11]
            if (data[0] == 9 && data[1] == 8 && data[2] == 10)
                       {
                       //    printf("Proceeding with movement");
                           network_control = 51;
                           network_target = data[3];
                       }

            // South is [9][9][11]
            if (data[0] == 9 && data[1] == 8 && data[2] == 11)
                       {
                       //    printf("Proceeding with movement");
                           network_control = 52;
                           network_target = data[3];
                       }

            // West is [9][9][12]
            if (data[0] == 9 && data[1] == 8 && data[2] == 12)
                       {
                       //    printf("Proceeding with movement");
                           network_control = 53;
                           network_target = data[3];
                       }

            // Down (z) is [9][9][12]
            if (data[0] == 9 && data[1] == 8 && data[2] == 14)
                       {
                       //    printf("Proceeding with movement");
                           network_control = 54;
                           network_target = data[3];
                       }

            // Up (z) is [9][9][12]
            if (data[0] == 9 && data[1] == 8 && data[2] == 13)
                       {
                       //    printf("Proceeding with movement");
                       //    printf("Jump budget %d", jump_budget);
                           network_control = 55;
                           network_target = data[3];
                       }

            used = 50;
            break;
        }
        case 5: {

                // 1 of these is passed back to the server -

            if (data[0] == 5 && data[1] == 8)
                       {
                           if (data[2] == 1)
                           {
                               network_control = 49;
                           }
                           if (data[2] == 2)
                           {
                               network_control = 48;
                           }
                           if (data[2] == 3)
                           {
                               network_control = 47;
                           }
                           if (data[2] == 4)
                           {
                               network_control = 46;
                           }
                           network_target = data[3];
                       }

            used = 50;
            break;
        }

        case CHAT_DEL: {
            Uint8 which;

            /* Figure out which channel we lost */
            which = data[CHAT_DEL_SLOT];
            if ( (which >= CHAT_MAXCLIENTS) ||
                        ! people[which].active ) {
                /* Invalid channel?? */
                break;
            }
            people[which].active = 0;

            /* Let the user know what happened */
            printf(
    "* Lost client on %d (%s)\n", which, people[which].name);

            /* Unbind the address on the UDP socket */
            SDLNet_UDP_Unbind(udpsock, which);
        }
        used = CHAT_DEL_LEN;
        break;
        case CHAT_BYE: {
            printf("* Chat server full\n");
        }
        used = CHAT_BYE_LEN;
        break;
        default: {
            /* Unknown packet type?? */;
        }
        used = 0;
        break;
    }

    return(used);
}

void HandleServer(void)
{
    Uint8 data[512];
    int pos, len;
    int used;

    /* Has the connection been lost with the server? */
    len = SDLNet_TCP_Recv(tcpsock, (char *)data, 512);
    if ( len <= 0 ) {
        SDLNet_TCP_DelSocket(socketset, tcpsock);
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
        printf("Connection with server lost!\n");
    } else {
        pos = 0;
        while ( len > 0 ) {
            used = HandleServerData(&data[pos]);
            pos += used;
            len -= used;
            if ( used == 0 ) {
                /* We might lose data here.. oh well,
                   we got a corrupt packet from server
                 */
                len = 0;
            }
        }
    }
}
void HandleClient(void)
{
    int n;

    n = SDLNet_UDP_RecvV(udpsock, packets);
    while ( n-- > 0 ) {
        if ( packets[n]->channel >= 0 ) {
        }
    }
}

void HandleNet(void)
{
    SDLNet_CheckSockets(socketset, 0);
    if ( SDLNet_SocketReady(tcpsock) ) {
        HandleServer();
    }
    if ( SDLNet_SocketReady(udpsock) ) {
        HandleClient();
    }
}

// hardcoded for the moment
char* determine_server(char* servername ){

    server = "51.7.52.125";
    return servername;

}

// SDL_net_init
bool find_game_server(char *servername, Uint16 port, char* name, bool connecting_success){

    connecting_success = false;

    printf("Starting client...\n");

    server = servername;
    printf("server determined as %s\n",server);

    /* Allocate a vector of packets for client messages */
    packets = SDLNet_AllocPacketV(4, CHAT_PACKETSIZE);
    if ( packets == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't allocate packets: Out of memory\n");
    }

    /* Connect to remote host and create UDP endpoint */
    printf("Connecting to %s:%d ... \n", server,port);
    SDLNet_ResolveHost(&serverIP, server, port);
    if ( serverIP.host == INADDR_NONE ) {
        printf("Couldn't resolve hostname ... \n");
    } else {
        /* If we fail, it's okay, the GUI shows the problem */
        tcpsock = SDLNet_TCP_Open(&serverIP);
        if ( tcpsock == NULL ) {
            printf("Connect failed ... \n");
        } else {
            printf("Connected ... \n");
        }
    }
    printf("Try ports in the range {CHAT_PORT - CHAT_PORT+10}... \n");
    for (int i=0; (udpsock == NULL) && i<port; ++i ) {
        udpsock = SDLNet_UDP_Open(port+i);
    }
    if ( udpsock == NULL ) {
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
        printf("Couldn't create UDP endpoint\n");
    }

    printf("Allocate the socket set for polling the network...\n");
    socketset = SDLNet_AllocSocketSet(2);
    if ( socketset == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create socket set: %s\n",
                     SDLNet_GetError());
    }
    if (!SDLNet_TCP_AddSocket(socketset, tcpsock))
    {
        printf("Couldn't add TCP socket\n");
    }
    SDLNet_UDP_AddSocket(socketset, udpsock);

    IPaddress *myip;
    char hello[1+1+256];
    int i, n;

    /* No people are active at first */
    for ( i=0; i<CHAT_MAXCLIENTS; ++i ) {
        people[i].active = 0;
    }
    if ( tcpsock != NULL ) {
        /* Get our chat handle */
        if ( (name == NULL) &&
             ((name=getenv("CHAT_USER")) == NULL) &&
             ((name=getenv("USER")) == NULL ) ) {
            name="Unknown";
        }
        printf("Using name '%s'\n", name);

        /* Construct the packet */
        hello[0] = CHAT_HELLO;
        myip = SDLNet_UDP_GetPeerAddress(udpsock, -1);
        memcpy(&hello[CHAT_HELLO_PORT], &myip->port, 2);
        if ( strlen(name) > 255 ) {
            n = 255;
        } else {
            n = strlen(name);
        }
        hello[CHAT_HELLO_NLEN] = n;
        strncpy(&hello[CHAT_HELLO_NAME], "", n);
        hello[CHAT_HELLO_NAME+n++] = 0;

        /* Send it to the server */
        SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
        connecting_success = true;
        return connecting_success;
    }
    return connecting_success;
}

void soon_obsolete_hardcoded_network_test_function(gameworld_constants gameworld_constants_logic, SDL_Window* gWindow){
    // Network testing section of the spaghetti
	// printf( "Able to initialize graphics SDL_net...\n" );
    // inform the player you're attempting to connect
    glClear( GL_COLOR_BUFFER_BIT );
    printf("trying to connect to the network\n");
    SDL_GL_SwapWindow( gWindow );
    // find the IP to connect to
    gameworld_constants_logic.connection_status = find_game_server("51.7.52.125", 1338, "Name", gameworld_constants_logic.connection_status);
    // Connecting to a client I guess

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    if (gameworld_constants_logic.connection_status == true)
    {
                dynamic_screenprint_function((SCREEN_WIDTH/2)-((strlen("connection to the specified address successful")*8)/2), SCREEN_HEIGHT/2, gameworld_constants_logic, "connection to the specified address successful", 0);
    }
    else
    {
                dynamic_screenprint_function((SCREEN_WIDTH/2)-((strlen("connection to the specified address unsuccessful")*8)/2), SCREEN_HEIGHT/2, gameworld_constants_logic, "connection to the specified address unsuccessful", 0);
    }
    SDL_GL_SwapWindow( gWindow );
    SDL_Delay(500);

    //Clear color buffer
    SDL_GL_SwapWindow( gWindow );
}

void process_moving_north() {

bool pass = true;

for (int x = 0; x != sizeof(cubePositions)/3; x++)
{
    if (cubePositions[network_target][1] == cubePositions[x][1])
    {
        if (cubePositions[network_target][2] - 0.5f == cubePositions[x][2])
        {
             if (cubePositions[network_target][0]  == cubePositions[x][0])
            {
                 pass = false;
            }
        }
    }
}
if (pass)
{

    if (inks < 300) {
    char coord_decode;
    animation_buffer_terminate[inks] = 0.5f;
    animation_buffer[inks] = 0.f;
    animation_targets[inks] = network_target;
    animation_instructions[inks] = MOVENORTH;

    ((Carbon*)player->get(network_target))->movement_map_x = ((Carbon*)player->get(network_target))->movement_map_x-1;

    char nvalue;
    if ( ((Carbon*)player->get(network_target))->movement_map_x > 0) {
      nvalue = 0;
    } else {
      nvalue = 1;
    }

    printf("((Carbon*)player->get(network_target))->movement_map_x: %d network_target: %d\n",
            ((Carbon*)player->get(network_target))->movement_map_x,
           network_target );

    inks++;
    }
  }
}

void process_moving_east() {

bool pass = true;

for (int x = 0; x != sizeof(cubePositions)/3; x++)
{
    if (cubePositions[network_target][1] == cubePositions[x][1])
    {
        if (cubePositions[network_target][2] == cubePositions[x][2])
        {
             if (cubePositions[network_target][0] - 0.5f  == cubePositions[x][0])
            {
                 pass = false;
            }
        }
    }
}
if (pass)
{
    if (inks < 300) {
    char coord_decode;
    animation_buffer_terminate[inks] = 0.5f;
    animation_buffer[inks] = 0.f;
    animation_targets[inks] = network_target;
    animation_instructions[inks] = MOVEEAST;



    ((Carbon*)player->get(network_target))->movement_map_y = ((Carbon*)player->get(network_target))->movement_map_y-1;

    char nvalue;
    if ( ((Carbon*)player->get(network_target))->movement_map_y > 0) {
      nvalue = 0;
    } else {
      nvalue = 1;
    }

    printf("((Carbon*)player->get(network_target))->movement_map_y: %d network_target: %d\n",
            ((Carbon*)player->get(network_target))->movement_map_y,
           network_target );
    inks++;
    }
  }
}

void process_moving_south() {

bool pass = true;

for (int x = 0; x != sizeof(cubePositions)/3; x++)
{
    if (cubePositions[network_target][1] == cubePositions[x][1])
    {
        if (cubePositions[network_target][2] + 0.5f == cubePositions[x][2])
        {
             if (cubePositions[network_target][0]  == cubePositions[x][0])
            {
                 pass = false;
            }
        }
    }
}
if (pass)
{
         if (inks < 300) {
         char coord_decode;
         animation_buffer_terminate[inks] = 0.5f;
         animation_buffer[inks] = 0.f;
         animation_targets[inks] = network_target;
         animation_instructions[inks] = MOVESOUTH;
         inks++;


    char nvalue;
    if (((Carbon*)player->get(network_target))->movement_map_x > 0) {
      nvalue = 0;
    } else {
      nvalue = 1;
    }

    printf("((Carbon*)player->get(network_target))->movement_map_x: %d network_target: %d\n",
            ((Carbon*)player->get(network_target))->movement_map_x,
           network_target );
    }
  }
}





void process_moving_west() {

bool pass = true;

for (int x = 0; x != sizeof(cubePositions)/3; x++)
{
    if (cubePositions[network_target][1] == cubePositions[x][1])
    {
        if (cubePositions[network_target][2] == cubePositions[x][2])
        {
             if (cubePositions[network_target][0] + 0.5f  == cubePositions[x][0])
            {
                 pass = false;
            }
        }
    }
}
if (pass)
{
         if (inks < 300) {
         char coord_decode;
         animation_buffer_terminate[inks] = 0.5f;
         animation_buffer[inks] = 0.f;
         animation_targets[inks] = network_target;
         animation_instructions[inks] = MOVEWEST;
         inks++;

    ((Carbon*)player->get(network_target))->movement_map_y = ((Carbon*)player->get(network_target))->movement_map_y+1;

    char nvalue;
    if ( ((Carbon*)player->get(network_target))->movement_map_y > 0) {
      nvalue = 0;
    } else {
      nvalue = 1;
    }

    printf("((Carbon*)player->get(network_target))->movement_map_y: %d network_target: %d\n",
            ((Carbon*)player->get(network_target))->movement_map_y,
           network_target );
}
}
}


void process_moving_up() {

bool pass = true;

for (int x = 0; x != sizeof(cubePositions)/3; x++)
{
    if (cubePositions[network_target][1] + 0.5f == cubePositions[x][1])
    {
        if (cubePositions[network_target][2] == cubePositions[x][2])
        {
             if (cubePositions[network_target][0]  == cubePositions[x][0])
            {
                 pass = false;
            }
        }
    }
}

if (pass && jump_budget > 0)
{
         cubePositions[network_target][1] = cubePositions[network_target][1] + 0.5f;
         jump_budget--;

         char coord_decode = primitive_movement_data_encoder(coord_decode, cubePositions[network_target][1]);
         char nvalue;
         if (cubePositions[network_target][1] > 0) {nvalue = 0;} else {nvalue = 1;}

                    network_control = 0;
                    finalsend[0] = 109;
                    finalsend[1] = 9;
                    finalsend[2] = 18;
                    finalsend[3] = network_target;
                    finalsend[4] = coord_decode;
                    finalsend[5] = nvalue;

                    SDLNet_TCP_Send(tcpsock, finalsend, 10);
                //_____________________________
}
}



void process_moving_down() {

    bool pass = true;

for (int x = 0; x != sizeof(cubePositions)/3; x++)
{
    if (cubePositions[network_target][1] - 0.5f == cubePositions[x][1])
    {
        if (cubePositions[network_target][2] == cubePositions[x][2])
        {
             if (cubePositions[network_target][0] == cubePositions[x][0])
            {
                 pass = false;
                 jump_budget = maximum_jump_budget;
            }
        }
    }
}
if (pass)
{
    cubePositions[network_target][1] = cubePositions[network_target][1] - 0.5f;

    char coord_decode = primitive_movement_data_encoder(coord_decode, cubePositions[network_target][1]);
    char nvalue;
    if (cubePositions[network_target][1] > 0) {nvalue = 0;} else {nvalue = 1;}

    network_control = 0;
    finalsend[0] = 109;
    finalsend[1] = 9;
    finalsend[2] = 19;
    finalsend[3] = network_target;
    finalsend[4] = coord_decode;
    finalsend[5] = nvalue;

    SDLNet_TCP_Send(tcpsock, finalsend, 10);
    //_____________________________
}

}

       //__________________________
Uint32 UpdateCardinals(Uint32 Interval, void*) {


       char cardinal;
        if  (user_screen.screen_controls_cardinal_rotation == 1.f)
        {
            cardinal = 1;
        }
        else if (user_screen.screen_controls_cardinal_rotation == 2.f)
        {
            cardinal = 2;
        }
        else if (user_screen.screen_controls_cardinal_rotation == 3.f)
        {
            cardinal = 3;
        }
        else if (user_screen.screen_controls_cardinal_rotation == 4.f)
        {
            cardinal = 4;
        }

        //which player to move
        // char player_index = tcpsock;
        hello[0] = 5;
        hello[1] = 9;
        // [2] contains the cardinal direction
        hello[2] = cardinal;
        hello[3] = network_carbon_controlling;

        //    hello[4] = player_index;
        SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);

        return 102;
}

Uint32 UpdateCoordinates_z(Uint32 Interval, void*) {
                hello[0] = 12;
                hello[1] = 9;
                hello[2] = 9;
                hello[3] = network_carbon_controlling;

               SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
               //_____________________________
        return 3100;
}

Uint32 UpdateCoordinates_x(Uint32 Interval, void*) {
                hello[0] = 12;
                hello[1] = 8;
                hello[2] = 8;
                hello[3] = network_carbon_controlling;
                     SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
               //_____________________________
        return 3150;
 }

 Uint32 UpdateCoordinates_y(Uint32 Interval, void*) {
                hello[0] = 12;
                hello[1] = 7;
                hello[2] = 7;
                hello[3] = network_carbon_controlling;
                SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
               //_____________________________
        return 3200;
}

 Uint32 falling(Uint32 Interval, void*) {
        hello[0] = 9;
        hello[1] = 9;
        hello[2] = 13;
        hello[3] = network_carbon_controlling;
        SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
        //_____________________________
        return 500;
}

void network_switches_preceding_controls() {

if (network_control == 87)
				{
				    // printf("UPDATE %f\n", float_data_from_serverside);
				    ((Carbon*)player->get(network_target))->var[carbon_coord_y] = float_data_from_serverside;
				    cubePositions[network_target][2] = ((Carbon*)player->get(network_target))->var[carbon_coord_y];
				    network_target = -1;
				    network_control = 0;
				}

        if (network_control == 86)
				{
				    // printf("UPDATE %f\n", float_data_from_serverside);
				    ((Carbon*)player->get(network_target))->var[carbon_coord_x] = float_data_from_serverside;
				    cubePositions[network_target][0] = ((Carbon*)player->get(network_target))->var[carbon_coord_x];
				    network_target = -1;
				    network_control = 0;
				}

        if (network_control == 85)
				{
				    // printf("UPDATE %f\n", float_data_from_serverside);
				    ((Carbon*)player->get(network_target))->var[carbon_coord_z] = float_data_from_serverside;
				    cubePositions[network_target][1] = ((Carbon*)player->get(network_target))->var[carbon_coord_z];
				    network_target = -1;
				    network_control = 0;
				}

        if (network_control == 49)
            {
				 ((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] = 1;
				  network_target = -1;
				  network_control = 0;
			}
			if (network_control == 48)
            {
				 ((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] = 2;
				  network_target = -1;
				  network_control = 0;
			}
			if (network_control == 47)
            {
				 ((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] = 3;
				  network_target = -1;
				  network_control = 0;
			}
			if (network_control == 46)
            {
				 ((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] = 4;
				  network_target = -1;
				  network_control = 0;
			}
}

void network_switches_following_controls() {
if (network_control == 50)
				{
// North network control - Moves a player north
				//______________________
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 1)
                    {
                        process_moving_north();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 2)
                    {
                        process_moving_east();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 3)
                    {
                        process_moving_south();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 4)
                    {
                        process_moving_west();
                    }
                    network_control = 0;
		}
		if (network_control == 52)
				{
// East network control - Moves a player east
				//______________________
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 1)
                    {
                        process_moving_west();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 2)
                    {
                        process_moving_north();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 3)
                    {
                        process_moving_east();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 4)
                    {
                        process_moving_south();
                    }
                    network_control = 0;
		}
  // South network control - Moves a player south
		if (network_control == 51)
				{
				//______________________
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 1)
                    {
                        process_moving_south();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 2)
                    {
                        process_moving_west();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 3)
                    {
                        process_moving_north();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 4)
                    {
                        process_moving_east();
                    }
                    network_control = 0;
				}
// South network control - Moves a player west
		if (network_control == 53)
				{
				//______________________
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 1)
                    {
                          process_moving_east();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 2)
                    {
                        process_moving_south();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 3)
                    {
                        process_moving_west();
                    }
                    if (((Carbon*)player->get(network_target))->var[carbon_cardinal_direction] == 4)
                    {
                        process_moving_north();
                    }
                    network_control = 0;
				}
			if (network_control == 54)
				{
					process_moving_up();
					 network_control = 0;
				}
			if (network_control == 55)
				{
					process_moving_down();
					 network_control = 0;
				}
}

void network_dialling() {

keyboardState = SDL_GetKeyboardState(NULL);

	while( SDL_PollEvent( &e ))
		{
			 switch( e.type ) {

				//User requests quit
				//__________________

				case SDL_QUIT:
					game_state_systems = game_state_off;
					break;
				case SDL_KEYDOWN:

				    if (keyboardState[SDL_SCANCODE_5])
                {
                    Mix_PlayChannel( -1, gDealerHigh, 0 );
                    viewport_size_x = viewport_size_x - (SCREEN_WIDTH/2/10);
                    viewport_size_y = viewport_size_y - (SCREEN_HEIGHT/2/10);
				    viewport_magnify_x = viewport_magnify_x + (SCREEN_WIDTH/10);
				    viewport_magnify_y = viewport_magnify_y + (SCREEN_HEIGHT/10);
				//  printf("%d,%d,%d,%d\n", viewport_size_x,
                //  viewport_size_y, viewport_magnify_x, viewport_magnify_y);
                }
                 if (keyboardState[SDL_SCANCODE_6])
                {
                    viewport_size_x = viewport_size_x + (SCREEN_WIDTH/2/10);
                    viewport_size_y = viewport_size_y + (SCREEN_HEIGHT/2/10);
				    viewport_magnify_x = viewport_magnify_x - (SCREEN_WIDTH/10);
				    viewport_magnify_y = viewport_magnify_y - (SCREEN_HEIGHT/10);
				//  printf("%d,%d,%d,%d\n", viewport_size_x,
                //  viewport_size_y, viewport_magnify_x, viewport_magnify_y);
                }
				    if (keyboardState[SDL_SCANCODE_A])
                {
                    if (angle_rot >= -5.f)
                    angle_rot = angle_rot - 5.0f;
                    printf("%f\n", angle);
                }
                if (keyboardState[SDL_SCANCODE_D])
                {
                    if (angle_rot <= 5.f)
                    angle_rot = angle_rot + 5.0f;
                    printf("%f\n", angle);
                }

				if (keyboardState[SDL_SCANCODE_UP])
                {
                    hello[0] = 9;
                    hello[1] = 9;
                    hello[2] = 9;
                    hello[3] = network_carbon_controlling;
                    SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
                   //_____________________________
                }
                if (keyboardState[SDL_SCANCODE_DOWN])
                {
                    hello[0] = 9;
                    hello[1] = 9;
                    hello[2] = 10;
                    hello[3] = network_carbon_controlling;
                    SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
                //_____________________________
                }
                if (keyboardState[SDL_SCANCODE_LEFT])
                {
                hello[0] = 9;
                hello[1] = 9;
                hello[2] = 12;
                hello[3] = network_carbon_controlling;
                SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
                //_____________________________
                }
                if (keyboardState[SDL_SCANCODE_RIGHT])
                {
                hello[0] = 9;
                hello[1] = 9;
                hello[2] = 11;
                hello[3] = network_carbon_controlling;
                SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
                //_____________________________
                }
                if (keyboardState[SDL_SCANCODE_S])
                {
                    hello[0] = 9;
                    hello[1] = 9;
                    hello[2] = 14;
                    hello[3] = network_carbon_controlling;
                    SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
                //_____________________________
                }
                if (keyboardState[SDL_SCANCODE_X])
                {
                    hello[0] = 9;
                    hello[1] = 9;
                    hello[2] = 13;
                    hello[3] = network_carbon_controlling;
                    SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
               //_____________________________
                }
        break;
		}
	}
}

Uint32 Request_to_leave_purgatory(Uint32 Interval, void*) {

        printf("Poll server :(\n");
        hello[0] = 20;
        hello[1] = 90;
        hello[2] = 90;
        hello[3] = network_carbon_controlling;
        SDLNet_TCP_Send(tcpsock, hello, 6);
        if (!ingame)
        {
            return 3010;
        }
        else {
            return 0;
        }
}

void purgatory() {

   SDL_AddTimer(140, Request_to_leave_purgatory, nullptr);
   while (stuck_in_purgatory)
    {
    HandleNet();
    }
}

void play_step() {
     step++;
     if (step >= 3){
            int isStep_sound = rand() % 4 + 1;
     if (isStep_sound == 1)
     {
         stepsound1 = true;
         step = 0;
     }
     else if (isStep_sound > 2 && isStep_sound < 3)
    {
        step = 0;
    }
     else
    {
         stepsound2 = true;
         step = 0;
    }
  }
}



#endif // NETWORK_H_INCLUDED
