// Plasmatic by Matt Young
// UQCS Hackathon 2025
// Programmed on FreeDOS
#include <conio.h>
#include <dos.h>
#include <graph.h>
#include <malloc.h>
#include <math.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// brought to you by Howie
// close enough(TM)
// brought to you by Yufeng 16/08/25
// "verify if it's correct, it's probably not"
// checked approx 5 minutes later by Matt
// and it's correct
// chat he's goated
// brought to you AGAIN by bpaul omg :O
// chat they're goated?!
#define PI 3.14159265358979323846264338327950288197169399375105820974

#define X_RES 320
#define Y_RES 200

// VGA stuff, credit root42
#define SET_MODE 0x0
#define VIDEO_INT 0x10
#define INPUT_STATUS 0x03DA
#define VRETRACE 0x08
char far *const VGA = (char far *) 0xA0000000L;

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// Image size: 4052x113, total pixels: 457876
#define GREETZ_WIDTH 4052
#define GREETZ_HEIGHT 113
#include "GREETZ.INC"

// source: libgdx math utils
#define SIN_BITS (8)
#define SIN_MASK (~(-1 << SIN_BITS)) // 255 entries
#define SIN_COUNT (SIN_MASK + 1)

#define RAD_FULL (PI * 2)
#define RAD2INDEX (SIN_COUNT / RAD_FULL)

static float *SinTab;

static inline void init_sin_tab() {
	int i = 0;
	float radFull;

	SinTab = malloc(SIN_COUNT * sizeof(float));
	for (i = 0; i < SIN_COUNT; i++) {
		SinTab[i] = (float) sin((i + 0.5f) / SIN_COUNT * radFull);
	}
}

#define HALF_PI (PI/2)

#define SIN(x) (SinTab[(int)(x * RAD2INDEX) & SIN_MASK])
//#define COS(x) (SinTab[(int)((x + (PI/2)) * RAD2INDEX) & SIN_MASK])
#define COS(x) (SinTab[(int)((x + HALF_PI) * RAD2INDEX) & SIN_MASK])

// source: https://github.com/czinn/perlin

static inline float rawnoise(int n) {
	n = (n << 13) ^ n;
	return (1.0
		- ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)
			/ 1073741824.0);
}

static inline float noise1d(int x, int octave, int seed) {
	return rawnoise(x * 1619 + octave * 3463 + seed * 13397);
}

static inline float noise2d(int x, int y, int octave, int seed) {
	return rawnoise(x * 1619 + y * 31337 + octave * 3463 + seed * 13397);
}

static inline float noise3d(int x, int y, int z, int octave, int seed) {
	return rawnoise(
		x * 1919 + y * 31337 + z * 7669 + octave * 3463 + seed * 13397);
}

static inline float interpolate(float a, float b, float x) {
	float f = (1.f - cos(x * PI)) * 0.5f;

	return a * (1.f - f) + b * f;
}

static inline float smooth3d(
	float x, float y, float z, int octave, int seed) {
	int intx = (int) x;
	float fracx = x - intx;
	int inty = (int) y;
	float fracy = y - inty;
	int intz = (int) z;
	float fracz = z - intz;

	float v1 = noise3d(intx, inty, intz, octave, seed);
	float v2 = noise3d(intx + 1, inty, intz, octave, seed);
	float v3 = noise3d(intx, inty + 1, intz, octave, seed);
	float v4 = noise3d(intx + 1, inty + 1, intz, octave, seed);
	float v5 = noise3d(intx, inty, intz + 1, octave, seed);
	float v6 = noise3d(intx + 1, inty, intz + 1, octave, seed);
	float v7 = noise3d(intx, inty + 1, intz + 1, octave, seed);
	float v8 = noise3d(intx + 1, inty + 1, intz + 1, octave, seed);

	float i1 = interpolate(v1, v2, fracx);
	float i2 = interpolate(v3, v4, fracx);
	float i3 = interpolate(v5, v6, fracx);
	float i4 = interpolate(v7, v8, fracx);

	float j1 = interpolate(i1, i2, fracy);
	float j2 = interpolate(i3, i4, fracy);

	return interpolate(j1, j2, fracz);
}

static inline float pnoise3d(
	float x, float y, float z, float persistence, int octaves, int seed) {
	float total = 0.0;
	float frequency = 1.0;
	float amplitude = 1.0;
	int i = 0;

	for (i = 0; i < octaves; i++) {
		total += smooth3d(x * frequency, y * frequency, z * frequency, i, seed)
			* amplitude;
		frequency /= 2;
		amplitude *= persistence;
	}

	return total;
}

static inline char get_pixel(int px, int py) {
	unsigned long long pixel_index, word_index, bit_index, word;

	// 1. Bounds check
	if (px < 0 || px >= GREETZ_WIDTH || py < 0 || py >= GREETZ_HEIGHT)
		return 0; // error / out of bounds

	// 2. Linear pixel index (row-major)
	pixel_index = (unsigned long long) py * (unsigned long long) GREETZ_WIDTH
		+ (unsigned long long) px;

	// 3. Which word?
	word_index = pixel_index / 64ULL;

	// 4. Which bit in that word?
	bit_index = (int) (pixel_index % 64ULL);

	// 5. Extract the bit
	word = IMAGE[word_index];
	return (char) ((word >> bit_index) & 1ULL);
}

#define DRAW(x, y, c) *(framebuf + (unsigned long) X_RES * (y) + (x)) = c
#define GET(x, y) *(framebuf + (unsigned long) X_RES * (y) + (x))
#define MASK(x, y, c) *(mask + (unsigned long) X_RES * (y) + (x)) = c
#define GETMASK(x, y) *(mask + (unsigned long) X_RES * (y) + (x))

// source: wikipedia,
static int BAYER4x4[4][4] = {
	/*{ 0, 8, 2, 10},
	{12, 4, 14, 6},
	{3, 11, 1, 9},
	{15, 7, 13, 5},*/

	{15,195,60,240},
	{135,75,180,120},
	{45,225,30,210},
	{165,105,150,90},
};

int main(void) {
	int y;
	int x;
	int time = 0;
	int kb_c;
	int i;
	int seed;

	// dithering
	char oldpix;
	char newpix;
	char quant_error;

	// text stuff
	int mask_idx;
	char pix; // pixel in image
	char show_greetz = 0;

	// greetz stuff
	int greetz_offset;

	char far *framebuf;
	char far *mask;

	// allocate framebuffer
	// framebuf = malloc((Y_RES - 32 - 32) * (X_RES - 32 - 32));
	framebuf = _fmalloc(X_RES * Y_RES);
	_fmemset(framebuf, 0, X_RES * Y_RES);

	mask = _fmalloc(X_RES * Y_RES);
	_fmemset(mask, 0, X_RES * Y_RES);

	// 320x200; 256 colour mode
	_setvideomode(_MRES256COLOR);

	greetz_offset = 0;

	init_sin_tab();
	seed = rand();

	while (1) {
		if (kbhit()) {
			kb_c = getch();
			// 27 == ESC
			if (kb_c == 27)
				break;

			// 104 == 'H'
			if (kb_c == 104) {
				if (show_greetz == 0) {
					show_greetz = 1;
				} else {
					show_greetz = 0;
				}
				// wait til next frame
				continue;
			} // END IF
		} // END KBHIT

		// greetz blitting
		if (show_greetz == 1) {
			// lookup the texel
			for (y = 32; y < Y_RES - 32; y++) {
				for (x = 32; x < X_RES - 32; x++) {
					// translate from our screen coords
					// to texel coords
					// and apply the scroll
					pix = get_pixel((x - 32 + greetz_offset) % GREETZ_WIDTH,
						 y - 32);
					MASK(x, y, pix ? 255 : 0);
				} // END INNER FOR
			} // END OUTER FOR

			greetz_offset += 5;
			if (greetz_offset > GREETZ_WIDTH) {
				greetz_offset = 0;
			}
		} // END IF GREETZ
		else {
			// no greetz so just clear the mask
			_fmemset(mask, 255, X_RES * Y_RES);
		}

		// draw to main buffer
		for (y = 32; y < Y_RES - 32; y++) {
			for (x = 32; x < X_RES - 32; x++) {
				char noise = (char) (pnoise3d(x * 0.005, y * 0.005,
									time * 0.005,
									.7, 1, 12124)
					* 0xFF);
				// this was an attempt to coerce it into the
				// pretty colours
				//noise = (noise + 0x20F) % 0xFF;

				if ((GETMASK(x,y)) == 0) {
					// don't draw
					DRAW(x, y, 0);
				} else {
					// we're ok to draw noise
					DRAW(x, y, noise);
				}
			} // END INNER FOR
		} // END OUTER FOR

		// apply bayer dithering
		/*for (y = 32; y < Y_RES - 32; y++) {
			for (x = 32; x < X_RES - 32; x++) {
			} // END INNER FOR
		} // END OUTER FOR*/

		// wait for vblank
		while (inp(INPUT_STATUS) & VRETRACE)
			;
		while (!inp(INPUT_STATUS) & VRETRACE)
			;

		// copy - making sure we use the far ptr version
		// this will blit it to the screen
		_fmemcpy(VGA, framebuf, X_RES * Y_RES);

		// also toggle text mode every few frames
		/*if (time % 64 == 0) {
			show_greetz = !show_greetz;
		}*/

		time++;
	} // END WHILE

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("PLASMATIC - M YOUNG 2025\n");
	printf("thanks for watching :3\n");
	return 0;
} // END MAIN
