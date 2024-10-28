/*
 * proj2_app.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *      Edited by: Aiden Weiehn
 */

#ifndef PROJ2_APP_H_
#define PROJ2_APP_H_

#include <HAL/Timer.h>

// All definitions responsible for timers
#define TITLE_SCREEN_TIMER 3000
#define OBSTACLE_WAIT 2000 // 2 seconds
#define OBSTACLE_WAIT2 1750 // 1.75 seconds
#define OBSTACLE_WAIT3 1500
#define OBSTACLE_MOVE 5000 // 5 seconds
#define OBSTACLE_MOVE2 4500 // 4.5 seconds
#define OBSTACLE_MOVE3 4000 // 4 seconds
#define UART_TIMER 1000
#define IFRAMES 500 // 0.5 seconds
#define JUMP 250 // 1 second
#define DAMAGE 50 // 0.05 seconds

// Defines responsible for game logic
#define LIVES 3
#define SCORE 000000
#define OBSTACLE_SIZE 3

// Defines for position of the player
#define MIN_X 10.0
#define MAX_X 15.0
#define MIN_Y 69.0
#define MAX_Y 74.0

#define LOWERBOUND -1
#define UPPERBOUND 1

/**
 * The top-level Application object, initialized in the main() entry point and
 * passed around to basically every other function. Holds state variables
 * which YOU define as part of your application's state.
 */

// Struct containing all the FSM states
enum _GameState
{
    TITLE_SCREEN,
    MENU_SCREEN,
    INSTRUCTIONS_SCREEN,
    GAME_SCREEN,
    FINAL_SCREEN,
    RESULTS_SCREEN
};
typedef enum _GameState GameState;

// Struct for cursor positions
enum _Cursor
{
    CURSOR_0,
    CURSOR_1,
    CURSOR_2
};

typedef enum _Cursor Cursor;

// Struct that contains all the information about an obstacle
struct _Obstacles
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double yMin2;
    double yMax2;
    bool moving;
    SWTimer obstacleTimer;
};
typedef struct _Obstacles Obstacle;

// Application struct
struct _App_proj2
{
    SWTimer miscTimer, obstacleWait, titleTimer, iFrames, waitTimer, dmgTimer; // General-purpose timer for when screens must disappear
    GameState state;
    Cursor cursor;
    int highScores[3];
    int lives;
    bool reset, isWaiting;
    unsigned int score;
    char currentScore[7], highScoreOne[7], highScoreTwo[7], highScoreThree[7];
    float minX, maxX, minY, maxY;
    Obstacle obstacles[OBSTACLE_SIZE];
};
typedef struct _App_proj2 App_proj2;

// Boilerplate constructor function for the Application
App_proj2 App_proj2_construct(HAL *hal_p);
Obstacle App_obstacle_construct(HAL *hal_p, App_proj2 *app_p);
void App_proj2_loop(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);

#endif /* PROJ2_APP_H_ */
