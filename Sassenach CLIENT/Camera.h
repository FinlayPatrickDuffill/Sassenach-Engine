#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

float camera_z_cardinal = 1.0f;
    float camera_x_cardinal = 7.0f;
    float camera_y_cardinal = 7.5f;
    float radians = 1.0f;
    float angle = 45.0f;
    float angle2 = 45.5f;
    float angle_rot = 0.f;
    float angle_rot_orth_x = 0.65f;
    float angle_rot_orth_z = 0.2f;
    bool model_boolean = true;
    float camera_x_coord = 0.f;
    float camera_y_coord = 0.f;
    float camera_z_coord = 0.f;
    float camera_manip_ortho_y = 0.5f;
    float cycletexture = 3;
    int veal = 0;
    GLuint texvalue1 = 3;
    GLuint texvalue2 = 5;
    bool swaptexture = false;

    GLint viewport_size_y = 0;
    GLint viewport_size_x = -75;
    GLsizei viewport_magnify_x = 0;
    GLsizei viewport_magnify_y = 0;

void configure_angle() {

    // pass transformation matrices to the shader
        angle = angle + angle_rot;

        if (angle == 225+35)
        {
            user_screen.screen_controls_cardinal_rotation = 2.f;
            angle_rot = 0.f;
        }
        if (angle == 135+35)
        {
            user_screen.screen_controls_cardinal_rotation = 3.f;
            angle_rot = 0.f;
        }
        if (angle == 45+35)
        {
            angle_rot = 0.f;
            global_sprite_rot_angle_x = 2.733231; /* 3.949997f; */
            global_sprite_rot_angle_y = 2.733231;
            user_screen.screen_controls_cardinal_rotation = 4.f;
        }
        if (angle == 320+35)
        {
            user_screen.screen_controls_cardinal_rotation = 1.f;
            angle_rot = 0.f;
        }
        if (angle >= 360.0f)
        {
            angle = 0.f;
        }
        if (angle <= -1.0f)
        {
            angle = 360.f;
        }

}

#endif // CAMERA_H_INCLUDED
