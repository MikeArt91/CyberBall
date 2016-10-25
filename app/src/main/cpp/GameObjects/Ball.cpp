#include "Ball.hpp"

Ball::Ball(float Radius, int slices, glm::vec3 FirstColor, glm::vec3 SecondColor) {
    mRadius = Radius;
    mSlices = slices;
    iMax = mSlices+1;
    mFirstColor = FirstColor;
    mSecondColor = SecondColor;
}

// creates geomety array:
// 3 vertex coords, 2 texture u,v coords, 3 colors + alpha channel
void Ball::GenBallGeom(GLfloat BallGeom[]) {
    int ind = 0;

    // this is quite classical algorytm to create a sphere
    // with a spherical coordinates

    float angleStepI = ((float) M_PI / mSlices);
    float angleStepJ = ((2.0f * (float) M_PI) / mSlices);

    for (int i = 0; i < iMax; i++) {
        for (int j = 0; j < iMax; j++) {
            float sini = glm::sin(angleStepI * i);
            float sinj = glm::sin(angleStepJ * j);
            float cosi = glm::cos(angleStepI * i);
            float cosj = glm::cos(angleStepJ * j);

            // vertex
            // using sperical coordinates
            BallGeom[ind++] = mRadius * sini * sinj;
            BallGeom[ind++] = mRadius * sini * cosj;
            BallGeom[ind++] = mRadius * cosi;

            // u,v
            BallGeom[ind++] = (float) j / (float) mSlices;
            BallGeom[ind++] = (1.0f - i) / (float) mSlices;

            // r,g,b,alpha
            // makes stripes on the ball
            if(j%2 == 0)  {
                BallGeom[ind++] = mFirstColor.x; //R
                BallGeom[ind++] = mFirstColor.y; //G
                BallGeom[ind++] = mFirstColor.z; //B
                BallGeom[ind++] = 1.0f; //ALPHA
            } else {
                BallGeom[ind++] = mSecondColor.x; //R
                BallGeom[ind++] = mSecondColor.y; //G
                BallGeom[ind++] = mSecondColor.z; //B
                BallGeom[ind++] = 1.0f; //ALPHA
            }

        }
    }
}

// array of indices
void Ball::GenBallInd(GLushort BallInd[]) {
    int ind = 0;

    for(int i=0; i< mSlices; i++) {
        for (int j = 0; j < mSlices; j++) {
            int i1 = i+1;
            int j1 = j+1;
            BallInd[ind++] = (GLushort) (i*iMax + j);
            BallInd[ind++] = (GLushort) (i1*iMax + j);
            BallInd[ind++] = (GLushort) (i1*iMax + j1);
            BallInd[ind++] = (GLushort) (i*iMax + j);
            BallInd[ind++] = (GLushort) (i1*iMax + j1);
            BallInd[ind++] = (GLushort) (i*iMax + j1);
        }
    }

}

int Ball::GetGeomSize() {
    int nVertices=iMax*iMax;
    return 9*nVertices* sizeof(GLfloat);
}

int Ball::GetIndSize() {
    return mSlices*mSlices*6;
}