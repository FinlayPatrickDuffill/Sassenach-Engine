#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"
#include <filesystem>
#include "LMapLoading.h"
#include <SDL.h>
#include <SDL_events.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

int concatenate(int i, int j) {
    int result = 0;

    printf("%d || %d\n",i,j);
    for (int x = i; x <= j; x++) {
       result = result * 10 + x;
    }
    return result;
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

chunk_storage *load_chunk_from_file(const char *name_of_file) {


            cout << "Loading a chunk '(' "<< name_of_file <<" ')..." << endl;
            FILE* fp = std::fopen(name_of_file, "w+");
    if (!fp)
    {
        std::perror("File opening failed");
    }
            string line;
            size_t len, pos;
            int size_eval_increment;
            bool isFound;
            int file_sizing = filesize(name_of_file);

            chunk_storage * chunk_to_store;

            int c; // Note: int, not char, required to handle EOF
            while ((c = std::fgetc(fp)) != EOF) {
                        if (c == '#')
            {
                    size_eval_increment++;
                    printf("Turf: # %d \n", size_eval_increment);
            }
            else
            {
                cout << "A line in the config file contains bad syntax..." << endl;
            }
            };

            chunk_to_store->chunk_size = size_eval_increment * size_eval_increment;

            //chunk_to_store = malloc(sizeof(chunk_storage));

            return chunk_to_store;
};

ObjectArray* describe_chunk(const char *name_of_file, ObjectArray *chunk) {

    int squareroot;
        cout << "Loading a chunk '(' "<< name_of_file <<" ')..." << endl;
        FILE* fp = std::fopen(name_of_file, "r");
        if (!fp)
        {
                std::perror("File opening failed");
        }
        else
        {
            printf("File opened\n");
        }
        string line;
        size_t len, pos = 0;
        int size_eval_increment = 0;
        bool isFound;
        int file_sizing = filesize(name_of_file);
            int c; // Note: int, not char, required to handle EOF
            while ((c = std::fgetc(fp)) != EOF) {
                        if (c == '#')
            {
                //DEBUG_TERRAIN
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN"));
            }
                        if (c == '/')
            {
                    chunk->chunk_enum++;
            }
                        if (c == 'A')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN_INVERT"));
            }
                        if (c == 'O')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN_WALL"));
            }
                        if (c == 'y')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("AIR"));
            }
            };
            squareroot = sqrt(size_eval_increment/chunk->chunk_enum);
            printf("Size eval increment %d\n",size_eval_increment);
            printf("chunk_enum %d\n",chunk->chunk_enum);
            printf("chunk_scope %d\n",chunk->chunk_scope);
            chunk->chunk_size_eval_increment = size_eval_increment;
            chunk->chunk_scope = squareroot * chunk->chunk_enum;
            printf("Chunk size eval increment/2 %d\n",chunk->chunk_size_eval_increment);
            return chunk;

};

ObjectArray* describe_chunk_new(const char *name_of_file, ObjectArray *chunk) {

        int squareroot;
        cout << "Loading a chunk '(' "<< name_of_file <<" ')..." << endl;
        FILE* fp = std::fopen(name_of_file, "r");
        FILE* fp2 = std::fopen(name_of_file, "r");

        if (!fp)
        {
                std::perror("File opening failed");
        }
        else
        {
            printf("File opened\n");
        }

        size_t len, pos = 0;
        bool isFound;
        int file_sizing = filesize(name_of_file);
        int c, c2; // Note: int, not char, required to handle EOF

        int terrain_block = -1;
        std::string line;

        while ((c = std::fgetc(fp)) != EOF) {
                if (c == '{')
            {
                terrain_block++;
                printf("Describing terrain %d\n", terrain_block);
                while ((c = std::fgetc(fp)) != '|') {
                        printf("%c",c);
                            line.append(1, c);
                        }
                        chunk->add(new Terrain("TERRAIN"));
                        line.clear();

                while ((c = std::fgetc(fp)) != '$') {
                        if (c == '}')
                    {
                        printf("End of terrain block\n\n");
                       c = '$';
                    }
                }
            }
        }

        // Flatten the object array here
        chunk->flatten();
        chunk->chunk_size_eval_increment = terrain_block;

        bool coord_var_block = false;
        bool regular_var_block = false;

        int variable_index = 0;
        int variable_value = 0;

        c = 0;
        terrain_block = 0;
        // Place the beginning of the co-ordinates and variables
        int beginning_coord_map = 0;
        int chunk_index = 0;

            fp2 = std::fopen(name_of_file, "r");
        if (!fp2)
        {
                std::perror("File opening failed");
        }
        else
        {
            printf("File opened\n");
        }

        while ((c2 = std::fgetc(fp2)) != EOF)
            {
                if (c2 == '{')
            {
                terrain_block++;
                printf("Describing terrain %d\n", terrain_block);
                while ((c2 = std::fgetc(fp2)) != '$')
                    {

                        // ___________

                        if (c2 == '|')
                        {
                            if (coord_var_block == false)
                            {
                                printf("Doing coord var block\n");
                                regular_var_block = true;
                                coord_var_block = true;
                                while ((c2 = std::fgetc(fp2)) != '|')
                                {
                                    if (c2 != ',')
                                    {
                                        // Fucked concatenation
                                        variable_value = (variable_value*10) + c2-48;
                                        printf("variable value %d:-> %d\n",variable_index, variable_value);
                                    }
                                    if (c2 == ',')
                                    {
                                       ((Terrain*)chunk->get(chunk_index))->var[variable_index] = variable_value*0.5f;
                                       variable_index++;
                                       variable_value = 0;
                                    }
                                }
                                printf("000\n");
                                variable_value = 0;
                                variable_index = 0;
                                printf("Doing regular var block\n");
                                bool is_the_value = false;
                                while ((c2 = std::fgetc(fp2)) != '}')
                                {
                                //_________________________________//
                                    if (c2 == '/')
                                    {
                                        is_the_value = false;
                                    }
                                    else if (c2 == '=')
                                    {
                                        ((Terrain*)chunk->get(chunk_index))->var[variable_index] = variable_value*1.f;
                                        printf("New variable added %d -> %f\n", variable_index,
                                        ((Terrain*)chunk->get(chunk_index))->var[variable_index]);
                                        is_the_value = true;
                                        variable_index = 0;
                                        variable_value = 0;
                                    }
                                    else
                                    {   if (!is_the_value)
                                        {
                                            variable_index = ((variable_index*10) + c2-48)+2;
                                            printf("%d:-> %d\n",variable_index, variable_value);
                                        }
                                        if (is_the_value)
                                        {
                                            // Fucked concatenation
                                            variable_value = (variable_value*10) + c2-48;
                                            printf("The value %d:-> %d\n",variable_index, variable_value);
                                        }
                                    }
                                }

                                variable_index = 0;
                                variable_value = 0;
                            }
                            printf("coords of Terrain %d labelled [%s]: -> X %f Y %f Z %f 4 %f\n",
                            chunk_index, ((Terrain*)chunk->get(0))->getName(),
                            ((Terrain*)chunk->get(chunk_index))->var[0],
                            ((Terrain*)chunk->get(chunk_index))->var[1],
                            ((Terrain*)chunk->get(chunk_index))->var[2],
                            ((Terrain*)chunk->get(chunk_index))->var[3]);
                            }
                        // ___________
                        if (c2 == '}')
                        {
                           printf("Finished that one\n\n");
                           coord_var_block = false;
                           regular_var_block = false;
                           beginning_coord_map = 0;
                           variable_index = 0;
                           chunk_index++;
                           c2 = '$';
                        }
                    }
                }
            }
    return chunk;
}

void describe_peripheral_chunk(const char *name_of_file, ObjectArray *chunk) {

};
