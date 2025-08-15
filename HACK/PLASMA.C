// Plasmatic by Matt Young
// UQCS Hackathon 2025
// Programmed on FreeDOS
#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <math.h>

#define X_RES 320
#define Y_RES 200

int main(void) {
    	int y;
	int x;
	int time = 0;
	int kb_c;

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
			    _setcolor(time % 256);
			    _setpixel(x, y);
			}
	    }
	    time++;
	}

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("Demo done\n");
        return 0;
}
