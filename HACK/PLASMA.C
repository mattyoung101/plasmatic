/* Plasmatic by Matt Young */
/* UQCS Hackathon 2025 */
/* Programmed on FreeDOS */

#include <stdio.h>
#include <graph.h>
#include <conio.h>

#define X_RES 640
#define Y_RES 480

int main(void) {
    	// 640x480; 256 colour mode
    	_setvideomode(_VRES256COLOR);

	_setcolor(14); // yellow?
	_rectangle(_GFILLINTERIOR, 0, 0, X_RES-1, Y_RES-1);
	getch();

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("Demo done\n");
        return 0;
}
