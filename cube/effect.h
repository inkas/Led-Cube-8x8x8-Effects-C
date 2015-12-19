#ifndef EFFECT_H
#define EFFECT_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "cube.h"

void effect_box_shrink_grow (int iterations, int rot, int flip, uint16_t delay);

void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay);
void sendvoxels_rand_z (int iterations, int delay, int wait);

void effect_planboing (int plane, int speed);

void effect_random_filler (int delay, int state);

void effect_z_updown (int iterations, int delay);
void effect_rain(int iterations, int delay);
void effect_stringfly2(char * str);
void effect_blinky2(int delay, int delayFill, int wait);
void draw_positions_axis (char axis, unsigned char positions[64], int invert);
void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert);

void effect_random_sparkle_flash (int iterations, int voxels, int delay);
void effect_random_sparkle (int iterations, int delay);

void effect_box_woopwoop (int delay, int grow);
void effect_telcstairs (int invert, int delay, int val);
void effect_loadbar(int delay);
void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode);
void effect_pathmove (unsigned char *path, int length);
void effect_rand_patharound (int iterations, int delay);
void effect_pathspiral (int iterations, int delay);
void effect_path_text (int delay, char *str);
void effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis);

void fireworks(int iterations, int delay, int explode_speed, uint8_t explode_range, uint8_t explode_density);
void fireworks_flash(uint8_t floor, uint8_t x, uint8_t y, int explode_speed, uint8_t explode_range, uint8_t explode_density);

void border_box(int delay, int iterations);

void moving_boxes(int delay,int iter);
void filled_box_draw(int x,int y,int z,uint8_t size,uint8_t mode);

void diagonal_triangle(int delay, int iterations);

void planes_in_order(int delay, int iterations);

void planes_exchange(int delay, int iterations);

void crazy_straw(int delay, int iterations);

//void explosing_sphere(int delay, int iterations);
void bomb(int iterations, int delay);
void walk_through_walls(int delay, int iterations);
void snake(int delay, int iterations);


//void plotCircle(int xm, int ym, int r);
//void border_box_effect(int delay);
//void border_box_draw(int delay, int wall, int layer, int cube_size);
//int fireworks_flash_extend(int generatedRandomNumber, uint8_t explode_range);
#endif

