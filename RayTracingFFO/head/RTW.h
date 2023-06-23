#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;


const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926f;

float degress2radians(float degress) {
	return degress * pi / 180.0f;
}

float clamp(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

float random_float() {
	return rand() / (RAND_MAX + 1.0f);
}

float random_float(float min, float max) {
	return min + (max - min) * random_float();
}

int random_int(int min, int max) {
	return static_cast<int>(random_float(min, max+1));
}

#include <Ray.h>
