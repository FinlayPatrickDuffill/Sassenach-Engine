#ifndef GEOMETRIES_H_INCLUDED
#define GEOMETRIES_H_INCLUDED

#include <glm.hpp>

// World space positions of every object in the chunk
std::vector<glm::vec3> cubePositions = {
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

#endif // GEOMETRIES_H_INCLUDED
