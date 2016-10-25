#ifndef CYBERBALL_BALL_HPP
#define CYBERBALL_BALL_HPP


#include "Core/Log.hpp"
#include "Core/Engine.hpp"
#include "Rendering/SimpleGeom.hpp"

class Ball {
private:
    float mRadius;
    int mSlices;
    int iMax;
    glm::vec3 mFirstColor;
    glm::vec3 mSecondColor;

public:
    Ball(float Radius, int slices, glm::vec3 FirstColor, glm::vec3 SecondColor);
    void GenBallGeom(GLfloat BallGeom[]);
    void GenBallInd(GLushort BallInd[]);
    int GetGeomSize();
    int GetIndSize();

};

#endif //CYBERBALL_BALL_HPP
