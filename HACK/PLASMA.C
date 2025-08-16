// Plasmatic by Matt Young
// UQCS Hackathon 2025
// Programmed on FreeDOS
// GREETZ TO:
// broken_pipe, bpaul, eerie!!, devilbunny, howie
#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <math.h>

// brought to you by Howie
// close enough(TM)
// brought to you by Yufeng 16/08/25
// "verify if it's correct, it's probably not"
// checked approx 5 minutes later by Matt
// and it's correct
// chat he's goated
#define PI 3.1415926535897932384626

#define X_RES 320
#define Y_RES 200

// source: https://github.com/czinn/perlin

static inline double rawnoise(int n) {
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

static inline double noise1d(int x, int octave, int seed) {
    return rawnoise(x * 1619 + octave * 3463 + seed * 13397);
}

static inline double noise2d(int x, int y, int octave, int seed) {
    return rawnoise(x * 1619 + y * 31337 + octave * 3463 + seed * 13397);
}

static inline double noise3d(int x, int y, int z, int octave, int seed) {
    return rawnoise(x * 1919 + y * 31337 + z * 7669 + octave * 3463 + seed * 13397);
}

static inline double interpolate(double a, double b, double x) {
    double f = (1 - cos(x * 3.141593)) * 0.5;

    return a * (1 - f) + b * f;
}

static inline double smooth1d(double x, int octave, int seed) {
    int intx = (int)x;
    double fracx = x - intx;

    double v1 = noise1d(intx, octave, seed);
    double v2 = noise1d(intx + 1, octave, seed);

    return interpolate(v1, v2, fracx);
}

static inline double smooth2d(double x, double y, int octave, int seed) {
    int intx = (int)x;
    double fracx = x - intx;
    int inty = (int)y;
    double fracy = y - inty;

    double v1 = noise2d(intx, inty, octave, seed);
    double v2 = noise2d(intx + 1, inty, octave, seed);
    double v3 = noise2d(intx, inty + 1, octave, seed);
    double v4 = noise2d(intx + 1, inty + 1, octave, seed);

    double i1 = interpolate(v1, v2, fracx);
    double i2 = interpolate(v3, v4, fracx);

    return interpolate(i1, i2, fracy);
}

static inline double smooth3d(double x, double y, double z, int octave, int seed) {
    int intx = (int)x;
    double fracx = x - intx;
    int inty = (int)y;
    double fracy = y - inty;
    int intz = (int)z;
    double fracz = z - intz;


    double v1 = noise3d(intx, inty, intz, octave, seed);
    double v2 = noise3d(intx + 1, inty, intz, octave, seed);
    double v3 = noise3d(intx, inty + 1, intz, octave, seed);
    double v4 = noise3d(intx + 1, inty + 1, intz, octave, seed);
    double v5 = noise3d(intx, inty, intz + 1, octave, seed);
    double v6 = noise3d(intx + 1, inty, intz + 1, octave, seed);
    double v7 = noise3d(intx, inty + 1, intz + 1, octave, seed);
    double v8 = noise3d(intx + 1, inty + 1, intz + 1, octave, seed);

    double i1 = interpolate(v1, v2, fracx);
    double i2 = interpolate(v3, v4, fracx);
    double i3 = interpolate(v5, v6, fracx);
    double i4 = interpolate(v7, v8, fracx);

    double j1 = interpolate(i1, i2, fracy);
    double j2 = interpolate(i3, i4, fracy);

    return interpolate(j1, j2, fracz);
}

static inline double pnoise1d(double x, double persistence, int octaves, int seed) {
   double total = 0.0;
   double frequency = 1.0;
   double amplitude = 1.0;
   int i = 0;

   for(i = 0; i < octaves; i++) {
       total += smooth1d(x * frequency, i, seed) * amplitude;
       frequency /= 2;
       amplitude *= persistence;
   }

   return total;
}

static inline double pnoise2d(double x, double y, double persistence, int octaves, int seed) {
   double total = 0.0;
   double frequency = 1.0;
   double amplitude = 1.0;
   int i = 0;

   for(i = 0; i < octaves; i++) {
       total += smooth2d(x * frequency, y * frequency, i, seed) * amplitude;
       frequency /= 2;
       amplitude *= persistence;
   }

   return total;
}

static inline double pnoise3d(double x, double y, double z, double persistence, int octaves, int seed) {
   double total = 0.0;
   double frequency = 1.0;
   double amplitude = 1.0;
   int i = 0;

   for(i = 0; i < octaves; i++) {
       total += smooth3d(x * frequency, y * frequency, z * frequency, i, seed) * amplitude;
       frequency /= 2;
       amplitude *= persistence;
   }

   return total;
}


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

	    	for (y = 32; y < Y_RES - 32; y++) {
				for (x = 32; x < X_RES - 32; x++) {
					int noise = (int)
						(pnoise3d(x * 0.01, y * 0.01, time * 0.01,
						0.7, 5, 12124) * 255.);
					_setcolor(noise);
					_setpixel(x, y);
				}
	    	}
	    	time++;
	}

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("Demo done!!\n");
    return 0;
}
