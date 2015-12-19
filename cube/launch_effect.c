#include "launch_effect.h"
#include "effect.h"
#include "draw.h"

void launch_effect (int effect){
    unsigned char ii;
	fill(0x00);

	switch (effect){		
		case 0x00:
			snake(700, 500);		//int delay, int iterations, uint8_t snake_length
			break;
		
		case 1:
			walk_through_walls(300, 3);		//int delay, int iterations
			break;
		
		case 2:
			bomb(300, 50);		//int delay, int iterations
			break;
		
		case 3:
			crazy_straw(250, 100);		//int delay, int iterations
			break;
			
		case 4:
			moving_boxes(400,150);			//int delay, int iterations
			break;
		
		case 5:
			diagonal_triangle(400, 20);		//int delay, int iterations
			break;
		
		case 6:
			planes_exchange(500, 40);		//int delay, int iterations
			break;
		
		case 7:
			planes_in_order(500, 7);		//int delay, int iterations
			break;
		
		case 8:
			border_box(350, 5);				//int delay, int iterations
			break;
		
		case 9:
			effect_rain(300, 300);			//iterations, delay
			break;

		//OK
		case 10:
			sendvoxels_rand_z(20,220,2000);		//iterations, delay, wait
			break;

		//OK
		case 11:
			effect_random_filler(5,1);		//delay, state
			effect_random_filler(5,0);		//delay, state
			effect_random_filler(5,1);		//delay, state
			effect_random_filler(5,0);		//delay, state
			break;

		//OK
		case 12:
			effect_z_updown(20, 400);		//iterations, delay
			break;

		//OK
		case 13:
			effect_blinky2(750, 100, 1000);		//delay, delayFill, wait
			break;

		//OK
		case 14: 
            for (ii=0;ii<8;ii++) {
				effect_box_shrink_grow (2, ii%4, ii & 0x04, 450);		//iterations, rot, flip, delay
			}

			effect_box_woopwoop(800,0);		//delay, grow
			effect_box_woopwoop(800,1);		//delay, grow
			effect_box_woopwoop(800,0);		//delay, grow
			effect_box_woopwoop(800,1);		//delay, grow
			break;

		//OK	
		case 15:
			effect_planboing (AXIS_Z, 400);		//plane, speed
			effect_planboing (AXIS_X, 400);		//plane, speed
			effect_planboing (AXIS_Y, 400);		//plane, speed
			effect_planboing (AXIS_Z, 400);		//plane, speed
			effect_planboing (AXIS_X, 400);		//plane, speed
			effect_planboing (AXIS_Y, 400);		//plane, speed
			fill(0x00);
			break;

		//OK
		case 16:
			fill(0x00);
			effect_telcstairs(0,800,0xff);		//invert, delay, val
			effect_telcstairs(0,800,0x00);		//invert, delay, val
			effect_telcstairs(1,800,0xff);		//invert, delay, val
			effect_telcstairs(1,800,0xff);		//invert, delay, val
			break;

		//OK
		//try to make it repeat (to last longer)
		case 17:
			effect_loadbar(700);		//delay
			break;

		//OK
		case 18:
			effect_axis_updown_randsuspend(AXIS_Z, 350,5000,0);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_Z, 350,5000,1);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_Z, 350,5000,0);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_Z, 350,5000,1);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_X, 350,5000,0);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_X, 350,5000,1);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_Y, 350,5000,0);		//char axis, delay, sleep, invert
			effect_axis_updown_randsuspend(AXIS_Y, 350,5000,1);		//char axis, delay, sleep, invert
			break;

		//OK
		case 19:
			effect_boxside_randsend_parallel (AXIS_Z, 0 , 200,1);	//char axis, int origin, int delay, int mode
			delay_ms(1500);
			effect_boxside_randsend_parallel (AXIS_Z, 1 , 200,1);	//char axis, int origin, int delay, int mode
			delay_ms(1500);
			
			effect_boxside_randsend_parallel (AXIS_Z, 0 , 200,2);	//char axis, int origin, int delay, int mode
			delay_ms(1500);
			effect_boxside_randsend_parallel (AXIS_Z, 1 , 200,2);	//char axis, int origin, int delay, int mode
			delay_ms(1500);
			
			effect_boxside_randsend_parallel (AXIS_Y, 0 , 200,1);	//char axis, int origin, int delay, int mode
			delay_ms(1500);
			effect_boxside_randsend_parallel (AXIS_Y, 1 , 200,1);	//char axis, int origin, int delay, int mode
			delay_ms(1500);
			break;

		//OK
		case 20:
			effect_rand_patharound(500,500);		//int iterations, int delay
			break;

		//OK - Fireworks	
		case 21:
			effect_random_sparkle(20, 200);		//int iterations, int delay
			break;
			
	}
}

