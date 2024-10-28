/*
 * logic.c
 *
 *  Created on: Oct 22, 2024
 *      Author: Aiden Wiehn
 */

#include "Code/Header/logic.h"
#include "Code/Header/sounds.h"

// Function for taking an array of ints, and turns it into
// an array of chars
void make_6digit_NumString(unsigned int num, char *string)
{
    string[0] = (num / 100000) % 10 + '0';
    string[1] = (num / 10000) % 10 + '0';
    string[2] = (num / 1000) % 10 + '0';
    string[3] = (num / 100) % 10 + '0';
    string[4] = (num / 10) % 10 + '0';
    string[5] = num % 10 + '0';
}

// Function responsible for moving the square in both the X and Y directions
// Also responsible for the jumping logic
void moveSquare(GFX *gfx_p, HAL *hal_p, App_proj2 *app_p)
{
    static bool timerStarted = false;
    // Set the graphics to black and draw over the current square
    GFX_setForeground(gfx_p, GRAPHICS_COLOR_BLACK);
    GFX_fillRectangle(gfx_p, app_p->minX, app_p->minY, app_p->maxX,
                      app_p->maxY);

    // Logic for determining which direction to move the player
    if (Joystick_isPressedToRight(&hal_p->joystick) && app_p->maxX < 127)
    {
        float x = return_Joystick_X(&hal_p->joystick);
        app_p->minX += (0.9 / 6000) * (x - 10000) + 0.1;
        app_p->maxX += (0.9 / 6000) * (x - 10000) + 0.1;
    }
    else if (Joystick_isPressedToLeft(&hal_p->joystick) && app_p->minX > 0)
    {
        float x2 = return_Joystick_X(&hal_p->joystick);
        app_p->minX -= (0.9 / 5000) * (5000 - x2) + 0.1;
        app_p->maxX -= (0.9 / 5000) * (5000 - x2) + 0.1;
    }

    // If BB1 is pressed OR we are currently jumping, make the player jump
    if (Button_isTapped(&hal_p->boosterpackS1) || !SWTimer_expired(&app_p->miscTimer))
    {
        // Can only jump if we aren't currently jumping (aka timer isn't started)
       if (timerStarted == false) {
       app_p->miscTimer = SWTimer_construct(JUMP);
       SWTimer_start(&app_p->miscTimer);
       timerStarted = true;
       }
       // Y position logic for jumping
       if (app_p->minY > 22.5) {
           if (Joystick_isPressedToUp(&hal_p->joystick)) {
               app_p->minY -= (0.9 / 6000) * (return_Joystick_Y(&hal_p->joystick) - 12000) + 0.75;
               app_p->maxY -= (0.9 / 6000) * (return_Joystick_Y(&hal_p->joystick) - 12000) + 0.75;
           }
           else if (Joystick_isPressedToDown(&hal_p->joystick)) {
               app_p->minY -= (0.9 / 6000) * (1 / (3000 - return_Joystick_Y(&hal_p->joystick))) + 0.5;
               app_p->maxY -= (0.9 / 6000) * (1 / (3000 - return_Joystick_Y(&hal_p->joystick))) + 0.5;
           }
           else {
               app_p->minY = app_p->minY - 0.75;
               app_p->maxY = app_p->maxY - 0.75;
           }
       }
    }
    // If we aren't jumping, fall down
    else {
        if (timerStarted == true) {
            timerStarted = false;
        }
        if (app_p->maxY < 106) {
        app_p->minY = app_p->minY + 0.5;
        app_p->maxY = app_p->maxY + 0.5;
        }
    }
    // Change graphics back to white and draw the player again
    GFX_setForeground(gfx_p, GRAPHICS_COLOR_WHITE);
    GFX_fillRectangle(gfx_p, app_p->minX, app_p->minY, app_p->maxX,
                      app_p->maxY);
}

// Function responsible for spawning the obstacles as
// well as moving them across the screen
void obstacleSpawner(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
    int continueCheck = -1;
    int i;

    // Iterate through all the obstacles, and stop on the first one that is currently not moving
    for (i = 0; i < OBSTACLE_SIZE; i++)
    {
        if (!app_p->obstacles[i].moving)
        {
            continueCheck = i;
            break;
        }
    }

    // If the timer in charge of the delay in between objects is over, start a new timer
    // isWaiting was originally a static here, but I thought changing it to an app variable would
    // Fix the bug (news flash, it did not :()
    if (!app_p->isWaiting)
    {
        waitDifficulty(app_p);
        SWTimer_start(&app_p->obstacleWait);
        app_p->isWaiting = true;
    }

    // If the wait between objects is over, set up a new object and enable it
    if (continueCheck != -1 && SWTimer_expired(&app_p->obstacleWait))
    {
        for (i = 0; i < OBSTACLE_SIZE; i++)
        {
            Obstacle *newObstacle = &app_p->obstacles[continueCheck];
            obstacleTypeVal(app_p, hal_p, obj_p, continueCheck);
            spawnDifficulty(app_p, newObstacle);
            newObstacle->moving = true;
            SWTimer_start(&newObstacle->obstacleTimer);
            app_p->isWaiting = false;
        }
    }

    // Iterate through every single possible object and update their position and score
    for (i = 0; i < OBSTACLE_SIZE; i++)
    {
        Obstacle *currentObstacle = &app_p->obstacles[i];
        // If currently active, run all of this, if not, immediately move to the next object
        if (currentObstacle->moving)
        {
            // Update x position and draw over it if not out of the screen
            double runTime = SWTimer_percentElapsed(
                    &currentObstacle->obstacleTimer);
            runTime = runTime * 1.35;
            currentObstacle->xMin = (1.1 - runTime) * 127.0;
            currentObstacle->xMax = (1.1 - runTime) * 127.0 + 5;
            // If off screen, disable the obstacle and add +500 points
            if (currentObstacle->xMax < -13)
            {
                currentObstacle->moving = false;
                app_p->score += 500;
            }
            // If still active, redraw the obstacle
            else
            {
                GFX_setForeground(gfx_p, GRAPHICS_COLOR_BLACK);
                GFX_fillRectangle(gfx_p, (1.2 - runTime) * 127.0, currentObstacle->yMin, (1.2 - runTime) * 127.0, currentObstacle->yMax);
                GFX_fillRectangle(gfx_p, (1.2 - runTime) * 127.0, currentObstacle->yMin2, (1.2 - runTime) * 127.0, currentObstacle->yMax2);
                GFX_setForeground(gfx_p, GRAPHICS_COLOR_WHITE);
                GFX_fillRectangle(gfx_p, currentObstacle->xMin, currentObstacle->yMin, currentObstacle->xMax, currentObstacle->yMax);
                GFX_fillRectangle(gfx_p, currentObstacle->xMin, currentObstacle->yMin2, currentObstacle->xMax, currentObstacle->yMax2);
            }
        }
    }
    // Had an issue with the score, these lines of code fixed it
    if (app_p->reset == false)
    {
        app_p->score = 000000;
        app_p->reset = true;
    }
}

// Function that updates the rate at which obstacles spawn depending on the score
void waitDifficulty(App_proj2 *app_p)
{
    if (app_p->score < 2000)
    { // When score is less than 2000 start at starting difficulty
        app_p->obstacleWait = SWTimer_construct(OBSTACLE_WAIT);
    }
    else if (app_p->score <= 4000)
    { // Between 2000 and 4000 points, go to difficulty 2
        app_p->obstacleWait = SWTimer_construct(OBSTACLE_WAIT2);
    }
    else
    { // Above 4000 points, go to the highest difficulty
        app_p->obstacleWait = SWTimer_construct(OBSTACLE_WAIT3);
    }
}

// Function that updates the speed at which obstacles move across the screen
void spawnDifficulty(App_proj2 *app_p, Obstacle *newObstacle)
{
    if (app_p->score < 2000)
    { // When score is less than 2000 start at starting difficulty
        newObstacle->obstacleTimer = SWTimer_construct(OBSTACLE_MOVE);
    }
    else if (app_p->score <= 4000)
    { // Between 2000 and 4000 points, go to difficulty 2
        newObstacle->obstacleTimer = SWTimer_construct(OBSTACLE_MOVE2);
    }
    else
    { // Above 4000 points, go to the highest difficulty
        newObstacle->obstacleTimer = SWTimer_construct(OBSTACLE_MOVE3);
    }
}

// Randomly spawns in the obstacles with a gap in between each of them
void obstacleTypeVal(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p, int i)
{
    Obstacle *newObstacle = &app_p->obstacles[i];

    double gapHeight = 30.0; // Fixed gap size between the pipes
    double minGapY = 20.0;   // The lowest possible y position for the obstacle (top border)
    double maxGapY = 107.0 - gapHeight; // The largest possible y position for the obstacle (bottom border)

    // Randomize the vertical position of the gap
    double randomGapY = minGapY + (rand() / (double)RAND_MAX) * (maxGapY - minGapY);

    // Set the top pipe's values
    newObstacle->yMin = 21.0;
    newObstacle->yMax = randomGapY;

    // Set the bottom pipe's values
    newObstacle->yMin2 = randomGapY + gapHeight;
    newObstacle->yMax2 = 106.0;

    // Set the horizontal position for both obstacles (start off the screen)
    newObstacle->xMin = 129;
    newObstacle->xMax = 134;

    // Initialize the obstacle movement
    newObstacle->moving = true;
}

// Checks if the player has taken damage
void damageCheck(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
       double playerXMin = app_p->minX;
       double playerXMax = app_p->maxX;
       double playerYMin = app_p->minY;
       double playerYMax = app_p->maxY;
       int i;

       for (i = 0; i < OBSTACLE_SIZE; i++)
       {
           Obstacle *obstacle = &app_p->obstacles[i];

           bool collidesWithTop =
               (((playerXMax >= obstacle->xMin && playerXMin <= obstacle->xMax) || (playerXMax >= obstacle->xMin + 7 && playerXMin <= obstacle->xMax + 7))
               && (playerYMin <= obstacle->yMax) && obstacle->moving == true);

           bool collidesWithBottom =
               (((playerXMax >= obstacle->xMin && playerXMin <= obstacle->xMax) || (playerXMax >= obstacle->xMin + 7 && playerXMin <= obstacle->xMax + 7))
               && (playerYMax >= obstacle->yMin2) && obstacle->moving == true);

           // If collision happened, remove a life
           if ((collidesWithBottom || collidesWithTop) && SWTimer_expired(&app_p->iFrames))
           {
               startBluePWMTimer();
               SWTimer_start(&app_p->dmgTimer);
               SWTimer_start(&app_p->iFrames);
               GFX_setForeground(gfx_p, GRAPHICS_COLOR_BLACK);
               GFX_fillRectangle(gfx_p, 95 + (10 * (app_p->lives - 3)), 112, 100 + (10 * (app_p->lives - 3)), 117);
               GFX_setForeground(gfx_p, GRAPHICS_COLOR_WHITE);
               app_p->lives--;
               break;
           }
       }

       if (SWTimer_expired(&app_p->dmgTimer)) {
           stopBluePWMTimer();
       }

       if (app_p->lives == 0)
       {
           GFX_clear(gfx_p);
           app_p->state = FINAL_SCREEN;
       }
}

// Calculates if the score in the current round is higher than
// any previous scores
void calculateHighScore(App_proj2 *app_p, HAL *hal_p)
{
    if (app_p->score > app_p->highScores[0])
    {
        app_p->highScores[2] = app_p->highScores[1];
        app_p->highScores[1] = app_p->highScores[0];
        app_p->highScores[0] = app_p->score;
    }
    else if (app_p->score > app_p->highScores[1])
    {
        app_p->highScores[2] = app_p->highScores[1];
        app_p->highScores[1] = app_p->score;
    }
    else if (app_p->score > app_p->highScores[2])
    {
        app_p->highScores[2] = app_p->score;
    }

    make_6digit_NumString(app_p->highScores[0], app_p->highScoreOne);
    make_6digit_NumString(app_p->highScores[1], app_p->highScoreTwo);
    make_6digit_NumString(app_p->highScores[2], app_p->highScoreThree);
}
