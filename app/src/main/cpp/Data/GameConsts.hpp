#ifndef CYBERBALL_GAMECONSTS_HPP
#define CYBERBALL_GAMECONSTS_HPP

// Render settings
#define RENDER_FOV 45.0f
#define RENDER_NEAR_CLIP 0.1f
#define RENDER_FAR_CLIP 200.0f

// size of a texture
#define TEXTURE_SIZE 64

// maximum delta T between two frames
#define MAX_DELTA_T 0.05f

// BALL

// Distance between the ball and center of a screen
#define BALL_CENTER_THRESH 4.0f

// Ball model params
#define BALL_SLICES 10
#define BALL_RADIUS 0.5f
#define BALL_FIRST_COLOR {1.0f, 0.0f, 1.0f}
#define BALL_SECOND_COLOR {1.0f, 1.0f, 0.0f}
#define BALL_POS_X -16.0f

// Rotation Speed limits
#define BALL_MIN_ROTATE 4.0f
#define BALL_MAX_ROTATE 10.0f

// OBSTACLES

// distance between the obstacles
#define OBS_DISTANCE_BETWEEN 8.0f

// An obstacle is a grid of boxes. This indicates how many boxes by how many boxes this grid is.
#define OBS_GRID_SIZE 15

// How many cells we cut?
#define CUT_CELLS 4

// This is how wide each of the grid cells are
#define OBS_CELL_SIZE ((2*TUNNEL_HALF_H)/(float)OBS_GRID_SIZE)

// Ratio between the size of the box and size of the cell. Keep under 1.0f!
#define BOX_CELL_RATIO 0.9f

// size of each obstacle box
#define OBS_BOX_SIZE (BOX_CELL_RATIO * OBS_CELL_SIZE)

// index of the first NOT-NULL obstacle
#define OBS_START_IND 3

// once a tunnel section or an obstacle is this far behind the camera, delete it
#define SHIFT_THRESH 15.0f

// TUNNEL

// Size of the tunnel
#define TUNNEL_HALF_W 20.0f
#define TUNNEL_HALF_H 10.0f

// length of each tunnel section
#define TUNNEL_SECTION_LENGTH 100.0f

// number of tunnel sections to render ahead
#define RENDER_TUNNEL_SECTION_COUNT 4

// GAME PARAMS

#define CAMERA_SPEED 7.5f // 10.0f
#define BALL_KICK 60.0f // 75
#define GRAVITY_CONST 65.0f //65
#define UP_SPEED_LIMIT 20.0f
#define DOWN_SPEED_LIMIT -25.0f

// rotation acceleration
#define ROTATION_ACCELERATION 7.0f
#define ROTATION_KICK 6.0f

// bounds on player's movement
#define MAX_X TUNNEL_HALF_W - BALL_RADIUS
#define MIN_X -(MAX_X)
#define MAX_Z TUNNEL_HALF_H - BALL_RADIUS
#define MIN_Z -(MAX_Z)

// settings for rendering the score to the screen
#define SCORE_POS_Y 0.93f
#define SCORE_FONT_SCALE 1.0f

// scale of the signs that appear onscreen
#define SIGN_FONT_SCALE 0.9f

// how long do signs stay onscreen
#define SIGN_DURATION 0.2f

// duration of a text sign's zoom-in animation
#define SIGN_ANIM_DUR 0.2f

// Game Over tone. First row is "noise" with f0, second is actual sound
#define TONE_GAME_OVER "a100 d15 f0. a40 d75 f0. a30 f0. a20 f0. d120 a0." \
        "f415 a70. a0. f392 a70. a0. f370 a70. a0. f349 a70. a0. d240 f329 a70. a0. d180 f311 a70."

// High Score tone. First row is "noise" with f0, second is actual sound
#define TONE_HIGH_SCORE "a100 d15 f0. a40 d75 f0. a30 f0. a20 f0. d120 a0." \
        "f247 a70. a0. f311 a70. a0. f370 a70. a0. f311 a70. a0. f370 a70. a0. d300 f494 a70."

#define TONE_TOUCH " a20 d40 f440. f740."

// Light Colors, first is main, second used when game is over
#define COLORS_LIGHT {0.0f, 0.0f, 0.9f, \
                      0.5f, 0.5f, 0.5f}

#define COLORS_BALL {1.0f, 1.0f, 0.0f, \
                     0.0f, 1.0f, 1.0f}

// Obstacle Colors, first row is not used cause Style-0 represents an empty obstacle
#define COLORS_OBS {0.0f, 0.0f, 0.0f, \
                    0.0f, 0.4f, 1.0f, \
                    0.0f, 1.0f, 0.0f, \
                    0.0f, 1.0f, 1.0f, \
                    1.0f, 0.0f, 0.0f, \
                    1.0f, 0.0f, 1.0f, \
                    1.0f, 1.0f, 0.0f}

// Tunnel Colors
#define COLORS_TUNNEL {0.7f, 0.2f, 0.8f, \
                       0.7f, 1.0f, 0.1f, \
                       0.2f, 0.7f, 0.9f}

// player's acceleration, in units per second squared
#define CAMERA_ACCELERATION_NEGATIVE_SPEED 10.0f  // used when speed is negative
#define CAMERA_ACCELERATION_POSITIVE_SPEED 40.0f  // used when speed is positive

// how long does the game take to end after we show Game Over
#define GAME_OVER_EXPIRE 120.0f
#define GAME_OVER_STATS 1.0f

// UI transition animation duration
#define TRANSITION_DURATION 0.25f

// menu item pulse animation settings
#define SIGN_PULSE_AMOUNT 1.1f
#define SIGN_PULSE_PERIOD 0.5f

// save file name
#define SAVE_FILE_NAME "score.dat"


#endif //CYBERBALL_GAMECONSTS_HPP
