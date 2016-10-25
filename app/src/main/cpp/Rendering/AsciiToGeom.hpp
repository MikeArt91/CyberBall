#ifndef CYBERBALL_ASCIITOGEOM_HPP
#define CYBERBALL_ASCIITOGEOM_HPP

#include "Core/Engine.hpp"
#include "SimpleGeom.hpp"

/* Converts ASCII art into a Vbo/Ibo pair. Useful for retro-looking drawings/text!
 * scale is the size of each character. The center of the rendering will be 0,0.
 *
 * Examples:
 *
 * Square:
 *     +---+
 *     |   |
 *     +---+
 *
 * Triangle:
 *     +-----+
 *      `   /
 *       ` /
 *        +
 *
 * The + sign represents a vertex; lines are represented by -, /, ` and |.
 */
SimpleGeom* AsciiArtToGeom(const char *art, float scale);

#endif //CYBERBALL_ASCIITOGEOM_HPP
