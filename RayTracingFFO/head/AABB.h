#pragma once

#include "RTW.h"
#include <Vec3.h>

class AABB {
public:
	AABB() {}
	AABB(const point3& a, const point3& b) :minimum(a), maximum(b) {}
	
	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	bool hit(const Ray& ray, float tmin, float tmax) const;
public:
	point3 minimum;
	point3 maximum;
};


bool AABB::hit(const Ray& ray, float tmin, float tmax) const {
	for (int a = 0; a < 3; ++a) {
		auto t0 = fmin((minimum[a] - ray.origin()[a]) / ray.direction()[a],
			(maximum[a] - ray.origin()[a]) / ray.direction()[a]);
		auto t1 = fmax((minimum[a] - ray.origin()[a]) / ray.direction()[a],
			(maximum[a] - ray.origin()[a]) / ray.direction()[a]);
		tmin = fmax(t0, tmin);
		tmax = fmin(t1, tmax);
		if (tmax < tmin)
			return false;
	}
	return true;
}

AABB surrounding_box(AABB& box1, AABB& box2) {
	point3 small(
		fmin(box1.min().x, box2.min().x),
		fmin(box1.min().y, box2.min().y),
		fmin(box1.min().z, box2.min().z)
	);
	point3 big(
		fmax(box1.max().x, box2.max().x),
		fmax(box1.max().y, box2.max().y),
		fmax(box1.max().z, box2.max().z)
	);

	return AABB(small, big);
}
