#ifndef CYBERBALL_CUBE_HPP
#define CYBERBALL_CUBE_HPP

#include "Core/Engine.hpp"

/*
    H+-------+G
    /.      /|
 D / .    C/ |
  +-------+..+F
  | /E    | /
  |/      |/
 A+-------+B

*/
#define V_A  -0.5f, -0.5f,  0.5f
#define V_B   0.5f, -0.5f,  0.5f
#define V_C   0.5f,  0.5f,  0.5f
#define V_D  -0.5f,  0.5f,  0.5f
#define V_E  -0.5f, -0.5f, -0.5f
#define V_F   0.5f, -0.5f, -0.5f
#define V_G   0.5f,  0.5f, -0.5f
#define V_H  -0.5f,  0.5f, -0.5f
#define COLOR_1  1.0f, 1.0f, 1.0f, 1.0f
#define COLOR_2  0.8f, 0.8f, 0.8f, 1.0f
#define COLOR_3  0.6f, 0.6f, 0.6f, 1.0f

// max tex coordinate (i.e. texture repeats across cube)
#define TC_R 2.0f


static GLfloat CUBE_GEOM[] = {
        // front face of cube:
        V_A, 0.0f, 0.0f, COLOR_3,
        V_B, TC_R, 0.0f, COLOR_3,
        V_D, 0.0f, TC_R, COLOR_3,
        V_D, 0.0f, TC_R, COLOR_3,
        V_B, TC_R, 0.0f, COLOR_3,
        V_C, TC_R, TC_R, COLOR_3,
        // right face of cube:
        V_B, 0.0f, 0.0f, COLOR_2,
        V_F, TC_R, 0.0f, COLOR_2,
        V_C, 0.0f, TC_R, COLOR_2,
        V_C, 0.0f, TC_R, COLOR_2,
        V_F, TC_R, 0.0f, COLOR_2,
        V_G, TC_R, TC_R, COLOR_2,
        // left face of cube
        V_A, 0.0f, TC_R, COLOR_2,
        V_D, TC_R, TC_R, COLOR_2,
        V_E, 0.0f, 0.0f, COLOR_2,
        V_E, 0.0f, 0.0f, COLOR_2,
        V_D, TC_R, TC_R, COLOR_2,
        V_H, 0.0f, TC_R, COLOR_2,
        // back face of cube
        V_E, TC_R, 0.0f, COLOR_3,
        V_H, TC_R, TC_R, COLOR_3,
        V_F, 0.0f, 0.0f, COLOR_3,
        V_F, 0.0f, 0.0f, COLOR_3,
        V_H, TC_R, TC_R, COLOR_3,
        V_G, 0.0f, TC_R, COLOR_3,
        // bottom of cube
        V_A, 0.0f, TC_R, COLOR_1,
        V_E, 0.0f, 0.0f, COLOR_1,
        V_B, TC_R, TC_R, COLOR_1,
        V_B, TC_R, TC_R, COLOR_1,
        V_E, 0.0f, 0.0f, COLOR_1,
        V_F, TC_R, 0.0f, COLOR_1,
        // top of cube
        V_D, 0.0f, 0.0f, COLOR_1,
        V_C, TC_R, 0.0f, COLOR_1,
        V_H, 0.0f, TC_R, COLOR_1,
        V_H, 0.0f, TC_R, COLOR_1,
        V_C, TC_R, 0.0f, COLOR_1,
        V_G, TC_R, TC_R, COLOR_1,
};

#endif //CYBERBALL_CUBE_HPP
