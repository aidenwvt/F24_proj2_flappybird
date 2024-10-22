/*
 * show.h
 *
 *  Created on: Sep 28, 2024
 *      Author: Aiden Wiehn
 */

#ifndef CODE_SHOW_H_
#define CODE_SHOW_H_

#include <HAL/HAL.h>
#include <Code/Header/proj2_app.h>

// Defines for the lanes
#define BORDER_LEFT 0
#define BORDER_RIGHT 127
#define DIVIDER_TOP 107
#define DIVIDER_BOTTOM 20

// Defines for the lives
#define LIFE_YMIN 112
#define LIFE_YMAX 117
#define LIFEONE_XMIN 75
#define LIFEONE_XMAX 80
#define LIFETWO_XMIN 85
#define LIFETWO_XMAX 90
#define LIFETHREE_XMIN 95
#define LIFETHREE_XMAX 100

// Function declarations for showing different screens
void App_proj2_showTitleScreen(GFX *gfx_p, App_proj2 *app_p);
void App_proj2_showMenuScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showInstructionsScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showGameScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showResultScreen(App_proj2 *app_p, GFX *gfx_p);
void App_proj2_showFinalScreen(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);

#endif /* CODE_SHOW_H_ */
