#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <Hitable.h>
#include <AABB.h>

class MovingSphere : public Hitable {
public:
	MovingSphere() {}
	MovingSphere(point3 cen0, point3 cen1, float t0, float t1, float r, shared_ptr<Material> m)
		:center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material(m) {};

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const override;
	virtual bool bounding_box(float t1, float t2, AABB& box) const override;
	point3 center(float time) const;

public:
	point3 center0, center1;
	float time0, time1;
	float radius;
	shared_ptr<Material> material;
};


bool MovingSphere::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const {
	vec3 oc = ray.m_origin - center(ray.time());
	float a = ray.direction().length_sq();
	float b = dot(oc, ray.direction());
	float c = oc.length_sq() - radius * radius;

	float d = b * b - a * c;
	if (d < 0) return false;

	float sd = sqrt(d);
	float root = (-b - sd) / a;
	if (root < tmin || root > tmax) {
		root = (-b + sd) / a;
		if (root < tmin || root > tmax) {
			return false;
		}
	}

	rec.t = root;
	rec.p = ray.at(rec.t);
	vec3 out_normal = (rec.p - center(ray.time())) / radius;
	rec.set_face_normal(ray, out_normal);
	rec.material = material;

	return true;
}

bool MovingSphere::bounding_box(float t1, float t2, AABB& box) const {
	AABB box1(
		center(t1) - vec3(radius, radius, radius),
		center(t1) + vec3(radius, radius, radius)
		);
	AABB box2(
		center(t2) - vec3(radius, radius, radius),
		center(t2) + vec3(radius, radius, radius)
		);
	box = surrounding_box(box1, box2);
	return true;
}

point3 MovingSphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

