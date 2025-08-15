// Plasmatic by Matt Young
// UQCS Hackathon 2025
// Programmed on FreeDOS
#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <math.h>

// brought to you by Howie
// close enough(TM)
#define PI 3.1415926

#define X_RES 320
#define Y_RES 200

// source: libgdx
static inline float
map(float inRStart, float inREnd, float outRStart,
	float outREnd, float value) {
	return outRStart + (value - inRStart) * (outREnd - outRStart)
		/ (inREnd - inRStart);
}

int main(void) {
    int y;
	int x;
	int time = 0;
	int kb_c;

	float scale_x = 1.0;
	float scale_y = 1.0;

	float c_x, c_y, v;

   	 // 320x200; 256 colour mode
    	_setvideomode(_MRES256COLOR);

	srand(0);
	while (1) {
		if (kbhit()) {
			kb_c = getch();
			// 27 == ESC
			if (kb_c == 27)
				break;
	    	}

	    	for (y = 0; y < Y_RES; y++) {
				for (x = 0; x < X_RES; x++) {
					c_x = x * scale_x - scale_x/2.0;
					c_y = y * scale_y - scale_y/2.0;
					v = 0.0;

					v += sin((c_x + time));
					v += sin((c_y + time)/2.0);
					v += sin((c_x + c_y + time)/2.0);

					c_x += scale_x/2.0 * sin(time/2.0);
					c_y += scale_y/2.0 * cos(time/2.0);

					v += sin(sqrt(c_x*c_x + c_y*c_y + 1.0) + time);
					v /= 2.0;

					_setcolor((int) v * 255);
					_setpixel(x ,y);


					//int r = 1;
					//int g = sin(PI*v);
					//int b = cos(PI*v);

			    	//_setcolor(time % 256);
			    	//_setpixel(x, y);
				}
	    	}
	    	time++;
	}

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("Demo done\n");
        return 0;
}
