/*
 * logic.h
 *
 *  Created on: Sep 28, 2024
 *      Author: Aiden Wiehn
 */

#ifndef CODE_LOGIC_H_
#define CODE_LOGIC_H_

#include <HAL/HAL.h>
#include <Code/Header/proj2_app.h>
#include <stdlib.h>

// Methods in charge of all the game logic (spawning obstacles, moving players, etc)
void make_6digit_NumString(unsigned int num, char *string);
void moveSquare(GFX *gfx_p, HAL *hal_p, App_proj2 *app_p);
void obstacleSpawner(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);
void obstacleTypeVal(App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p, int i);
void damageCheck(GFX *gfx_p, App_proj2 *app_p, HAL *hal_p, Obstacle *obj_p);
void calculateHighScore(App_proj2 *app_p, HAL *hal_p);
void waitDifficulty(App_proj2 *app_p);
void spawnDifficulty(App_proj2 *app_p, Obstacle *newObstacle);

#endif /* CODE_LOGIC_H_ */
