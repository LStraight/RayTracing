#pragma once


#include <RTW.h>
#include <Vec3.h>

class Ray{
public:
	Ray() {}

	Ray(const point3& orig, const vec3& direc, float time)
		: m_origin(orig), m_direction(direc), m_tm(time) {}


	point3 origin() const	{ return m_origin; }
	vec3 direction() const	{ return m_direction; }
	float time() const		{ return m_tm; }

	point3 at(float t) const {
		return m_origin + t * m_direction;
	}

public:
	point3 m_origin;
	vec3 m_direction;
	float m_tm;
};