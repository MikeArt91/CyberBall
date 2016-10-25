#include <Data/GameConsts.hpp>
#include "ObstacleManager.hpp"

void ObstacleGenerator::Generate(Obstacle *result) {
    result->Reset();

    // choose one of them randomly
    result->style = 1 + Random(7);

    // Obstacle *o = result;
    int cut = 3 + Random(7);
    result->cut = cut;

    for (int i=0; i<=3; i++) {
        FillCol(result,i);
    }

    for (int j=0; j<CUT_CELLS; j++) {
        EraseRow(result,cut+j);
    }

}

void ObstacleGenerator::FillRow(Obstacle *result, int row) {
    for (int i = 0; i < OBS_GRID_SIZE; ++i) {
        result->grid[i][row] = true;
    }
}

void ObstacleGenerator::EraseRow(Obstacle *result, int row) {
    for (int i = 0; i < OBS_GRID_SIZE; ++i) {
        result->grid[i][row] = false;
    }
}

void ObstacleGenerator::FillCol(Obstacle *result, int col) {
    for (int i = 0; i < OBS_GRID_SIZE; ++i) {
        result->grid[col][i] = true;
    }

}