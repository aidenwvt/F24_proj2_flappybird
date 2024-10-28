/*
 * handle.c
 *
 *  Created on: Oct 22, 2024
 *      Author: Aiden Wiehn
 */

#include "Code/Header/handle.h"
#include "Code/Header/show.h"
#include "Code/Header/logic.h"
#include "Code/Header/sounds.h"

void App_proj2_handleTitleScreen(App_proj2 *app_p, HAL *hal_p)
{
    App_proj2_showTitleScreen(&hal_p->gfx, app_p);
    if (SWTimer_expired(&app_p->miscTimer)) // When the timer expires, go to the menu screen
    {
        app_p->state = MENU_SCREEN;
        App_proj2_showMenuScreen(app_p, &hal_p->gfx);
    }
}

// Controls the menu logic, cursor, and navigating the menu
void App_proj2_handleMenuScreen(App_proj2 *app_p, HAL *hal_p, GFX *gfx_p)
{
    switch (app_p->cursor)
    {
    case CURSOR_0: // Cursor next to the play game option
        GFX_print(gfx_p, ">", 6, 0);
        if (Joystick_isTappedToDown(&hal_p->joystick))
        {
            startShiftPWMTimer();
            GFX_print(gfx_p, " ", 6, 0);
            app_p->cursor = CURSOR_1;
        }
        break;
    case CURSOR_1: // Cursor next to instructions
        GFX_print(gfx_p, ">", 7, 0);
        if (Joystick_isTappedToDown(&hal_p->joystick))
        {
            startShiftPWMTimer();
            GFX_print(gfx_p, " ", 7, 0);
            app_p->cursor = CURSOR_2;
        }
        else if (Joystick_isTappedToUp(&hal_p->joystick))
        {
            startShiftPWMTimer();
            GFX_print(gfx_p, " ", 7, 0);
            app_p->cursor = CURSOR_0;
        }
        break;
    case CURSOR_2: // Cursor next to high scores
        GFX_print(gfx_p, ">", 8, 0);
        if (Joystick_isTappedToUp(&hal_p->joystick))
        {
            startShiftPWMTimer();
            GFX_print(gfx_p, " ", 8, 0);
            app_p->cursor = CURSOR_1;
        }
        break;
    }
    // This timer is used to ensure the obstacles from the previous game are expired
    // Only starts running after a game is finished
    if (app_p->cursor == CURSOR_0 && Button_isTapped(&hal_p->boosterpackJS) && SWTimer_expired(&app_p->waitTimer)) // If cursor is next to game and JS pressed, start game
    {
        GFX_clear(gfx_p);
        app_p->state = GAME_SCREEN;
    }
    else if (app_p->cursor == CURSOR_1 // If cursor is next to instructions and JS pressed, go to instructions
    && Button_isTapped(&hal_p->boosterpackJS))
    {
        GFX_clear(gfx_p);
        app_p->state = INSTRUCTIONS_SCREEN;
    }
    else if (app_p->cursor == CURSOR_2 // If cursor is next to high scores and JS pressed, go to high scores
    && Button_isTapped(&hal_p->boosterpackJS))
    {
        GFX_clear(gfx_p);
        app_p->state = RESULTS_SCREEN;
    }
    stopShiftPWMTimer();
}

// Prints the instruction screen
void App_proj2_handleInstructionsScreen(App_proj2 *app_p, HAL *hal_p)
{
    static bool isShown = false;
    if (isShown == false) // Show graphics once
    {
        App_proj2_showInstructionsScreen(app_p, &hal_p->gfx);
        isShown = true;
    }
    if (Button_isTapped(&hal_p->boosterpackJS)) // If JS pressed, go back to menu
    {
        GFX_clear(&hal_p->gfx);
        App_proj2_showMenuScreen(app_p, &hal_p->gfx);
        app_p->state = MENU_SCREEN;
        isShown = false;
    }
}

// Function responsible for all the game logic
void App_proj2_handleGameScreen(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p)
{
    static bool isShown = false;
    if (isShown == false) // Show graphics once
    {
        App_proj2_showGameScreen(app_p, &hal_p->gfx);
        isShown = true;
    }

// These two functions constantly update the current score

    make_6digit_NumString(app_p->score, app_p->currentScore);
    GFX_print(&hal_p->gfx, app_p->currentScore, 1, 11);

// moveSquare, obstacleSpawner, and damageCheck are the three functions
// in charge of all the moving parts of the game

    moveSquare(&hal_p->gfx, hal_p, app_p);
    obstacleSpawner(&hal_p->gfx, app_p, hal_p, obj_p);
    damageCheck(&hal_p->gfx, app_p, hal_p, obj_p);

// Turn off the game when all lives are lost

    if (app_p->lives == 0)
    {
        isShown = false;
    }
}

// Prints the result screen
void App_proj2_handleResultScreen(App_proj2 *app_p, HAL *hal_p)
{
    static bool isShown = false;
    if (isShown == false) // Show graphics once
    {
        App_proj2_showResultScreen(app_p, &hal_p->gfx);
        isShown = true;
    }
    if (Button_isTapped(&hal_p->boosterpackJS)) // If JS pressed, go back to menu
    {
        GFX_clear(&hal_p->gfx);
        App_proj2_showMenuScreen(app_p, &hal_p->gfx);
        app_p->state = MENU_SCREEN;
        isShown = false;
    }
}

// When all lives are lost, show the final screen, and when
// the JS is pressed, go back to the menu and reset all variables
void App_proj2_handleFinalScreen(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p,
                                 Obstacle *obj_p)
{
    // Print out the final screen graphics
    if (SWTimer_expired(&app_p->dmgTimer)) {
        stopBluePWMTimer();
    }
    App_proj2_showFinalScreen(app_p, hal_p, obj_p);
    SWTimer_start(&app_p->waitTimer);
    if (Button_isTapped(&hal_p->boosterpackJS)) // When JS pressed, go back to menu and reset all variables needed for the game
    {
        GFX_clear(&hal_p->gfx);
        App_proj2_showMenuScreen(app_p, &hal_p->gfx);
        calculateHighScore(app_p, hal_p);
        app_p->state = MENU_SCREEN;
        app_p->lives = LIVES;
        app_p->score = SCORE;
        app_p->minX = MIN_X;
        app_p->maxX = MAX_X;
        app_p->minY = MIN_Y;
        app_p->maxY = MAX_Y;
        app_p->reset = false;
        app_p->isWaiting = false;
        stopBluePWMTimer();
    }
}


