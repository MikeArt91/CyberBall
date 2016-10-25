#ifndef CYBERBALL_TUNNEL_HPP
#define CYBERBALL_TUNNEL_HPP

#include "Core/Engine.hpp"
#include "GameConsts.hpp"

// (first char is the coord, second is 'B'ottom, 'R'ight, 'L'eft or 'T'op,
// 'N'ear or 'F'ar)
// and third is whether this accounts for the 'S'pacing or 'N'ot.

#define ZB -TUNNEL_HALF_H
#define ZT TUNNEL_HALF_H
#define XL -TUNNEL_HALF_W
#define XR TUNNEL_HALF_W
#define YN (TUNNEL_SECTION_LENGTH * -0.5f)
#define YF (TUNNEL_SECTION_LENGTH * 0.5f)

#define TCOLOR_M 1.0f,1.0f,1.0f,1.0f
#define TCOLOR_D 0.2f,0.2f,0.2f,1.0f

// max texture coord along X and Z axis (i.e. repetitions)
#define REPN 10.0f

// max texture coord along Y axis (i.e. repetitions)
#define REPF (REPN * TUNNEL_SECTION_LENGTH / (2 * TUNNEL_HALF_H))

// note: the color data is temporary (will be replaced with texture once texture is implemented)
static GLfloat TUNNEL_GEOM[] = {
        /* vert  0: x,y,z:*/ XL, YN, ZB,  /*tex u,v*/ 0.0f, 0.0f, /*color:*/ TCOLOR_M,
        /* vert  1: x,y,z:*/ XR, YN, ZB,  /*tex u,v*/ REPN, 0.0f, /*color:*/ TCOLOR_M,
        /* vert  2: x,y,z:*/ XR, YF, ZB,  /*tex u,v*/ REPN, REPF, /*color:*/ TCOLOR_M,
        /* vert  3: x,y,z:*/ XL, YF, ZB,  /*tex u,v*/ 0.0f, REPF, /*color:*/ TCOLOR_M,
        /* vert  4: x,y,z:*/ XL, YN, ZB,  /*tex u,v*/ 0.0f, 0.0f, /*color:*/ TCOLOR_D,
        /* vert  5: x,y,z:*/ XL, YF, ZB,  /*tex u,v*/ REPF, 0.0f, /*color:*/ TCOLOR_D,
        /* vert  6: x,y,z:*/ XL, YF, ZT,  /*tex u,v*/ REPF, REPN, /*color:*/ TCOLOR_D,
        /* vert  7: x,y,z:*/ XL, YN, ZT,  /*tex u,v*/ 0.0f, REPN, /*color:*/ TCOLOR_D,
        /* vert  8: x,y,z:*/ XR, YN, ZB,  /*tex u,v*/ REPF, 0.0f, /*color:*/ TCOLOR_D,
        /* vert  9: x,y,z:*/ XR, YN, ZT,  /*tex u,v*/ REPF, REPN, /*color:*/ TCOLOR_D,
        /* vert 10: x,y,z:*/ XR, YF, ZT,  /*tex u,v*/ 0.0f, REPN, /*color:*/ TCOLOR_D,
        /* vert 11: x,y,z:*/ XR, YF, ZB,  /*tex u,v*/ 0.0f, 0.0f, /*color:*/ TCOLOR_D,
        /* vert 12: x,y,z:*/ XL, YF, ZT,  /*tex u,v*/ 0.0f, 0.0f, /*color:*/ TCOLOR_M,
        /* vert 13: x,y,z:*/ XR, YF, ZT,  /*tex u,v*/ REPN, 0.0f, /*color:*/ TCOLOR_M,
        /* vert 14: x,y,z:*/ XR, YN, ZT,  /*tex u,v*/ REPN, REPF, /*color:*/ TCOLOR_M,
        /* vert 15: x,y,z:*/ XL, YN, ZT,  /*tex u,v*/ 0.0f, REPF, /*color:*/ TCOLOR_M,
};

static GLushort TUNNEL_GEOM_INDICES[] = {
        0,  1,  2,  0,  2,  3, // floor
        // 8,  9, 11, 11,  9, 10, // right wall CURRENTLY DISABLED
        4,  5,  7,  7,  5,  6, // left wall
        12, 13, 15, 15, 13, 14  // ceiling
};

#define GEOM_STRIDE (9 * sizeof(GLfloat))
#define GEOM_TEXCOORD_OFFSET (3 * sizeof(GLfloat))
#define GEOM_COLOR_OFFSET (5 * sizeof(GLfloat))

#endif //CYBERBALL_TUNNEL_HPP
