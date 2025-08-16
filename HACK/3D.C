#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} point_t;

// source: computer graphics from scratch
// NB float *out must be of size ceil(i1)
static inline void
interpolate(float i0, float d0, float i1, float d1, float *out)
{
	float i = 0;
	int idx = 0;
	float a, d;

	if (i0 == i1) {
	    out[0] = d0;
	    return;
	}

	a = (d1 - d0) / (i1 - i0);
	d = d0;
	for (i = i0; i <= i1; i++) {
		out[idx++] = d;
		d += a;
	}
}

static inline void
line(point_t p0, point_t p1, short colour)
{

}

int main(void) {
    int y;
    int x;
    int kb_c;

    // 320x200; 256 colour mode
    _setvideomode(_MRES256COLOR);

    while (1) {
	if (kbhit()) {
		kb_c = getch();
		// 27 = ESC
		if (kb_c == 27) break;
	}
    }

    _setvideomode(_DEFAULTMODE);

    return 0;
}
