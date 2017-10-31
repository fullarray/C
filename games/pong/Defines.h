//////////////////////////////////////////////////////////////////////////////////
// Defines.h
//////////////////////////////////////////////////////////////////////////////////

#pragma once

// Window related defines //
#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600
#define WINDOW_CAPTION "Paddle Battle"

// Game related defines //
#define FRAMES_PER_SECOND 30
#define FRAME_RATE        1000/FRAMES_PER_SECOND 

// Location of images within bitmap //
#define PADDLE_BITMAP_X 0
#define PADDLE_BITMAP_Y 0
#define BALL_BITMAP_X   100 
#define BALL_BITMAP_Y   0

// Locations of paddles in the game //
#define COMPUTER_Y 30
#define PLAYER_Y   550

// Dimensions of a paddle //
#define PADDLE_WIDTH  100
#define PADDLE_HEIGHT 20

// Diameter of the ball //
#define BALL_DIAMETER 20

// Paddle speeds //
#define PLAYER_SPEED    10
#define COMPUTER_SPEED  10

// Ball speeds //
#define BALL_SPEED_MODIFIER 5  // divide location on paddle by this
#define BALL_SPEED_Y        10 // max speed of ball along y axis

// Location of output text //
#define COMPUTER_SCORE_X 10
#define COMPUTER_SCORE_Y 10
#define PLAYER_SCORE_X   150
#define PLAYER_SCORE_Y   10








