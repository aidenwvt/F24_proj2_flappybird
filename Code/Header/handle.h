/*
 * handle.h
 *
 *  Created on: Sep 28, 2024
 *      Author: Aiden Wiehn
 */

#ifndef CODE_HANDLE_H_
#define CODE_HANDLE_H_

#include <HAL/HAL.h>
#include <Code/Header/proj2_app.h>

// These methods are in charge of all the logic, which functions to call etc.
// For each respective screen in the game
void App_proj2_handleTitleScreen(App_proj2 *app_p, HAL *hal_p);
void App_proj2_handleMenuScreen(App_proj2 *app_p, HAL *hal_p, GFX *gfx_p);
void App_proj2_handleInstructionsScreen(App_proj2 *app_p, HAL *hal_p);
void App_proj2_handleGameScreen(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);
void App_proj2_handleResultScreen(App_proj2 *app_p, HAL *hal_p);
void App_proj2_handleFinalScreen(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);

#endif /* CODE_HANDLE_H_ */
