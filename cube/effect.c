#include "effect.h"
#include "draw.h"
#include "font.h"
#include <math.h>
#include <avr/interrupt.h>

//EFFECT 0
void effect_rain (int iterations, int delay){
	int i, ii;
	int rnd_x;
	int rnd_y;
	int rnd_num;
	
	for (ii=0;ii<iterations;ii++){
		rnd_num = rand()%4;
		
		for (i=0; i < rnd_num;i++){
			rnd_x = rand()%8;
			rnd_y = rand()%8;
			setvoxel(rnd_x,rnd_y,7);
		}
		
		delay_ms(delay);
		shift(AXIS_Z,-1);
	}
}

//EFFECT 1
// For each coordinate along X and Y, a voxel is set either at level 0 or at level 7
// for n iterations, a random voxel is sent to the opposite side of where it was.
void sendvoxels_rand_z (int iterations, int delay, int wait){
	unsigned char x, y, last_x = 0, last_y = 0, i;

	fill(0x00);

	// Loop through all the X and Y coordinates
	for (x=0;x<8;x++){
		for (y=0;y<8;y++){
			// Then set a voxel either at the top or at the bottom
            // rand()%2 returns either 0 or 1. multiplying by 7 gives either 0 or 7.
            setvoxel(x,y,((rand()%2)*7));
		}
	}

	for (i=0;i<iterations;i++){
		// Pick a random x,y position
		x = rand()%8;
		y = rand()%8;
		// but not the sameone twice in a row
		if (y != last_y && x != last_x){
			// If the voxel at this x,y is at the bottom
			if (getvoxel(x,y,0)){
				// send it to the top
				sendvoxel_z(x,y,0,delay);
			} 
			else{
				// if its at the top, send it to the bottom
				sendvoxel_z(x,y,7,delay);
			}
			delay_ms(wait);
			
			// Remember the last move
			last_y = y;
			last_x = x;
		}
	}

}
// Send a voxel flying from one side of the cube to the other
// If its at the bottom, send it to the top..
void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay){
	int i, ii;
	for (i=0; i<8; i++){
		if (z == 7){
			ii = 7-i;
			clrvoxel(x,y,ii+1);
		}
		else{
			ii = i;
			clrvoxel(x,y,ii-1);
		}
		setvoxel(x,y,ii);
		delay_ms(delay);
	}
}

//EFFECT 2
// Set or clear exactly 512 voxels in a random order.
void effect_random_filler (int delay, int state){
	int x,y,z;
	int loop = 0;

	if (state == 1){
		fill(0x00);
	}
	else{
		fill(0xff);
	}
	
	while (loop<511){
		x = rand()%8;
		y = rand()%8;
		z = rand()%8;

		if ((state == 0 && getvoxel(x,y,z) == 0x01) || (state == 1 && getvoxel(x,y,z) == 0x00)){
			altervoxel(x,y,z,state);
			delay_ms(delay);
			loop++;
		}
	}
}

//EFFECT 3
void effect_z_updown (int iterations, int delay){
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,y,move;
	
	for (i=0; i<64; i++){
		positions[i] = 4;
		destinations[i] = rand()%8;
	}

	for (i=0; i<8; i++){
		effect_z_updown_move(positions, destinations, AXIS_Z);
		delay_ms(delay);
	}
	
	for (i=0;i<iterations;i++){
		for (move=0;move<8;move++){
			effect_z_updown_move(positions, destinations, AXIS_Z);
			delay_ms(delay);
		}

		delay_ms(delay*4);

		for (y=0;y<32;y++){
			destinations[rand()%64] = rand()%8;
		}
	}
}
void effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis){
	int px;
	for (px=0; px<64; px++){
		if (positions[px]<destinations[px]){
			positions[px]++;
		}
		if (positions[px]>destinations[px]){
			positions[px]--;
		}
	}
	draw_positions_axis (AXIS_Z, positions,0);
}
void draw_positions_axis (char axis, unsigned char positions[64], int invert){
	int x, y, p;
	fill(0x00);
	
	for (x=0; x<8; x++){
		for (y=0; y<8; y++){
			if (invert){
				p = (7-positions[(x*8)+y]);
			}
			else{
				p = positions[(x*8)+y];

			}
			if (axis == AXIS_Z) {
				setvoxel(x,y,p);
			}
			if (axis == AXIS_Y) {
				setvoxel(x,p,y);
			}	
			if (axis == AXIS_X) {
				setvoxel(p,y,x);
			}
		}
	}
}

//EFFECT 4
void effect_blinky2(int delay, int delayFill, int wait){
	int r;
	fill(0x00);
	
	for (r=0;r<2;r++){
		while (delay>0){
			fill(0x00);
			delay_ms(delay);
			
			fill(0xff);
			delay_ms(delayFill);
			
			delay = delay - (15+(1000/(delay/10)));
		}
		
		delay_ms(wait);
		
		while (delay>0){
			fill(0x00);
			delay_ms((delay+1) - delay);
			
			fill(0xff);
			delay_ms(delayFill);
			
			delay = delay - (15+(1000/(delay/10)));
		}
	}
}

//EFFECT 5
void effect_box_shrink_grow (int iterations, int rot, int flip, uint16_t delay){
	int x, i, xyz;
	for (x=0;x<iterations;x++){
		for (i=0;i<16;i++){
            xyz = 7-i; // This reverses counter i between 0 and 7.
            if (i > 7) {
                xyz = i-8; // at i > 7, i 8-15 becomes xyz 0-7.
            }
            fill(0x00); delay_ms(1);
            cli(); // disable interrupts while the cube is being rotated
			box_wireframe(0,0,0,xyz,xyz,xyz);

            if (flip > 0) {// upside-down
                mirror_z();
			}
            if (rot == 1 || rot == 3) {
                mirror_y();
			}
            if (rot == 2 || rot == 3) {
                mirror_x();
            }
            sei(); // enable interrupts
			delay_ms(delay);
	        fill(0x00);
        }
    }
}
// Creates a wireframe box that shrinks or grows out from the center of the cube.
void effect_box_woopwoop (int delay, int grow){
	int i,ii;
	
	fill(0x00);
	for (i=0;i<4;i++){
        ii = i;
        if (grow > 0) {
			ii = 3-i;
		}
		box_wireframe(4+ii,4+ii,4+ii,3-ii,3-ii,3-ii);
		delay_ms(delay);
		fill(0x00);
	}
}

//EFFECT 6
// Draw a plane on one axis and send it back and forth once.
void effect_planboing (int plane, int speed){
	int i;
	for (i=0;i<8;i++){
		fill(0x00);
        setplane(plane, i);
		delay_ms(speed);
	}
	
	for (i=7;i>=0;i--){
		fill(0x00);
        setplane(plane,i);
		delay_ms(speed);
	}
}

//EFFECT 7
int effect_telcstairs_do(int x, int val, int delay){
	int y,z;

	for(y = 0, z = x; y <= z; y++, x--){
		if(x < CUBE_SIZE && y < CUBE_SIZE){
			cube[x][y] = val;
		}
	}
	delay_ms(delay);
	return z;
}
void effect_telcstairs (int invert, int delay, int val){
	int x;

	if(invert){
		for(x = CUBE_SIZE*2; x >= 0; x--){
			x = effect_telcstairs_do(x,val,delay);
		}
	}
	else {
		for(x = 0; x < CUBE_SIZE*2; x++) {
			x = effect_telcstairs_do(x,val,delay);
		}
	}
}


//EFFECT 8
// Light all leds layer by layer,
// then unset layer by layer
void effect_loadbar(int delay){
	fill(0x00);
	
	int z,y;
	
	for (z=0;z<8;z++){
		for (y=0;y<8;y++)
			cube[z][y] = 0xff;
			
		delay_ms(delay);
	}
	
	delay_ms(delay*3);
	
	for (z=0;z<8;z++){
		for (y=0;y<8;y++)
			cube[z][y] = 0x00;
			
		delay_ms(delay);
	}
}

//EFFECT 9
void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert){
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,px;
	
    // Set 64 random positions
	for (i=0; i<64; i++){
		positions[i] = 0; // Set all starting positions to 0
		destinations[i] = rand()%8;
	}

    // Loop 8 times to allow destination 7 to reach all the way
	for (i=0; i<8; i++){
        // For every iteration, move all position one step closer to their destination
		for (px=0; px<64; px++){
			if (positions[px]<destinations[px]){
				positions[px]++;
			}
		}
        // Draw the positions and take a nap
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay);
	}
	
    // Set all destinations to 7 (opposite from the side they started out)
	for (i=0; i<64; i++){
		destinations[i] = 7;
	}
	
    // Suspend the positions in mid-air for a while
	delay_ms(sleep);
	
    // Then do the same thing one more time
	for (i=0; i<8; i++){
		for (px=0; px<64; px++){
			if (positions[px]<destinations[px]){
				positions[px]++;
			}
			if (positions[px]>destinations[px]){
				positions[px]--;
			}
		}
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay);
	}
}

//EFFECT 10
void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode)
{
	int i;
	int done;
	unsigned char cubepos[64];
	unsigned char pos[64];
	int notdone = 1;
	int notdone2 = 1;
	int sent = 0;
	
	for (i=0;i<64;i++)
	{
		pos[i] = 0;
	}
	
	while (notdone)
	{
		if (mode == 1)
		{
			notdone2 = 1;
			while (notdone2 && sent<64)
			{
				i = rand()%64;
				if (pos[i] == 0)
				{
					sent++;
					pos[i] += 1;
					notdone2 = 0;
				}
			}
		} else if (mode == 2)
		{
			if (sent<64)
			{
				pos[sent] += 1;
				sent++;
			}
		}
		
		done = 0;
		for (i=0;i<64;i++)
		{
			if (pos[i] > 0 && pos[i] <7)
			{
				pos[i] += 1;
			}
				
			if (pos[i] == 7)
				done++;
		}
		
		if (done == 64)
			notdone = 0;
		
		for (i=0;i<64;i++)
		{
			if (origin == 0)
			{
				cubepos[i] = pos[i];
			} else
			{
				cubepos[i] = (7-pos[i]);
			}
		}
		
		
		delay_ms(delay);
		draw_positions_axis(axis,cubepos,0);
	//	LED_PORT ^= LED_RED;
	}
	
}

//EFFECT 11
//WILL HELP FOR FIREWORKS (effect_pathmove and effect_pathspiral)
void effect_pathspiral (int iterations, int delay){
	int z, i;
	z = 4;
	unsigned char path[16];
	
	font_getpath(1,path,16);
	
	for (i = 0; i < iterations; i++){
		setvoxel(4,0,i%8);
		delay_ms(delay);
		effect_pathmove(path, 28);
	}
}
void effect_pathmove (unsigned char *path, int length){
	int i,z;
	unsigned char state;
	
	for (i=(length-1);i>=1;i--){
		for (z=0;z<8;z++){
			state = getvoxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
			altervoxel(((path[i]>>4) & 0x0f), (path[i] & 0x0f), z, state);
		}
	}
	for (i=0;i<8;i++) {
		clrvoxel(((path[0]>>4) & 0x0f), (path[0] & 0x0f),i);
	}
}

//EFFECT 12
void effect_rand_patharound (int iterations, int delay){
	int z, dz, i;
	z = 4;
	unsigned char path[28];
	
	font_getpath(0,path,28);
	
	for (i = 0; i < iterations; i++){
		dz = ((rand()%3)-1);
		z += dz;
		
		if (z>7) {
			z = 7;
		}
		if (z<0) {
			z = 0;
		}
		effect_pathmove(path, 28);
		setvoxel(0,7,z);
		delay_ms(delay);
	}
}

//EFFECT 13
// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.
// Set n number of voxels at random positions
void effect_random_sparkle (int iterations, int delay){
	int i;
	for (i=1;i<20;i++){
		effect_random_sparkle_flash(iterations, i, delay);
	}
	for (i=20;i>=1;i--){
		effect_random_sparkle_flash(iterations, i, delay);
	}
}
void effect_random_sparkle_flash (int iterations, int voxels, int delay){
	int i;
	int v;
	for (i = 0; i < iterations; i++){
		for (v=0;v<=voxels;v++) {
			setvoxel(rand()%8,rand()%8,rand()%8);
		}
		delay_ms(delay);
		fill(0x00);
	}
}

//EFFECT 14
void effect_path_text (int delay, char *str){
	int z, i,ii;
	z = 4;
	unsigned char path[28];
	font_getpath(0,path,28);
	
	unsigned char chr[5];
	unsigned char stripe;
	
	while (*str){
		//charfly(*str++, direction, axis, mode, delay);
		font_getchar(*str++, chr);
		
		for (ii=0;ii<5;ii++){
			//stripe = pgm_read_byte(&font[(chr*5)+ii]);
			stripe = chr[ii];
			
			for (z=0;z<8;z++){
				if ((stripe>>(7-z)) & 0x01) {
					setvoxel(0,7,z);
				}
				else {
					clrvoxel(0,7,z);
				}
			}
			effect_pathmove(path, 28);
			delay_ms(delay);
		}
		effect_pathmove(path, 28);
		delay_ms(delay);
	}
	for (i=0;i<28;i++){
		effect_pathmove(path, 28);
		delay_ms(delay);
	}
}

void effect_stringfly2(char * str)
{
	int x,y,i,ii;
	int state;
	
	unsigned char chr[5];
	
	while (*str)
	{
		font_getchar(*str++, chr);
		
		for (x = 0; x < 5; x++)
		{
			for (y = 0; y < 8; y++)
			{
				if ((chr[x] & (0x80>>y)))
				{
					setvoxel(7,x+2,y);
				}
			}
		}
		
		for (ii = 0; ii<6; ii++)
		{
			delay_ms(1500);
			for (i = 0; i < 7; i++)
			{
				for (x = 0; x < 8; x++)
				{
					for (y = 0; y < 8; y++)
					{
						state = getvoxel(i+1,x,y);
						altervoxel(i,x,y,state);
					}
				}
			}
			for (x = 0; x < 8; x++)
			{
				for (y = 0; y < 8; y++)
				{
					clrvoxel(7,x,y);
				}
			}
		}
	}
	for (ii = 0; ii<8; ii++)
	{
		delay_ms(1500);
		for (i = 0; i < 7; i++)
		{
			for (x = 0; x < 8; x++)
			{
				for (y = 0; y < 8; y++)
				{
					state = getvoxel(i+1,x,y);
					altervoxel(i,x,y,state);
				}
			}
		}
		for (x = 0; x < 8; x++)
		{
			for (y = 0; y < 8; y++)
			{
				clrvoxel(7,x,y);
			}
		}
	}
	
}

//FIREWORKS
void fireworks(int iterations, int delay, int explode_speed, uint8_t explode_range, uint8_t explode_density) {
	int8_t i,j,k,m=1,d;
	int8_t x,y,z,zz;
	
	for (i=0;i<iterations;i++) {
		x = rand()%4+2;	//two to five [x,y] = [2,2] to [5,5]. This makes fireworks inner in the cube
		y = rand()%4+2;
		z = rand()%5+3;	//floor from 3 to 7
		for (j=0; j<z; j++) {	//up to 5th floor;
			setvoxel(x, y, j);	//[x,y,z] = [3,3,0]
			delay_ms(delay);
			fill(0x00);
		}

		for(j=0;j<CUBE_SIZE;j++) {
			d=2;
			while(d){
			
				fill(0x00);
				
				if(m) m=0;
				else m=1;
				
				for(k=0;k<4;k++){
					zz=z;
					if(k==0){
						if(j==0||j==3){
							zz=z+1;
						}
						if(j==1||j==2){
							zz=z+2;
						}
						if(j>4){
							zz=z-(j-4);
						}
					}
					else if(k==1){
						if(j==1){
							zz=z+1;
						}
						if(j>2){
							zz=z-(j-2);
						}
					}
					else if(k==2){
						if(j>1){
							zz=z-(2*j-2);
						}
					}
					else{
						if(j>0){
							zz=z-(2*j);
						}
					}
					
					if(m==0){
						setvoxel(x-j,y,zz);
						
						setvoxel(x,y-j,zz);
						setvoxel(x+j,y-j,zz);
						setvoxel(x-j,y-j,zz);
						
					}
					else{
						setvoxel(x+j,y,zz);
					
						setvoxel(x,y+j,zz);
						
						
						setvoxel(x-j,y+j,zz);
						setvoxel(x+j,y+j,zz);
					}
					
					
					
				}
				d--;
				delay_ms(delay);
			}
		//	delay_ms(3000);
		}
	}
}

//BORDER BOX
void border_box(int delay, int iterations) {
	uint8_t i,j,t,a,s,round;
	t = CUBE_SIZE-1;
	a = iterations*4;
	s = 1;	//1 - fill, 0 - remove
	round = 1;	//1-fill style 1; 2- remove style 1; 3- fill style 2; 4- remove style 2;
	
	while (a>0) {
		for (i=0;i<3;i++) {
			for (j=0;j<=t;j++) {
				if(i==0) {
					if (s) {
						if(round%4 == 1) {
							setvoxel(j,0,0);	//x
							setvoxel(0,j,0);	//y
							setvoxel(0,0,j);	//z
						}
						else {
							setvoxel(t-j,t,t);	//x
							setvoxel(t,t-j,t);	//y
							setvoxel(t,t,t-j);	//z
						}
					}
					else {
						if(round%4 == 2) {
							clrvoxel(j,t,t);	//x
							clrvoxel(0,t-j,t);	//y
							clrvoxel(0,t,t-j);	//z
						}
						else {
							clrvoxel(t-j,0,0);	//x
							clrvoxel(t,j,0);	//y
							clrvoxel(t,0,j);	//z
						}
					}
				}
				else if(i==1) {
					if (s) {
						if(round%4 == 1) {
							setvoxel(j,t,0);	//x
							setvoxel(j,0,t);	//x
						
							setvoxel(t,j,0);	//y
							setvoxel(0,j,t);	//y
						
							setvoxel(0,t,j);	//z
							setvoxel(t,0,j);	//z
						}
						else {
							setvoxel(t-j,t,0);	//x
							setvoxel(t-j,0,t);	//x
						
							setvoxel(t,t-j,0);	//y
							setvoxel(0,t-j,t);	//y
						
							setvoxel(0,t,t-j);	//z
							setvoxel(t,0,t-j);	//z
						}
					}
					else {
						if(round%4 == 2) {
							clrvoxel(j,t,0);	//x
							clrvoxel(j,0,t);	//x
							
							clrvoxel(0,t-j,0);	//y
							clrvoxel(t,t-j,t);	//y
							
							clrvoxel(0,0,t-j);	//z
							clrvoxel(t,t,t-j);	//z
						}
						else {
							clrvoxel(t-j,t,0);	//x
							clrvoxel(t-j,0,t);	//x
							
							clrvoxel(0,j,0);	//y
							clrvoxel(t,j,t);	//y
							
							clrvoxel(0,0,j);	//z
							clrvoxel(t,t,j);	//z
						}
					}
				}
				else if(i==2) {
					if (s) {
						if(round%4 == 1) {
							setvoxel(j,t,t);	//x
							setvoxel(t,j,t);	//y
							setvoxel(t,t,j);	//z
						}
						else {
							setvoxel(t-j,0,0);	//x
							setvoxel(0,t-j,0);	//y
							setvoxel(0,0,t-j);	//z
						}
					}
					else {
						if(round%4 == 2) {
							clrvoxel(j,0,0);	//x
							clrvoxel(t,t-j,0);	//y
							clrvoxel(t,0,t-j);	//z
						}
						else {
							clrvoxel(t-j,t,t);	//x
							clrvoxel(0,j,t);	//y
							clrvoxel(0,t,j);	//z
						}
					}
				}			
				delay_ms(delay);
			}
			if (i==2) {		//last iteration
				s = s ? 0 : 1;
			}
		}
		round++;
		a--;		
	}
}

//MOVING BOXES
void moving_boxes(int delay,int iter){
	int i;
	int8_t j,s,x1,x2,x3,y1,y2,y3,z1,z2,z3,xx,yy,zz,cube=1,dir=1,cc=1,dd=1;
	uint8_t d0=0,d1=0,d2=0,d3=0,d4=0,d5=0;

	x1=0;	y1=0;	z1=0;
	x2=4;	y2=4;	z2=0;
	x3=0;	y3=4;	z3=4;

	s=CUBE_SIZE/2;
	fill(0x00);

	for(i=0;i<iter+1;i++){
		if(i>0){
			do cube=rand()%3;
			while(cc==cube);
			cc=cube;
			
			do dir=rand()%6;
			while(dd==dir);
			dd=dir;
			
			if(cube==0){		//first cube select
				xx=x1;
				yy=y1;
				zz=z1;
			}
			else if(cube==1){	//second cube select
				xx=x2;
				yy=y2;
				zz=z2;
			}
			else{				//third cube select
				xx=x3;
				yy=y3;
				zz=z3;
			}
			
			if		(dir==0 && !getvoxel(xx+s+1,yy,zz) && xx<s)	d0=1;
			else if	(dir==1 && !getvoxel(xx,yy-1,zz) && yy>0) 	d1=1;
			else if	(dir==2 && !getvoxel(xx-1,yy,zz) && xx>0) 	d2=1;
			else if	(dir==3 && !getvoxel(xx,yy+s+1,zz) && yy<s) 	d3=1;
			else if	(dir==4 && !getvoxel(xx,yy,zz-1) && zz>0) 	d4=1;
			else if	(dir==5 && !getvoxel(xx,yy,zz+s+1) && zz<s) 	d5=1;
			else {
				i--;
				continue;
			}
		}
		
		for(j=1;j<=s;j++){
			if(i>0){
				if(cube==0){
					if		(d0) x1=j;
					else if	(d1) y1=s-j;
					else if	(d2) x1=s-j;
					else if	(d3) y1=j;
					else if	(d4) z1=s-j;
					else		 z1=j;					
				}
				else if(cube==1){
					if		(d0) x2=j;
					else if	(d1) y2=s-j;
					else if	(d2) x2=s-j;
					else if	(d3) y2=j;
					else if	(d4) z2=s-j;
					else 		 z2=j;
				}
				else{
					if		(d0) x3=j;
					else if	(d1) y3=s-j;
					else if	(d2) x3=s-j;
					else if	(d3) y3=j;
					else if	(d4) z3=s-j;
					else 		 z3=j;
				}
			}		
			fill(0x00);
			filled_box_draw(x1,y1,z1,s,1);
			filled_box_draw(x2,y2,z2,s,1);
			filled_box_draw(x3,y3,z3,s,1);
			delay_ms(delay);
		}
		d0=d1=d2=d3=d4=d5=0;
		delay_ms(300);
	}
}
void filled_box_draw(int x,int y,int z,uint8_t size,uint8_t mode) {	//mode 0 - clear, mode 1 - fill
	uint8_t i,j,k;
	for(i=0;i<size;i++) {
		for(j=0;j<size;j++) {
			for(k=0;k<size;k++) {
				if(mode) setvoxel(x+k,y+j,z+i);
				else clrvoxel(x+k,y+j,z+i);
			}
		}
	}
}

//DIAGONAL TRIANGLE
void diagonal_triangle(int delay, int iterations) {
	int8_t i,j,xx,jj,yy,b,c=1,s;
	s=CUBE_SIZE-1;
	for(i=0;i<iterations;i++) {
		do b=rand()%8;
		while(b==c);
		c = b;
		for(j=0;j<=s*3;j++) {
			for(yy=0;yy<=j;yy++) {
				for(xx=j-yy,jj=0;xx>=0;xx--,jj++) {
					if(b==0) setvoxel(xx,yy,jj);
					else if(b==1) setvoxel(s-xx,s-yy,s-jj);
					else if(b==2) setvoxel(s-xx,yy,jj);
					else if(b==3) setvoxel(xx,s-yy,jj);
					else if(b==4) setvoxel(xx,yy,s-jj);
					else if(b==5) setvoxel(s-xx,yy,s-jj);
					else if(b==6) setvoxel(xx,s-yy,s-jj);
					else setvoxel(s-xx,s-yy,jj);					
				}
			}
			delay_ms(delay);
			fill(0x00);
		}
	}
}

//PLANES IN ORDER
void planes_in_order(int delay, int iterations) {
	int8_t i,j,k,b=1,c=1,m=0,s;
	s=CUBE_SIZE-1;
	for(i=0;i<iterations*2;i++) {
		m=m?0:1;
		if(m) {
			do b=rand()%3;
			while(b==c);
			c = b;
		}
		for(j=0;j<CUBE_SIZE;j++) {
			if(m){
				for(k=0;k<CUBE_SIZE-j;k++) {
					if(b==0){
						clrplane_x(k-1);
						setplane_x(k);
					}
					else if(b==1){
						clrplane_y(k-1);
						setplane_y(k);
					}
					else{
						clrplane_z(k-1);
						setplane_z(k);
					}
					delay_ms(delay);
				}
			}
			else {
				for(k=CUBE_SIZE-j-1;k<CUBE_SIZE;k++) {
					if(b==0){
						clrplane_x(k);
						setplane_x(k+1);
					}
					else if(b==1){
						clrplane_y(k);
						setplane_y(k+1);
					}
					else{
						clrplane_z(k);
						setplane_z(k+1);
					}
					delay_ms(delay);
				}
			}
		}
	}
}

//PLANES EXCHANGING
void planes_exchange(int delay, int iterations){
	int8_t i,j,a0=1,a1=2,a2=4,s,ss=1;
	
	for(i=0;i<3;i++){
		if		(i==0) 	a0=rand()%2;	//0 or 1
		else if	(i==1) 	a1=rand()%2+2;	//2 or 3
		else 			a2=rand()%2+4;	//4 or 5
	}
	
	if(a0==0) setplane_x(0);
	if(a0==1) setplane_x(CUBE_SIZE-1);
	if(a1==2) setplane_y(0);
	if(a1==3) setplane_y(CUBE_SIZE-1);
	if(a2==4) setplane_z(0);
	if(a2==5) setplane_z(CUBE_SIZE-1);
	
	for(i=0;i<iterations;i++){
		do s=rand()%3;
		while(s==ss);
		ss=s;

		for(j=1;j<CUBE_SIZE;j++){
			if(s==0){
				if(a0==0){
					clrplane_x(j-1);
					setplane_x(j);
					if(j==CUBE_SIZE-1) a0=1;
				}
				else{
					clrplane_x(CUBE_SIZE-j);
					setplane_x(CUBE_SIZE-j-1);
					if(j==CUBE_SIZE-1) a0=0;
				}
				
				if(a1==2) setplane_y(0);
				else setplane_y(CUBE_SIZE-1);
				
				if(a2==4) setplane_z(0);
				else setplane_z(CUBE_SIZE-1);
			}
			else if(s==1){
				if(a1==2){
					clrplane_y(j-1);
					setplane_y(j);
					if(j==CUBE_SIZE-1) a1=3;
				}
				else{
					clrplane_y(CUBE_SIZE-j);
					setplane_y(CUBE_SIZE-j-1);
					if(j==CUBE_SIZE-1) a1=2;
				}
				
				if(a0==0) setplane_x(0);
				else setplane_x(CUBE_SIZE-1);
				
				if(a2==4) setplane_z(0);
				else setplane_z(CUBE_SIZE-1);
			}
			else{
				if(a2==4){
					clrplane_z(j-1);
					setplane_z(j);
					if(j==CUBE_SIZE-1) a2=5;
				}
				else{
					clrplane_z(CUBE_SIZE-j);
					setplane_z(CUBE_SIZE-j-1);
					if(j==CUBE_SIZE-1) a2=4;
				}
				
				if(a0==0) setplane_x(0);
				else setplane_x(CUBE_SIZE-1);
				
				if(a1==2) setplane_y(0);
				else setplane_y(CUBE_SIZE-1);
			}
			delay_ms(delay);
		}
	}
}

//CRAZY STRAW
void crazy_straw(int delay, int iterations){
	int8_t 	i,j,p=0,pp=0,
			xStart=0,yStart=0,zStart=0,
			xEnd=0,yEnd=0,zEnd=0,
			xDest=0,yDest=0,zDest=0,
			xx,yy,zz;

	//start point & straw random generation
	if(rand()%2) xStart=0;
	else xStart=CUBE_SIZE-1;
	
	if(rand()%2) yStart=0;
	else yStart=CUBE_SIZE-1;
	
	if(rand()%2) zStart=0;
	else zStart=CUBE_SIZE-1;
	
	if(xStart>0) xEnd=0;
	else xEnd=CUBE_SIZE-1;
	yEnd=yStart;
	zEnd=zStart;
	
	for(i=0;i<iterations;i++){
		while(1){
			xx=xEnd;
			yy=yEnd;
			zz=zEnd;
			
			do p=rand()%3;
			while(pp==p);
			pp=p;
			
			if(p==0){
				if(xEnd==0) xx=CUBE_SIZE-1;
				else xx=0;
			}
			if(p==1){
				if(yEnd==0)	yy=CUBE_SIZE-1;
				else yy=0;
			}
			if(p==2){
				if(zEnd==0)	zz=CUBE_SIZE-1;
				else zz=0;
			}
			
			if((xx==xStart && yy==yStart && zz==zStart) || (xx==xEnd && yy==yEnd && zz==zEnd)) continue;
			else{
				xDest=xx;
				yDest=yy;
				zDest=zz;
				break;
			}	
		}
		
		for(j=0;j<CUBE_SIZE;j++){
			if(xDest!=xEnd){
				if(xEnd==0) xx=j;
				if(xEnd==CUBE_SIZE-1) xx=xEnd-j;
			}
			
			if(yDest!=yEnd){
				if(yEnd==0) yy=j;
				if(yEnd==CUBE_SIZE-1) yy=yEnd-j;
			}
			
			if(zDest!=zEnd){
				if(zEnd==0) zz=j;
				if(zEnd==CUBE_SIZE-1) zz=zEnd-j;
			}
			
			line(xStart,yStart,zStart,xx,yy,zz);
			delay_ms(delay);
			fill(0x00);
			
			if(j==CUBE_SIZE-1){
				xEnd=xStart;
				yEnd=yStart;
				zEnd=zStart;
				xStart=xx;
				yStart=yy;
				zStart=zz;
			}
		}
	}
}

void bomb(int delay, int iterations){
	int i;
	uint8_t j,x,y,r;
	
	for(i=0;i<iterations;i++){
		x = rand()%8;
		y = rand()%8;
		setvoxel(x,y,CUBE_SIZE-1);
		for(j=0;j<CUBE_SIZE;j++){
			delay_ms(delay);
			shift(AXIS_Z,-1);
		}
		for(r=1;r<=CUBE_SIZE;r++){
			drawCircle(x,y,0,r);
			delay_ms(delay);
			fill(0x00);
		}
		delay_ms(800);
	}
}

void walk_through_walls(int delay, int iterations){
	int i,j,c,k;
	
	for(i=0;i<iterations*2;i++){	
		for(c=0;c<CUBE_SIZE;c++){
			for(k=0;k<4;k++){
				for(j=0;j<CUBE_SIZE;j++){
					if(k==0){
						if(!(i%2)) setvoxel(j,0,c);
						else clrvoxel(j,0,c);
						delay_ms(delay);
					}
					else if(k==1){
						if(!(i%2))	setvoxel(CUBE_SIZE-1,j+1,c);
						else clrvoxel(CUBE_SIZE-1,j+1,c);
						delay_ms(delay);
						if(j==CUBE_SIZE-2) break;
					}
					else if(k==2){
						if(!(i%2)) setvoxel(CUBE_SIZE-2-j,CUBE_SIZE-1,c);
						else clrvoxel(CUBE_SIZE-2-j,CUBE_SIZE-1,c);
						delay_ms(delay);
						if(j==CUBE_SIZE-2) break;
					}
					else{
						if(!(i%2))	setvoxel(0,CUBE_SIZE-2-j,c);
						else clrvoxel(0,CUBE_SIZE-2-j,c);
						delay_ms(delay);
						if(j==CUBE_SIZE-3) break;
					}
				}
			}
		}
	}
}

void snake(int delay, int iterations) {
	uint8_t c, k, rnd, flag = 1;
	int8_t snake_length = (rand()%11 + 10) * 3, sum;	//make the length of snake between 10 and 20
	int i, j, snake_array[snake_length], temp_array[snake_length];	//, temp_array2[3];
	uint16_t cc;
	
	void drawSnake(int delay, int delay_off){
		fill(0x00);
		delay_ms(delay_off);
		//Draw the snake
		for(cc=0; cc<snake_length; cc+=3){
			setvoxel(snake_array[cc], snake_array[cc+1], snake_array[cc+2]);
		}
		delay_ms(delay);
	}
	
	int checkWay(int array[]){
		if(
			(inrange(array[0]+1, array[1], array[2]) && !getvoxel(array[0]+1, array[1], array[2])) ||
			(inrange(array[0]-1, array[1], array[2]) && !getvoxel(array[0]-1, array[1], array[2])) ||
			(inrange(array[0], array[1]+1, array[2]) && !getvoxel(array[0], array[1]+1, array[2])) ||
			(inrange(array[0], array[1]-1, array[2]) && !getvoxel(array[0], array[1]-1, array[2])) ||
			(inrange(array[0], array[1], array[2]+1) && !getvoxel(array[0], array[1], array[2]+1)) ||
			(inrange(array[0], array[1], array[2]-1) && !getvoxel(array[0], array[1], array[2]-1))
		){
			return 1;
		}
		else{
			return 0;
		}
	}

	void snakeFillAndMove(uint8_t action){
		if(checkWay(snake_array)){
			while(1){
				if(action == 1){
					temp_array[0] = snake_array[j-3];
					temp_array[1] = snake_array[j-2];
					temp_array[2] = snake_array[j-1];
				}
				else{
					temp_array[0] = snake_array[0];
					temp_array[1] = snake_array[1];
					temp_array[2] = snake_array[2];
				}
				//Generate random number in range 0..2
				rnd = rand()%3;
				//Sum 1 or -1
				if(rand()%2){
					sum = 1;
				}
				else{
					sum = -1;
				}
				sum += temp_array[rnd];
				
				if(sum >= 0 && sum < CUBE_SIZE){
					temp_array[rnd] = sum;
					//Check if the newly chosen LED is switched off
					if(!getvoxel(temp_array[0], temp_array[1], temp_array[2])){
						if(action == 1){
							snake_array[j] = temp_array[0];
							snake_array[j+1] = temp_array[1];
							snake_array[j+2] = temp_array[2];
						}
						else{
							for(c=0; c<3; c++){
								//Shift right 3 times
								for(k=snake_length-1; k>0; k--){
									snake_array[k] = snake_array[k-1]; 
								}
								snake_array[0] = -1;
							}
							for(c=0;c<3;c++){
								snake_array[c] = temp_array[c];
							}
						}
						break;
					}
				}				
			}
		}
		else{
			drawSnake(2000, 2000);
			flag = 0;	
		}
	}
		
	for(i=0;i<iterations && flag;i++){
		if(i){
			snakeFillAndMove(2);
		}
		else{
			//WORKS FOR LENGTH 8+ or less, random creation
			for(j=0; j<snake_length && flag; j+=3){
				if(j==0){
					for(k=0;k<3;k++){
						snake_array[k] = rand() % CUBE_SIZE;	//In my case, 8
					}
				}
				else{
					snakeFillAndMove(1);	//action = 1 means fill the array, while any other number means shift the array
				}
			}
		}
		
		drawSnake(delay, 0);
	}
}