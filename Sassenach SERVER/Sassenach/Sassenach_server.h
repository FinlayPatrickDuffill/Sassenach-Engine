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

/* Convert four letters into a number */
#define MAKE_NUM(A, B, C, D)    (((A+B)<<8)|(C+D))

/* Defines for the chat client */
#define CHAT_SCROLLBACK 512     /* Save 512 lines in scrollback */
#define CHAT_PROMPT "> "
#define CHAT_PACKETSIZE 256     /* Maximum length of a message */

/* Defines shared between the server and client */
#define CHAT_PORT   1339

/* The protocol between the chat client and server */
#define CHAT_HELLO  0   /* 0+Port+len+name */
#define CHAT_HELLO_PORT     1
#define CHAT_HELLO_NLEN     CHAT_HELLO_PORT+2
#define CHAT_HELLO_NAME     CHAT_HELLO_NLEN+1
#define CHAT_ADD    1   /* 1+N+IP+Port+len+name */
#define CHAT_ADD_SLOT       1
#define CHAT_ADD_HOST       CHAT_ADD_SLOT+1
#define CHAT_ADD_PORT       CHAT_ADD_HOST+4
#define CHAT_ADD_NLEN       CHAT_ADD_PORT+2
#define CHAT_ADD_NAME       CHAT_ADD_NLEN+1
#define CHAT_DEL    2   /* 2+N */
#define CHAT_DEL_SLOT       1
#define CHAT_DEL_LEN        CHAT_DEL_SLOT+1
#define CHAT_BYE    255 /* 255 */
#define CHAT_BYE_LEN        1

/* The maximum number of people who can talk at once */
int CHAT_MAXPEOPLE;
