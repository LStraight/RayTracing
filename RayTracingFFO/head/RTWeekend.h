#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const float infinity = std::numeric_limits<double>::infinity();
const float pi = 3.1415926535897932385;

float Degrees2Radians(float degress) {
	return degress * pi / 180;
}

float RandomFloat() {
	return rand() / (RAND_MAX + 1.0);
}

float RandomFloat(float min, float max) {
	return min + (max - min) * RandomFloat();
}

int RandomInt(int min, int max) {
	return static_cast<int>(RandomFloat(min, max));
}

float clamp(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

#include <Ray.h>
#include <Vec3.h>


#define Color Vec3
#define Position Vec3
