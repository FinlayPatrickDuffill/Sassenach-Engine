#include "LTexture.h"
#include "LMapLoading.h"
#include "LUtil.h"
#include <IL/IL.h>
#include <glm.hpp>
#include <SDL.h>

#define IDDebugTerrainWall 5
#define IDDebugTerrain 2
#define IDTerrainInvert 1
#define blockelevation 25

bool textures_generated;
int terrain_index;
float trig_formula_a, trig_formula_b, trig_formula_c, trig_formula_d ;

// float i stores / used to store the angle of the isometric perspective
//there's only one screen, I probably don't have to change this right
float i, angle_flag;
int elevation, elevation_increment;

typedef struct player{
    int coord_x, coord_y, coord_z;
    ObjectArray* player_flags;
}   player;

//load hardcoded terminology to parse a config file
config_file_terminologies set_default_config_file_terminologies(config_file_terminologies file_terminologies){
    file_terminologies.item_of_terminology[0] = "Player";
    file_terminologies.item_of_terminology[1] = "Coord_x";
    file_terminologies.item_of_terminology[2] = "Coord_y";
    file_terminologies.item_of_terminology[3] = "Coord_z";
    return file_terminologies;
}

LTexture::LTexture()
{
    //Initialize texture ID
    mTextureID = 0;
//    printf("LTexture called Texture identity is: %d \n", mTextureID);
    textures_generated = false;

    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;
}

LTexture::~LTexture()
{
    //Free texture data if needed
    freeTexture();
}

void render_flat_quad(GLuint mTextureID, float Nv1, float Nv2, float Ev1, float Ev2, float Sv1, float Sv2, float Wv1, float Wv2){
        glBindTexture( GL_TEXTURE_2D, mTextureID );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 0.f ); glVertex2f(Nv1, Nv2);
        glTexCoord2f( 1.f, 0.f ); glVertex2f(Ev1, Ev2);
        glTexCoord2f( 1.f, 1.f ); glVertex2f(Sv1, Sv2);
        glTexCoord2f( 0.f, 1.f ); glVertex2f(Wv1, Wv2);
        glEnd();
}
void render_flat_sprite(GLuint mTextureID){
glBindTexture( GL_TEXTURE_2D, mTextureID );
        glBegin( GL_QUADS );
        glTexCoord2f( 1.f, 1.f ); glVertex2f(0.f, 0.f);
        glTexCoord2f( 2.f, 1.f ); glVertex2f(20.f, 0.f);
        glTexCoord2f( 2.f, 2.f ); glVertex2f(20.f, 20.f);
        glTexCoord2f( 1.f, 2.f ); glVertex2f(0.f, 20.f);
        glEnd();
}

bool LTexture::loadTextureFromFile( std::string path, int image_identity )
{
    //Texture loading success
    bool textureLoaded = false;

//    printf("LoadTextureFromFile called -  Texture identity is: %d \n", mTextureID);

    //Generate and set current image ID
    ILuint imgID = image_identity;
    ilGenImages( image_identity, &imgID );
    ilBindImage( imgID );

//    printf("LoadTextureFromFile called -  Image identity is: %d \n", imgID);

    //Load image
    ILboolean success = ilLoadImage( path.c_str() );

    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            //Create texture from file pixels
            textureLoaded = loadTextureFromPixels32( image_identity, (GLuint*)ilGetData(), (GLuint)ilGetInteger( IL_IMAGE_WIDTH ), (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );
    }

    //Report error
    if( !textureLoaded )
    {
//        printf( "Unable to load %s\n", path.c_str() );
    }

    return textureLoaded;
}

bool LTexture::loadTextureFromPixels32( int image_identity, GLuint* pixels, GLuint width, GLuint height )
{

//    printf("Image identity is: %d \n", image_identity);
    //Free texture if it exists

    //Get texture dimensions
    mTextureWidth = width;
    mTextureHeight = height;


//    printf("loadTextureFromPixels32 called - Texture identity is: %d \n", mTextureID);
//    printf("loadTextureFromPixels32 called - Image identity is: %d \n", image_identity);

        glGenTextures( 1, &mTextureID );
        textures_generated = true;

//    printf("loadTextureFromPixels32 called Texture identity is: %d \n", mTextureID);
//    printf("loadTextureFromPixels32 called Image identity is: %d \n", image_identity);

    //Need to create more named textures? X
    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );

//    printf("loadTextureFromPixels32 called - Texture identity is: %d \n", mTextureID);
//    printf("loadTextureFromPixels32 called - Image identity is: %d \n", image_identity);

    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
//        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }

    return true;
}

void LTexture::freeTexture()
{
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
}

int texval;
string terrain_value;

bool skip = false;

GLuint terrain_buffer(ObjectArray *chunk, GLuint mTextureID){

        terrain_value = (((Terrain*)chunk->get(terrain_index))->getName());

        //DEBUG_TERRAIN_WALL
        if (terrain_value == "DEBUG_TERRAIN_WALL")
        {
           mTextureID = IDDebugTerrainWall;
           skip = false;
        }
        //DEBUG_TERRAIN
        if (terrain_value == "DEBUG_TERRAIN")
        {
           mTextureID = IDDebugTerrain;
           skip = false;
        }
        if (terrain_value == "AIR")
        {
           skip = true;
        }
        //DEBUG_TERRAIN_INVERT
        if (terrain_value == "DEBUG_TERRAIN_INVERT")
        {
           mTextureID = IDTerrainInvert;
           skip = false;
        }
        return mTextureID;
}

void LTexture::renderisometricblock_userrelation( int visibletilesheight, int visibletileswidth, trigonometry_rendering trig, ObjectArray *chunk, user_perspective user_perspective)
{

    //Set texture ID
    if (skip == false)
    {
        if (mTextureID == IDDebugTerrainWall)
        {
            trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
            trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
            trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
            trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
            glTranslatef(250 + ((trig_formula_a + trig_formula_c))*0.93, (200  + (trig_formula_b + trig_formula_d)*0.93)-blockelevation*elevation, 0.f );
            render_flat_quad(mTextureID, trig.vertice_Ny, trig.vertice_Nx, trig.vertice_Ey, trig.vertice_Ex, trig.vertice_Sy, trig.vertice_Sx, trig.vertice_Wy, trig.vertice_Wx);
            render_flat_quad(mTextureID, trig.vertice_Ey, trig.vertice_Ex-blockelevation, trig.vertice_Ny, trig.vertice_Nx-blockelevation, trig.vertice_Ny, trig.vertice_Nx, trig.vertice_Ey, trig.vertice_Ex);
            render_flat_quad(mTextureID, trig.vertice_Sy, trig.vertice_Sx, trig.vertice_Sy, trig.vertice_Sx-blockelevation, trig.vertice_Ey, trig.vertice_Ex-blockelevation, trig.vertice_Ey, trig.vertice_Ex);
            render_flat_quad(mTextureID, trig.vertice_Wy, trig.vertice_Wx, trig.vertice_Wy, trig.vertice_Wx-blockelevation, trig.vertice_Ny, trig.vertice_Nx-blockelevation, trig.vertice_Ny, trig.vertice_Nx);
            render_flat_quad(mTextureID, trig.vertice_Wy, trig.vertice_Wx, trig.vertice_Wy, trig.vertice_Wx-blockelevation, trig.vertice_Sy, trig.vertice_Sx-blockelevation, trig.vertice_Sy, trig.vertice_Sx);
            render_flat_quad(mTextureID, trig.vertice_Ny, trig.vertice_Nx-blockelevation, trig.vertice_Ey, trig.vertice_Ex-blockelevation, trig.vertice_Sy, trig.vertice_Sx-blockelevation, trig.vertice_Wy, trig.vertice_Wx-blockelevation);
        }
    }

    mTextureID = 2;

}

void LTexture::renderisometricgrid_userrelation( int visibletilesheight, int visibletileswidth, trigonometry_rendering trig, ObjectArray *chunk, user_perspective user_perspective)
{
    // it's currently chunk_size_eval_increment (1860) / chunk_enum (2) but I'll have to change this eventually

    //Set texture ID
    if (skip == false)
    {
            trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
            trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
            trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
            trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
            glTranslatef(250 + ((trig_formula_a + trig_formula_c))*0.93, (200  + (trig_formula_b + trig_formula_d)*0.93)-blockelevation*elevation, 0.f );
            render_flat_quad(mTextureID, trig.vertice_Ny, trig.vertice_Nx, trig.vertice_Ey, trig.vertice_Ex, trig.vertice_Sy, trig.vertice_Sx, trig.vertice_Wy, trig.vertice_Wx);
    }

}


void LTexture::renderisometricfloor(float Nv1, float Nv2, float Ev1, float Ev2, float Sv1, float Sv2, float Wv1, float Wv2)
{

}

void set_angle(float angle_flag)
{
    if (angle_flag == 1.f)
    {
        i = i+(1/11.25f);
    }
    if (angle_flag == 2.f)
    {
        i = i-(1/11.25f);
    }
}

void LTexture::render_trigonometry_userrelation(ObjectArray *chunk, user_perspective user_perspective, float angle_flag, GLfloat x, GLfloat y, trigonometry_rendering trig )
{
    int square_chunk_formula = chunk->chunk_size_eval_increment/chunk->chunk_enum;
    elevation = user_perspective.Coord_z-1;
    elevation_increment = 0;
    trig = populate_trig(trig);

    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        glTranslatef( x, y, 0.f );

        int visibletilesheight = user_perspective.Coord_y-1;
        int zbreadtheight_last, zbreadtheight_lastbon;

        terrain_index = -1;

        for (int zbreadthheight = visibletilesheight+chunk->peripheral_offset_y; zbreadthheight != chunk->chunk_scope+user_perspective.Coord_y+chunk->peripheral_offset_y; zbreadthheight++)
            {

                visibletilesheight++;
                for (int visibletileswidth = user_perspective.angle_reference_x+chunk->peripheral_offset_x; visibletileswidth != 30.f+user_perspective.angle_reference_x+chunk->peripheral_offset_x; visibletileswidth++)
                {
                    elevation_increment++;
                    if (elevation_increment >= square_chunk_formula+1)
                    {
                        elevation_increment = 0;
                        visibletilesheight = user_perspective.Coord_y;
                        elevation++;
                    }
            glLoadIdentity();
            if (terrain_index != chunk->chunk_size_eval_increment)
            {
                terrain_index++;
            }
            mTextureID = terrain_buffer(chunk, mTextureID);
            if (mTextureID == IDDebugTerrainWall)
            {
                renderisometricblock_userrelation(visibletilesheight, visibletileswidth, trig, chunk, user_perspective);
            }
            else
            {
                renderisometricgrid_userrelation(visibletilesheight, visibletileswidth, trig, chunk, user_perspective);
            }
        }
    }



        // make this its own more general function for rendering different textures on the grid, maybe eventually different models

        for (int visibletilesheight = user_perspective.Coord_y; visibletilesheight != 31.f; visibletilesheight++)
        {
        // glTranslatef( (x - (trig.vertice_Nx / 2)*visibletilesheight), (y + (trig.vertice_Ny / 4)*visibletilesheight), 0.f );
        for (int visibletileswidth = user_perspective.angle_reference_x; visibletileswidth != 31.f; visibletileswidth++)
            {
                glLoadIdentity();
                trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
                trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
                trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
                trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
                glTranslatef((250 + (trig_formula_a + trig_formula_c)*0.00005), (200  + (trig_formula_b + trig_formula_d)*0.000005) , 0.f );

        if (visibletileswidth == user_perspective.unlabelled_user_perspective_data[1])
            {
                if (visibletilesheight == user_perspective.unlabelled_user_perspective_data[2])
                    {
                        render_flat_sprite(mTextureID);
                    }
                }
            }
        }
    }
    // freeTexture();
}

// previous todo: make a function that can render a flat plane with multiple textures at once
// changing the texture in the active buffer is all that I needed to do
void LTexture::render_trigonometry( float angle_flag, GLfloat x, GLfloat y, trigonometry_rendering trig )
{

    trig = populate_trig(trig);

    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        glTranslatef( x, y, 0.f );

/*         int mTextureHeightmagnifyx2 = mTextureHeight * 2;
        int mTextureWidthmagnifyx2 = mTextureWidth * 2;
*/
        for (int visibletilesheight = 0; visibletilesheight != 30; visibletilesheight++)
        {
        glLoadIdentity();
        // glTranslatef( (x - (trig.vertice_Nx / 2)*visibletilesheight), (y + (trig.vertice_Ny / 4)*visibletilesheight), 0.f );

        for (int visibletileswidth = 0; visibletileswidth != 30; visibletileswidth++)
            {
                glLoadIdentity();
                trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
                trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
                trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
                trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
                glTranslatef(250 + (trig_formula_a + trig_formula_c)*0.93, 200  + (trig_formula_b + trig_formula_d)*0.93, 0.f );
                // +(24*visibletileswidth+(trig.vertice_W-trig.vertice_S)*0.523599)
                // printf("x:%f y:%f\n",trig.vertice_W*visibletilesheight*1.75, trig.vertice_S*visibletilesheight);
            }
        }
    }
    // freeTexture();
}

user_perspective initial_user_perspective(user_perspective* user_perspective, float initial_angle, float initial_visible_portion[array_parameters_for_position])
{
    return *user_perspective;
}

trigonometry_rendering populate_trig(trigonometry_rendering trig_you_modify)
{
        trigonometry_rendering trig;

        trig.vertice_Nx = (40); // (mTextureWidth / 2.f);
        trig.vertice_Ny = 40 * 0.75f; // (mTextureHeight * 0.75f);
        trig.vertice_Ex = (40); // mTextureHeight / 2.f;
        trig.vertice_Ey = (40 * 0.75f); // mTextureWidth;
        trig.vertice_Sx = (40); // (mTextureWidth / 2.f);
        trig.vertice_Sy = (40 * 0.75f); // (mTextureHeight * 0.75f);
        trig.vertice_Wx = (40); //  mTextureWidth
        trig.vertice_Wy = (40 * 0.75f); // mTextureHeight / 2.f;

        // (mTextureWidth / 2.f), (mTextureHeight / 4.f)

        trig.angle = trig_you_modify.angle;

            trig.vertice_N = (42 * 0.75f)*glm::sin(i);
            trig.vertice_Ny = ((trig.vertice_Ny+(trig.vertice_N)) * 1.25) / 2;

            trig.vertice_E = -(42 * 0.75f)*glm::cos(i);
            trig.vertice_Ey = ((trig.vertice_Ey+(trig.vertice_E)) * 1.25) / 2;

            trig.vertice_S = -(42 * 0.75f)*glm::sin(i);
            trig.vertice_Sy = ((trig.vertice_Sy+(trig.vertice_S)) * 1.25) / 2;

            trig.vertice_W = (42 * 0.75f)*glm::cos(i);
            trig.vertice_Wy = ((trig.vertice_Wy+(trig.vertice_W)) * 1.25) / 2;

            // printf("| N %f E %f S %f W %f $ANGLE %f\n", trig.vertice_Ny, trig.vertice_E, trig.vertice_Sy, trig.vertice_W, i);

            trig.vertice_N = (42 / 2.f)*glm::cos(i);
            trig.vertice_Nx = (trig.vertice_Nx+(trig.vertice_N)) / 2;

            trig.vertice_E = (42 / 2.f)*glm::sin(i);
            trig.vertice_Ex = (trig.vertice_Ex+(trig.vertice_E)) / 2;

            trig.vertice_S = -(42 / 2.f)*glm::cos(i);
            trig.vertice_Sx = (trig.vertice_Sx+(trig.vertice_S)) / 2;

            trig.vertice_W = -(42 / 2.f)*glm::sin(i);
            trig.vertice_Wx = (trig.vertice_Wx+(trig.vertice_W)) / 2;

        trig.angle = i;
         return trig;
}

GLuint LTexture::getTextureID()
{
    return mTextureID;
}

GLuint LTexture::textureWidth()
{
    return mTextureWidth;
}

GLuint LTexture::textureHeight()
{
    return mTextureHeight;
}

void primitive_screenprint_function(gameworld_constants gameworld_constants_logic, string printing_string){

    // https://www.digitalocean.com/community/tutorials/convert-string-to-char-array-c-plus-plus
    	char arr[printing_string.length() + 1];

	strcpy(arr, printing_string.c_str());
	for (int i = 0; i < printing_string.length(); i++)

    glLoadIdentity();

    for (int x=0; x < printing_string.length(); x++){
            glBindTexture( GL_TEXTURE_2D, (gameworld_constants_logic.termination_of_loaded_alphabet_in_memory-26)+(arr[x]-96) );

            glTranslatef( 8.f, 0.f, 0.f );
            glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(0.f, 0.f);
            glTexCoord2f( 1.f, 0.f ); glVertex2f(8.f, 0.f);
            glTexCoord2f( 1.f, 1.f ); glVertex2f(8.f, 10.f);
            glTexCoord2f( 0.f, 1.f ); glVertex2f(0.f, 10.f);
         glEnd();
    }
}

void reloading_upon_memory_leak(const char* filename,
                                gameworld_constants gameworld_constants_logic,
                                SDL_Window* gWindow, ObjectArray* active_chunk) {
}
