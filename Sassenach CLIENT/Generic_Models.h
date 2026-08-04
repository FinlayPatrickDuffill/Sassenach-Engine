#ifndef GENERIC_MODELS_H_INCLUDED
#define GENERIC_MODELS_H_INCLUDED

#define generic_model_index_cube 1
#define generic_model_index_flatsprite 2
#define generic_model_index_UI 3

//__________________________
unsigned int VBO[900], VAO[900], VBO2[100], VAO2[100];

float global_sprite_rot_angle_x = 3.949997f;
float global_sprite_rot_angle_y  = 3.949997f;

float UI_model[] = {
        // UI elements are made out of 2 triangles
        -0.25f, -0.25f, 0.0f,  0.0f, 0.0f,
         0.25f, -0.25f, 0.0f,  1.0f, 0.0f,
         0.25f,  0.25f, 0.0f,  1.0f, 1.0f,
         0.25f,  0.25f, 0.0f,  1.0f, 1.0f,
        -0.25f,  0.25f, 0.0f,  0.0f, 1.0f,
        -0.25f, -0.25f, 0.0f,  0.0f, 0.0f,
    };

    float flat_sprite_model[] = {
        // this is one face of a geometric object
        -0.50f, -0.25f, 0.0f,  0.0f, 0.0f,
         0.50f, -0.25f, 0.0f,  1.0f, 0.0f,
         0.50f,  0.60f, 0.0f,  1.0f, 1.0f,
         0.50f,  0.60f, 0.0f,  1.0f, 1.0f,
        -0.50f,  0.60f, 0.0f,  0.0f, 1.0f,
        -0.50f, -0.25f, 0.0f,  0.0f, 0.0f,
    };

void normalize_flat_sprite_model() {

        flat_sprite_model[0] = (0.25f)*glm::sin(global_sprite_rot_angle_x);
        flat_sprite_model[2] = (0.27f)*glm::cos(global_sprite_rot_angle_y);
        flat_sprite_model[10] = (0.25f)*glm::sin(global_sprite_rot_angle_x);
        flat_sprite_model[12] = (0.27f)*glm::cos(global_sprite_rot_angle_y);
        flat_sprite_model[27] = flat_sprite_model[2];
        flat_sprite_model[25] = flat_sprite_model[0];
        flat_sprite_model[22] = flat_sprite_model[2];
        flat_sprite_model[20] = flat_sprite_model[0];
        flat_sprite_model[5] = (-0.25f)*glm::sin(global_sprite_rot_angle_x);
        flat_sprite_model[7] = (-0.27f)*glm::cos(global_sprite_rot_angle_y);
        flat_sprite_model[15] = flat_sprite_model[5];
        flat_sprite_model[17] = flat_sprite_model[7];
        flat_sprite_model[10] = flat_sprite_model[15];
        flat_sprite_model[12] = flat_sprite_model[17];

    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cube_model[] = {
        // this is all one cube
        -0.25f, -0.25f, -0.25f,  0.0f, 0.0f,
         0.25f, -0.25f, -0.25f,  1.0f, 0.0f,
         0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
         0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
        -0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
        -0.25f, -0.25f, -0.25f,  0.0f, 0.0f,

        -0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
         0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
         0.25f,  0.25f,  0.25f,  1.0f, 1.0f,
         0.25f,  0.25f,  0.25f,  1.0f, 1.0f,
        -0.25f,  0.25f,  0.25f,  0.0f, 1.0f,
        -0.25f, -0.25f,  0.25f,  0.0f, 0.0f,

        -0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
        -0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
        -0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
        -0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
        -0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
        -0.25f,  0.25f,  0.25f,  1.0f, 0.0f,

         0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
         0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
         0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
         0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
         0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
         0.25f,  0.25f,  0.25f,  1.0f, 0.0f,

        -0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
         0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
         0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
         0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
        -0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
        -0.25f, -0.25f, -0.25f,  0.0f, 1.0f,

        -0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
         0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
         0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
         0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
        -0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
        -0.25f,  0.25f, -0.25f,  0.0f, 1.0f
    };

glm::vec3 UI_Positions[] = {
        glm::vec3( 1.0f,  0.0f, 0.5f)
    };

void  initialize_generic_models () {

    glGenVertexArrays(1, &VAO[generic_model_index_cube]);
    glGenBuffers(1, &VBO[generic_model_index_cube]);
    glBindVertexArray(VAO[generic_model_index_cube]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[generic_model_index_cube]);
    // loads a model to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_model), cube_model, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO[generic_model_index_flatsprite]);
    glGenBuffers(1, &VBO[generic_model_index_flatsprite]);
    glBindVertexArray(VAO[generic_model_index_flatsprite]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[generic_model_index_flatsprite]);
    // loads a model to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(flat_sprite_model), flat_sprite_model, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glGenVertexArrays(1, &VAO[generic_model_index_UI]);
    glGenBuffers(1, &VBO[generic_model_index_UI]);
    glBindVertexArray(VAO[generic_model_index_UI]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[generic_model_index_UI]);
    // loads a model to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(UI_model), UI_model, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

#endif // GENERIC_MODELS_H_INCLUDED
