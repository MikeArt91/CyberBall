#ifndef CYBERBALL_OBSTACLE_HPP
#define CYBERBALL_OBSTACLE_HPP

#include <Data/GameConsts.hpp>
#include "Core/Engine.hpp"
#include "Core/Util.hpp"


// An obstacle consists of a grid of OBS_GRID_SIZE x OBS_GRID_SIZE cells; each of them may
// or may not contain a box.
//
// The obstacle grid lies on the XZ plane.
class Obstacle {
public:
    bool grid[OBS_GRID_SIZE][OBS_GRID_SIZE]; // indexed as [col][row]
    int style;  // obstacle style (currently, this specifies its color).
    int cut;
    const static int STYLE_NULL = 0;  // a null obstacle (not displayed)

    glm::vec3 GetBoxCenter(int gridCol, int gridRow, float posY) {
        return glm::vec3(-TUNNEL_HALF_W + (gridCol + 0.5f) * OBS_CELL_SIZE, posY,
                         -TUNNEL_HALF_H + (gridRow + 0.5f) * OBS_CELL_SIZE);

    }

    glm::vec3 GetBoxSize(int gridCol, int gridRow) {
        return glm::vec3(OBS_BOX_SIZE, OBS_BOX_SIZE, OBS_BOX_SIZE);
    }

    int GetRowAt(float z) {
        return Clamp((int)floor((z + TUNNEL_HALF_H) / OBS_CELL_SIZE), 0, OBS_GRID_SIZE - 1);
    }

    int GetColAt(float x) {
        return Clamp((int)floor((x + TUNNEL_HALF_W) / OBS_CELL_SIZE), 0, OBS_GRID_SIZE - 1);
    }

    float GetMinCutY() {
        if (style != STYLE_NULL) {
            return (-TUNNEL_HALF_H + cut * OBS_CELL_SIZE + ((1.0f-BOX_CELL_RATIO) / 2));
        } else {return -TUNNEL_HALF_H-OBS_CELL_SIZE;}
    };
    float GetMaxCutY() {
        if (style != STYLE_NULL) {
            return (-TUNNEL_HALF_H + (cut + CUT_CELLS) * OBS_CELL_SIZE - ((1.0f - BOX_CELL_RATIO) / 2));
        } else { return +TUNNEL_HALF_H + OBS_CELL_SIZE; }
    };

    void Reset() {
        style = STYLE_NULL;
        cut=-10;
        memset(grid, 0, sizeof(grid));
    }

};

#endif //CYBERBALL_OBSTACLE_HPP
