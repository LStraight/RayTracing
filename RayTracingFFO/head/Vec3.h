#pragma once
#include <cmath>
#include <iostream>

#include <RTW.h>

#define point3 Vec3
#define color3 Vec3
#define vec3 Vec3

class Vec3 {
public:
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float v[3]; };
	};

public:
	Vec3() :v{ 0,0,0 } {}
	Vec3(float i, float j, float k) :x(i), y(j), z(k) {}
	Vec3(const Vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }
	Vec3 operator-() const { return {-x, -y, -z}; }
	Vec3& operator+(const Vec3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	Vec3& operator*=(const float t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}
	Vec3& operator/=(const float t) {
		return *this *= 1 / t;
	}
	float length_sq() const { return x* x + y * y + z * z; }
	float length() const { return std::sqrt(length_sq()); }
	Vec3& Normalize() { return *this /= length(); }
	bool near2zero() const {
		float s = 1e-4f;
		return (std::fabs(x) < s && std::fabs(y) < s && std::fabs(z) < s);
	}
	static Vec3 random(){
		return { random_float(), random_float(), random_float() };
	}
	static Vec3 random(float min, float max) {
		return{ random_float(min, max), random_float(min, max), random_float(min, max) };
	}
	
};

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.x << ' ' << v.y << ' ' << v.z;
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

Vec3 operator*(float t, const Vec3& v) {
	return { t * v.x, t * v.y, t * v.z };
}


Vec3 operator*(const Vec3& v, float t) {
	return { t * v.x, t * v.y, t * v.z };
}

Vec3 operator/(const Vec3&v, const float t) {
	return { v.x / t, v.y / t, v.z / t };
}

float dot(const Vec3 v1, const Vec3 v2) {
	return  { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
}

Vec3 cross(const Vec3 v1, const Vec3 v2) {
	return {	v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x };	
}

Vec3 normalize(Vec3 v) {
	return v / v.length();
}

Vec3 random_in_unit_disk() {
	while (true) {
		auto p = Vec3(random_float(-1, 1), random_float(-1, 1), 0);
		if (p.length_sq() >= 1) continue;
		return p;
	}
}

Vec3 random_in_unit_sphere() {
	while (true) {
		Vec3 p = Vec3(random_float(-1, 1), random_float(-1, 1), random_float(-1,1));
		if (p.length_sq() >= 1) continue;
		return p;
	}
}

Vec3 random_unit_vector() {
	return normalize(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& normal) {
	Vec3 v = random_in_unit_sphere();
	if (dot(v, normal) > 0.0) return v;
	else return -v;
}

Vec3 reflect(const Vec3& v, const Vec3& normal) {
	return v - 2 * dot(v, normal) * normal;
}

Vec3 refract(const Vec3& uv, const Vec3& normal, float rate) {
	auto cot = fmin(dot(-uv, normal), 1.0f);
	Vec3 out_perp = rate * (uv + cot * normal);
	Vec3 out_parallel = -std::sqrt(std::fabs(1.0f - out_perp.length_sq())) * normal;
	return out_perp + out_parallel;
}





