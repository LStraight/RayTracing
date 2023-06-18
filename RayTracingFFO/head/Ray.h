#pragma once

#include "Vec3.h"
#include "Util.h"

class Ray {
public:
	Ray(){}
	Ray(const Vec3& origin, const Vec3& dir, float tMin = Util::DEFAULT_TMIN, float tMax = Util::LARGE_FLT)
		:m_origin(origin), m_direct(dir), m_tMin(tMin), m_tMax(tMax)
	{}
	
	const Vec3 At(float t)const { return m_origin + t * m_direct; }
	const Vec3 StarPos() const { return this->At(m_tMin); }
	const Vec3 EndPos() const { return this->At(m_tMax); }
	const Vec3 InvDir()const { return { 1.0f / m_direct.x, 1.0f / m_direct.y, 1.0f / m_direct.z }; }
	const Vec3 GetOrigin() const { return m_origin; }
	const Vec3 GetDirection()const { return m_direct; }
	const float GetTMin() const { return m_tMin; }
	const float GetTMax() const { return m_tMax; }
	void SetOrigin(Vec3 pos) { m_origin = pos; }
	void SetDirection(Vec3 dir) { m_direct = dir; }
	void SetTMin(float f) { m_tMin = f; }
	void SetTMax(float f) { m_tMax = f; }

private:
	Vec3 m_origin, m_direct;
	float m_tMin, m_tMax;
};

