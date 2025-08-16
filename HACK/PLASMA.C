// Plasmatic by Matt Young
// UQCS Hackathon 2025
// Programmed on FreeDOS
// GREETZ TO:
// broken_pipe, bpaul, eerie!!, devilbunny, howie
#include <conio.h>
#include <dos.h>
#include <graph.h>
#include <malloc.h>
#include <math.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>

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

// clang-format off
// Image size: 256x136, total pixels: 34816
static const unsigned long long hack_image[544] = {
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000002fffc00000ULL,     0x0000000000000000ULL,
    0x003fc00000001ff0ULL,     0x00000001ff000000ULL,     0xf80003fffffe0000ULL,     0x1ffc03ff8000000fULL,
    0x007fc00000000ff0ULL,     0x00000001ff000000ULL,     0xf8001fffffff0000ULL,     0x0ffc01ff80000007ULL,
    0x003fe00000000ff0ULL,     0x00000001ff800000ULL,     0xf8007fffffffe000ULL,     0x0ffc00ffc0000007ULL,
    0x003fc00000001ff0ULL,     0x00000003ff800000ULL,     0xf8007ffffffff800ULL,     0x0ff8007fe000000fULL,
    0x007fc00000000ff0ULL,     0x00000003ff800000ULL,     0xf8003ffffffffc00ULL,     0x0ffc003ff0000007ULL,
    0x003fc00000001ff0ULL,     0x00000007ffc00000ULL,     0xf8003ffffffffe00ULL,     0x0ff8001ff800000fULL,
    0x003fe00000000ff0ULL,     0x00000007ffc00000ULL,     0xf8003ff800bfff80ULL,     0x0ffc000ffc000007ULL,
    0x007fc00000001ff0ULL,     0x00000007ffc00000ULL,     0xf8001f80000fff80ULL,     0x0ff80007fc00000fULL,
    0x003fc00000000ff0ULL,     0x0000000fffe00000ULL,     0xf8000c000003ffe0ULL,     0x0ff80003ff000007ULL,
    0x003fc00000001ff0ULL,     0x0000000fffe00000ULL,     0xf80000000000ffe0ULL,     0x0ff80003ff00000fULL,
    0x003fc00000000ff0ULL,     0x0000000feff00000ULL,     0xf800000000007ff0ULL,     0x0ff80000ff800007ULL,
    0x007fc00000001ff0ULL,     0x0000001feff00000ULL,     0xf800000000003ff8ULL,     0x0ff80000ffc0000fULL,
    0x003fe00000000ff0ULL,     0x0000001fe7f80000ULL,     0xf800000000001ffcULL,     0x0ff800007fe00007ULL,
    0x003fc00000001ff0ULL,     0x0000003fc7f80000ULL,     0xf800000000000ffcULL,     0x0ff800003ff0000fULL,
    0x007fc00000000ff0ULL,     0x0000003fc7f80000ULL,     0xf8000000000007feULL,     0x0ff800001ff80007ULL,
    0x003fc00000001ff0ULL,     0x0000007fc7f80000ULL,     0xf8000000000007feULL,     0x0ff800000ffc000fULL,
    0x003fc00000000ff0ULL,     0x0000003f83fc0000ULL,     0xf8000000000003ffULL,     0x07f8000007fe0007ULL,
    0x007fc00000001ff0ULL,     0x0000007f83fc0000ULL,     0xf8000000000003ffULL,     0x0ff8000003fe000fULL,
    0x003fe00000000ff0ULL,     0x000000ff81fe0000ULL,     0xf8000000000001ffULL,     0x07f8000001ff8007ULL,
    0x003fc00000001ff0ULL,     0x800000ff01fe0000ULL,     0xf8000000000001ffULL,     0x0ff8000001ff800fULL,
    0x003fc00000000ff0ULL,     0x800000ff01ff0000ULL,     0xf8000000000000ffULL,     0x07f80000007fc007ULL,
    0x007fc00000001ff0ULL,     0x800001ff01ff0000ULL,     0xf8000000000000ffULL,     0x0ff00000007fe00fULL,
    0x003fc00000000ff0ULL,     0xc00001fe00ff8000ULL,     0xf8000000000000ffULL,     0x07f80000001ff007ULL,
    0x003fc00000001ff0ULL,     0x800003fe00ff0000ULL,     0xf8000000000000ffULL,     0x07f00000001ff80fULL,
    0x007fe00000000ff0ULL,     0xc00003fe007f8000ULL,     0xf80000000000007fULL,     0x07f80000000ffc07ULL,
    0x003fc00000001ff0ULL,     0xc00003fc007fc000ULL,     0xf80000000000007fULL,     0x07f000000007fe0fULL,
    0x003fc00000000ff0ULL,     0xc00007fc003fc000ULL,     0xf80000000000007fULL,     0x07f800000003ff07ULL,
    0x007ffffffffffff0ULL,     0xc00007f8003fc000ULL,     0xf80000000000007fULL,     0x07f000000001ff0fULL,
    0x003ffffffffffff0ULL,     0xe0000ff8003fe000ULL,     0xf80000000000007fULL,     0x07f000000000ffc7ULL,
    0x003ffffffffffff0ULL,     0xc0000ff8001fe000ULL,     0xf80000000000007fULL,     0x07f000000001ffcfULL,
    0x003ffffffffffff0ULL,     0xe0000ff0001ff000ULL,     0xf80000000000003fULL,     0x07f000000001ffe7ULL,
    0x007ffffffffffff0ULL,     0xc0001ff0001ff000ULL,     0xf80000000000007fULL,     0x07f000000003ffffULL,
    0x003ffffffffffff0ULL,     0xe0001fe0000ff000ULL,     0xf80000000000003fULL,     0x07f000000007ffffULL,
    0x003ffffffffffff0ULL,     0xc0003fe0000ff800ULL,     0xf80000000000007fULL,     0x07f00000000fffffULL,
    0x007fc00000000ff0ULL,     0xe0003fe00007f800ULL,     0xf80000000000003fULL,     0x07f00000000fffffULL,
    0x003fc00000001ff0ULL,     0xc0003fc00007f800ULL,     0xf80000000000007fULL,     0x07f00000001ff8ffULL,
    0x003fc00000000ff0ULL,     0xe0007fc00007fc00ULL,     0xf80000000000007fULL,     0x03f00000003ff87fULL,
    0x007fc00000001ff0ULL,     0xc0007f800003fc00ULL,     0xf80000000000007fULL,     0x07f00000007ff07fULL,
    0x003fc00000000ff0ULL,     0xc0007ffffffffe00ULL,     0xf80000000000007fULL,     0x03f00000007fe01fULL,
    0x003fc00000001ff0ULL,     0xc000fffffffffe00ULL,     0xf80000000000007fULL,     0x07f0000000ffc00fULL,
    0x003fe00000000ff0ULL,     0xc000ffffffffff00ULL,     0xf80000000000007fULL,     0x03f0000001ffc007ULL,
    0x007fc00000001ff0ULL,     0xc001ffffffffff00ULL,     0xf8000000000000ffULL,     0x07e0000003ff800fULL,
    0x003fc00000000ff0ULL,     0x8001ffffffffff00ULL,     0xf80000000000007fULL,     0x03f0000003ff0007ULL,
    0x003fc00000001ff0ULL,     0x8003ffffffffff80ULL,     0xf8000000000000ffULL,     0x03e0000007fe000fULL,
    0x007fc00000000ff0ULL,     0x8001ffffffffff80ULL,     0xf8000000000000ffULL,     0x03f000000ffe0007ULL,
    0x003fc00000001ff0ULL,     0x8003fe0000007f80ULL,     0xf8000000000001ffULL,     0x03e000001ffc000fULL,
    0x003fe00000000ff0ULL,     0x8007fc0000007fc0ULL,     0xf8000000000001ffULL,     0x03f000001ff80007ULL,
    0x007fc00000001ff0ULL,     0x0007fc0000003fc0ULL,     0xf8000000000003ffULL,     0x000000003ff0000fULL,
    0x003fc00000000ff0ULL,     0x0007fc0000003fe0ULL,     0xf8000000000003ffULL,     0x000000007ff00007ULL,
    0x003fc00000001ff0ULL,     0x000ff80000003fe0ULL,     0xf8000000000007feULL,     0x00000000ffe0000fULL,
    0x003fc00000000ff0ULL,     0x000ff80000001ff0ULL,     0xf8000000000007feULL,     0x00000000ffc00007ULL,
    0x007fc00000001ff0ULL,     0x001ff00000001ff0ULL,     0xf800000000000ffcULL,     0x00000001ff80000fULL,
    0x003fe00000000ff0ULL,     0x001ff00000000ff0ULL,     0xf800000000000ffcULL,     0x00000003ff000007ULL,
    0x003fc00000001ff0ULL,     0x001fe00000000ff0ULL,     0xf800000000003ff8ULL,     0x00000007ff00000fULL,
    0x007fc00000000ff0ULL,     0x003fe00000000ff8ULL,     0xf800000000003ff8ULL,     0x03f00007fe000007ULL,
    0x003fc00000001ff0ULL,     0x003fe000000007f8ULL,     0xf80000000000fff0ULL,     0x0ff8000ffc00000fULL,
    0x003fc00000000ff0ULL,     0x007fc000000007fcULL,     0xf80010000001ffe0ULL,     0x0ffc001ff8000007ULL,
    0x007fc00000001ff0ULL,     0x007fc000000007fcULL,     0xf8001d00000fffc0ULL,     0x1ffc003ff800000fULL,
    0x003fe00000000ff0ULL,     0x007fc000000003feULL,     0xf8001fe8007fff80ULL,     0x1ffe003ff0000007ULL,
    0x003fc00000001ff0ULL,     0x00ff8000000003feULL,     0xf8001fffffffff00ULL,     0x1ffc007fe000000fULL,
    0x003fc00000000ff0ULL,     0x00ff8000000001ffULL,     0xf8001ffffffffe00ULL,     0x1ffe00ffc0000007ULL,
    0x007fc00000001ff0ULL,     0x01ff0000000001feULL,     0xf8001ffffffffc00ULL,     0x1ffc01ffc000000fULL,
    0x003fc00000000ff0ULL,     0x01ff0000000000ffULL,     0xf8001ffffffff000ULL,     0x1ffc01ff80000007ULL,
    0x803fc00000001ff0ULL,     0x01ff0000000000ffULL,     0xf8001fffffffc000ULL,     0x0ffc03ff0000000fULL,
    0x807fe00000000ff0ULL,     0x03fe0000000000ffULL,     0xf80001fffffd0000ULL,     0x0ff807fe00000007ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x00000017ffd00000ULL,     0x03e0000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,
    0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,     0x0000000000000000ULL,

};
// clang-format on

#include "FONT.INC"

// source: https://github.com/czinn/perlin

static inline double rawnoise(int n) {
	n = (n << 13) ^ n;
	return (1.0
		- ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)
			/ 1073741824.0);
}

static inline double noise1d(int x, int octave, int seed) {
	return rawnoise(x * 1619 + octave * 3463 + seed * 13397);
}

static inline double noise2d(int x, int y, int octave, int seed) {
	return rawnoise(x * 1619 + y * 31337 + octave * 3463 + seed * 13397);
}

static inline double noise3d(int x, int y, int z, int octave, int seed) {
	return rawnoise(
		x * 1919 + y * 31337 + z * 7669 + octave * 3463 + seed * 13397);
}

static inline double interpolate(double a, double b, double x) {
	double f = (1 - cos(x * 3.141593)) * 0.5;

	return a * (1 - f) + b * f;
}

static inline double smooth1d(double x, int octave, int seed) {
	int intx = (int) x;
	double fracx = x - intx;

	double v1 = noise1d(intx, octave, seed);
	double v2 = noise1d(intx + 1, octave, seed);

	return interpolate(v1, v2, fracx);
}

static inline double smooth2d(double x, double y, int octave, int seed) {
	int intx = (int) x;
	double fracx = x - intx;
	int inty = (int) y;
	double fracy = y - inty;

	double v1 = noise2d(intx, inty, octave, seed);
	double v2 = noise2d(intx + 1, inty, octave, seed);
	double v3 = noise2d(intx, inty + 1, octave, seed);
	double v4 = noise2d(intx + 1, inty + 1, octave, seed);

	double i1 = interpolate(v1, v2, fracx);
	double i2 = interpolate(v3, v4, fracx);

	return interpolate(i1, i2, fracy);
}

static inline double smooth3d(
	double x, double y, double z, int octave, int seed) {
	int intx = (int) x;
	double fracx = x - intx;
	int inty = (int) y;
	double fracy = y - inty;
	int intz = (int) z;
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

static inline double pnoise1d(
	double x, double persistence, int octaves, int seed) {
	double total = 0.0;
	double frequency = 1.0;
	double amplitude = 1.0;
	int i = 0;

	for (i = 0; i < octaves; i++) {
		total += smooth1d(x * frequency, i, seed) * amplitude;
		frequency /= 2;
		amplitude *= persistence;
	}

	return total;
}

static inline double pnoise2d(
	double x, double y, double persistence, int octaves, int seed) {
	double total = 0.0;
	double frequency = 1.0;
	double amplitude = 1.0;
	int i = 0;

	for (i = 0; i < octaves; i++) {
		total += smooth2d(x * frequency, y * frequency, i, seed) * amplitude;
		frequency /= 2;
		amplitude *= persistence;
	}

	return total;
}

static inline double pnoise3d(
	double x, double y, double z, double persistence, int octaves, int seed) {
	double total = 0.0;
	double frequency = 1.0;
	double amplitude = 1.0;
	int i = 0;

	for (i = 0; i < octaves; i++) {
		total += smooth3d(x * frequency, y * frequency, z * frequency, i, seed)
			* amplitude;
		frequency /= 2;
		amplitude *= persistence;
	}

	return total;
}

#define DRAW(x, y, c) *(framebuf + (unsigned long) X_RES * (y) + (x)) = c

static const char *GREETZ
	= "GREETZ TO EERIE BROKENPIPE BPAUL DEVILBUNNY HOWIE RAYBENA "
	  "FRIES WALL STARER ANTI ALEGS";

static inline Glyph lookup_glyph(char c) {
	int i;
	Glyph g;

	for (i = 0; i < FONT_SIZE; i++) {
		g = FONT[i];
		if (g.ch == c) {
			return g;
		}
	}

	// not found, just return 'A'
	return *FONT;
}

int main(void) {
	int y;
	int x;
	int time = 0;
	int kb_c;
	int i;

	// text stuff
	int tword; // word idx
	int tbit; // bit in word idx
	char pix; // pixel in image
	char is_text_mode = 0;

	// greetz stuff
	int greetz_idx;
	char greetz_char;
	int greetz_offset;
	Glyph glyph;

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

	greetz_idx = 0;
	greetz_char = 'A';
	greetz_offset = 0;

	while (1) {
		if (kbhit()) {
			kb_c = getch();
			// 27 == ESC
			if (kb_c == 27)
				break;

			// 104 == 'H'
			if (kb_c == 104) {
				if (is_text_mode == 0) {
					is_text_mode = 1;
				} else {
					is_text_mode = 0;
				}
				// wait til next frame
				continue;
			} // END IF
		} // END KBHIT

		// draw to main buffer
		for (y = 32; y < Y_RES - 32; y++) {
			for (x = 32; x < X_RES - 32; x++) {
				char noise = (char) (pnoise3d(x * 0.01, y * 0.01, time * 0.01,
										 0.7, 5, 12124)
					* 255.);

				DRAW(x, y, noise);
			} // END INNER FOR
		} // END OUTER FOR

		if (is_text_mode == 1) {
			// draw to mask buffer
			// get the pixmap for the char
			greetz_char = *(GREETZ + greetz_idx);
			glyph = lookup_glyph(greetz_char);

			tword = 0;
			tbit = 0;

			// blit the char to the screen
			// we know the glyph is 128x128
			for (y = 64; y < 64 + 128; y++) {
				for (x = greetz_offset; x < MIN(greetz_offset + 128, X_RES);
					x++) {
					// lookup the pixmap
					pix = (glyph.data[tword] >> tbit) & 1;
					tbit++;
					if (tbit >= 64) {
						// next word
						tbit = 0;
						tword++;
					}

					if (pix == 1) {
						DRAW(x, y, 10);
					} else {
						DRAW(x, y, 0);
					}
				}
			}
		}

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
			is_text_mode = !is_text_mode;
		}*/

		time++;
	} // END WHILE

	// return to con mode
	_setvideomode(_DEFAULTMODE);

	printf("PLASMATIC - M YOUNG 2025\n");
	printf("thanks for watching :3\n");
	return 0;
} // END MAIN
