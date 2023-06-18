#pragma once

#include <cassert>
#include <cmath>
#include <algorithm>
#include <type_traits>
#include <iostream>

#include <RTWeekend.h>


class Vec3 {
public:
	Vec3(float x, float y, float z) : x(x), y(y), z(z){
		assert(!HasNaN());
	}

	Vec3() : x(0), y(0), z(0) {}

	Vec3(float v) : x(v), y(v), z(v) {}

	Vec3(const Vec3& v) : Vec3(v.x, v.y, v.z) {}

public:
	bool HasNaN() const {
		return std::isnan(static_cast<float>(x))
			|| std::isnan(static_cast<float>(y))
			|| std::isnan(static_cast<float>(z));
	}

	static const Vec3 Min(const Vec3& v1, const Vec3 v2) {
		return { (std::min)(v1.x, v2.x), (std::min)(v1.y, v2.y), (std::min)(v1.z, v2.z) };
	}
	static const Vec3 Max(const Vec3& v1, const Vec3 v2) {
		return { (std::max)(v1.x, v2.x), (std::max)(v1.y, v2.y), (std::max)(v1.z, v2.z) };
	}

	float operator[](int n) {
		assert(n >= 0 && n < 3);
		return _data[n];
	}

	const float operator[](int n) const{
		assert(n >= 0 && n < 3);
		return _data[n];
	}

	static const Vec3 Lerp(const Vec3& a, const Vec3& b, float t) {
		float tF = static_cast<float>(t);
		assert(tF >= 0.0f && tF <= 1.0f);
		Vec3 aF = a;
		Vec3 bF = b;
		//ta + (1-t)b
		return (1.0f - tF) * aF + tF * bF;
	}

	const Vec3 LerpWith(const Vec3& v, float t) const {
		return Lerp(*this, v, t);
	}

	const Vec3 operator+(const Vec3& v) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	Vec3& operator+=(const Vec3 & v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	const Vec3 operator-() const {
		return { -x, -y, -z };
	}

	const Vec3 operator-(const Vec3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	Vec3& operator-=(const Vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	const Vec3 operator*(float k) const {
		return { x * k, y * k, z * k };
	}

	Vec3& operator*=(float k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	friend const Vec3 operator*(float k, Vec3 v) {
		return v * k;
	}

	const Vec3 operator/(float k) const {
		assert(k != 0);
		float invk = 1 / k;
		return {x*invk, y*invk, z*invk};
	}

	Vec3& operator/=(float k) {
		assert(k != 0);
		float invk = 1 / k;
		x *= invk;
		y *= invk;
		z *= invk;
		return *this;
	}

	static float Dot(const Vec3& v1, const Vec3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	float Dot(const Vec3& v) const {
		return Dot(*this, v);
	}

	float Norm2() const {
		return this->Dot(*this);
	}
	float Norm() const {
		return std::sqrt(Norm2());
	}
	const Vec3 Normalize() const {
		float norm = Norm();
		return *this / norm;
	}

	const Vec3 operator*(const Vec3& v) const {
		return { x * v.x, y * v.y, z * v.z };
	}

	static const Vec3 Cross(const Vec3& v1, const Vec3& v2) {
		return {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		};
	}
	const Vec3 Cross(const Vec3& v) const {
		return Cross(*this, v);
	}

	static Vec3 Random() {
		return Vec3(RandomFloat(), RandomFloat(), RandomFloat());
	}

	static Vec3 Random(float min, float max) {
		return Vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
	}

	bool NearZero() const {
		const auto s = 1e-8;
		return (fabsf(x) < s) && (fabsf(y) < s) && (fabsf(z) < s);
	}

public:
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float _data[3]; };
	};
};	

Vec3 RandomInUnitSphere() {
	while (true) {
		Vec3 p = Vec3::Random(-1, 1);
		if (p.Dot(p) >= 1) continue;
		return p;
	}
}

Vec3 RandomUnitSphere() {
	return RandomInUnitSphere().Normalize();
}

Vec3 Reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * v.Dot(n) * n;
}

Vec3 Refract(const Vec3& uv, const Vec3& n, float etai_over_etat) {
	auto cos_theta = fmin(-uv.Dot(n), 1.0);
	Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.Dot(r_out_perp))) * n;
	return r_out_perp + r_out_parallel;
}