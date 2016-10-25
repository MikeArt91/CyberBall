#ifndef CYBERBALL_OBSTACLEMANAGER_HPP
#define CYBERBALL_OBSTACLEMANAGER_HPP

#include "Core/Engine.hpp"
#include "Obstacle.hpp"

// Generates obstacles given a difficulty level.
class ObstacleGenerator {
public:
    // generate a new obstacle.
    void Generate(Obstacle *result);

private:
    void FillRow(Obstacle *result, int row);
    void EraseRow(Obstacle *result, int col);
    void FillCol(Obstacle *result, int col);
};

#endif //CYBERBALL_OBSTACLEMANAGER_HPP
