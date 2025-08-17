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

#include "GREETZ.INC"

#define SIN_TAB_SIZE 512
static float SinTab[SIN_TAB_SIZE];
static float CosTab[SIN_TAB_SIZE];

// source: SignalWhisperer on GitHub Gists
static inline void init_sin_tab() {
	int i;

	float initial_phase = 0;
	float dphase = 2.f * PI / SIN_TAB_SIZE;

	for (i = 0; i < SIN_TAB_SIZE; i++) {
		SinTab[i] = sin(initial_phase + dphase * i);
		CosTab[i] = cos(initial_phase + dphase * i);
	}
}

static inline float approxsin(float phase) {
	phase = fmod(phase, 2 * PI);
	if (phase < 0)
		phase += 2 * PI;
	phase /= 2 * PI;
	return SinTab[(int) (phase * SIN_TAB_SIZE)];
}

static inline float approxcos(float phase) {
	phase = fmod(phase, 2 * PI);
	if (phase < 0)
		phase += 2 * PI;
	phase /= 2 * PI;
	return CosTab[(int) (phase * SIN_TAB_SIZE)];
}

// source: https://github.com/czinn/perlin

static inline float rawnoise(int n) {
	n = (n << 13) ^ n;
	return (1.0
		- ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)
			/ 1073741824.0);
}

static inline float noise3d(int x, int y, int z, int octave, int seed) {
	return rawnoise(
		x * 1919 + y * 31337 + z * 7669 + octave * 3463 + seed * 13397);
}

static inline float interpolate(float a, float b, float x) {
	float f = (1.f - approxcos(x * PI)) * 0.5f;

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
	{ 0, 8, 2, 10},
	{12, 4, 14, 6},
	{3, 11, 1, 9},
	{15, 7, 13, 5},
};

static int BAYER16x16[16][16] = {
	{0, 128, 32, 160, 8, 136, 40, 168, 2, 130, 34, 162, 10, 138, 42, 170},
	{192, 64, 224, 96, 200, 72, 232, 104, 194, 66, 226, 98, 202, 74, 234, 106},
	{48, 176, 16, 144, 56, 184, 24, 152, 50, 178, 18, 146, 58, 186, 26, 154},
	{240, 112, 208, 80, 248, 120, 216, 88, 242, 114, 210, 82, 250, 122, 218, 90},
	{12, 140, 44, 172, 4, 132, 36, 164, 14, 142, 46, 174, 6, 134, 38, 166},
	{204, 76, 236, 108, 196, 68, 228, 100, 206, 78, 238, 110, 198, 70, 230, 102},
	{60, 188, 28, 156, 52, 180, 20, 148, 62, 190, 30, 158, 54, 182, 22, 150},
	{252, 124, 220, 92, 244, 116, 212, 84, 254, 126, 222, 94, 246, 118, 214, 86},
	{3, 131, 35, 163, 11, 139, 43, 171, 1, 129, 33, 161, 9, 137, 41, 169},
	{195, 67, 227, 99, 203, 75, 235, 107, 193, 65, 225, 97, 201, 73, 233, 105},
	{51, 179, 19, 147, 59, 187, 27, 155, 49, 177, 17, 145, 57, 185, 25, 153},
	{243, 115, 211, 83, 251, 123, 219, 91, 241, 113, 209, 81, 249, 121, 217, 89},
	{15, 143, 47, 175, 7, 135, 39, 167, 13, 141, 45, 173, 5, 133, 37, 165},
	{207, 79, 239, 111, 199, 71, 231, 103, 205, 77, 237, 109, 197, 69, 229, 101},
	{63, 191, 31, 159, 55, 183, 23, 151, 61, 189, 29, 157, 53, 181, 21, 149},
	{255, 127, 223, 95, 247, 119, 215, 87, 253, 125, 221, 93, 245, 117, 213, 85}
};

int main(void) {
	int y;
	int x;
	int demotime;
	int kb_c;
	int i;
	int seed;
	int automode;

	// dithering
	char oldpix;
	char newpix;
	char quant_error;
	float bayerf;
	int dlevel;
	float dfrac;
	float dthresh;

	// text stuff
	int mask_idx;
	char pix; // pixel in image
	char show_greetz = 0;

	// greetz stuff
	int greetz_offset;
	int greetz_y = 0;
	int is_showing_greetz;
	int auto_greetz_ctr;

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
	demotime = 0;
	automode = 0;
	is_showing_greetz = 0;
	auto_greetz_ctr = 0;

	srand(time(NULL));
	seed = rand();
	init_sin_tab();

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
					is_showing_greetz = 1;
				} else {
					show_greetz = 0;
					is_showing_greetz = 0;
				}
				// wait til next frame
				continue;
			} // END IF

			if (kb_c == 'i') {
				automode = !automode;
			}
		} // END KBHIT

		// greetz blitting
		if (show_greetz == 1) {
			// wiggle the text up and down a bit
			greetz_y = (int) (approxsin(demotime / 10.f) * 15.f);
			// visit each pixel in the display area
			// then lookup the texel *for* that display pixel
			for (y = 32; y < Y_RES - 32; y++) {
				for (x = 32; x < X_RES - 32; x++) {
					// translate from our screen coords
					// to texel coords
					// and apply the scroll
					// note that we add a little extra black space
					// at the end - this is handled for us by
					// get_pixel
					pix = get_pixel(
						(x - 32 + greetz_offset) % (GREETZ_WIDTH + 128),
						 y - 32 - greetz_y);
					MASK(x, y, pix ? 255 : 0);
				} // END INNER FOR
			} // END OUTER FOR

			greetz_offset += 5;
			if (greetz_offset > GREETZ_WIDTH) {
				greetz_offset = 0;

				// once we've finished greetz in automode, hide them
				if (automode) {
					is_showing_greetz = 0;
					show_greetz = 0;
				}
			}
		} // END IF GREETZ
		else {
			// no greetz so just clear the mask
			_fmemset(mask, 255, X_RES * Y_RES);
		}

		// draw to main buffer
		for (y = 32; y < Y_RES - 32; y++) {
			for (x = 32; x < X_RES - 32; x++) {
				float noisef = (pnoise3d(x * 0.005, y * 0.005,
									demotime * 0.005,
									.7, 1, seed));
				// this was an attempt to coerce it into the
				// pretty colours
				//noise = (noise + 0x20F) % 0xFF;

				// apply bayer dithering
				dlevel = (int)(noisef * 0xFF);
				dfrac = noisef * 255 - dlevel;
				dthresh = (BAYER4x4[y % 4][x % 4] + 0.5f) * (1.f/16.f);
				if (dfrac > dthresh && dlevel < 255) {
					dlevel++;
				}

				if ((GETMASK(x,y)) == 0) {
					// don't draw
					DRAW(x, y, 0);
				} else {
					// we're ok to draw noise
					// translate to VGA palette
					DRAW(x, y, dlevel);
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

		// every some number ticks, in auto mode, show greetz
		// if we're not currently showing it
		if (!is_showing_greetz && !show_greetz) {
			auto_greetz_ctr++;
			if (auto_greetz_ctr >= 2000 && automode) {
				auto_greetz_ctr = 0;
				show_greetz = 1;
			}
		}
		/*if (demotime % 1000 == 0 && automode && !is_showing_greetz) {
			show_greetz = 1;
		}*/
		demotime++;
	} // END WHILE

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("PLASMATIC - ML YOUNG 2025\n");
	printf("thanks for watching :3\n");
	return 0;
} // END MAIN
