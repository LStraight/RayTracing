#pragma once

#include "RTW.h"
#include "Vec3.h"
#include "Hitable.h"

class xy_rect :public Hitable {
public:
	xy_rect() {}
	xy_rect(float x00, float x11, float y00, float y11, float kk, shared_ptr<Material> m)
		:x0(x00), x1(x11), y0(y00), y1(y11), k(kk), material(m) {};

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;

	virtual bool bounding_box(float t0, float t1, AABB& output_box)const override {
		output_box = AABB(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
		return true;
	}

public:
	shared_ptr<Material> material;
	float x0, x1, y0, y1, k;
};

bool xy_rect::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const {
	auto t = (k - ray.origin().z) / ray.direction().z;
	if (t<tmin || t > tmax) return false;

	auto x = ray.origin().x + t * ray.direction().x;
	auto y = ray.origin().y + t * ray.direction().y;
	if (x < x0 || x>x1 || y < y0 || y>y1)return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;

	auto out_normal = vec3(0, 0, 1);
	rec.set_face_normal(ray, out_normal);
	rec.material = material;
	rec.p = ray.at(t);
	return true;
}


class xz_rect :public Hitable {
public:
	xz_rect() {}
	xz_rect(float x00, float x11, float z00, float z11, float kk, shared_ptr<Material> m)
		:x0(x00), x1(x11), z0(z00), z1(z11), k(kk), material(m) {};

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;

	virtual bool bounding_box(float t0, float t1, AABB& output_box)const override {
		output_box = AABB(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
		return true;
	}

public:
	shared_ptr<Material> material;
	float x0, x1, z0, z1, k;
};

bool xz_rect::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const {
	auto t = (k - ray.origin().y) / ray.direction().y;
	if (t<tmin || t > tmax) return false;

	auto x = ray.origin().x + t * ray.direction().x;
	auto z = ray.origin().z + t * ray.direction().z;
	if (x < x0 || x>x1 || z < z0 || z>z1)return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	           
	auto out_normal = vec3(0, 1, 0);
	rec.set_face_normal(ray, out_normal);
	rec.material = material;
	rec.p = ray.at(t);
	return true;
}

class yz_rect :public Hitable {
public:
	yz_rect() {}
	yz_rect(float y00, float y11, float z00, float z11, float kk, shared_ptr<Material> m)
		:y0(y00), y1(y11), z0(z00), z1(z11), k(kk), material(m) {};

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;

	virtual bool bounding_box(float t0, float t1, AABB& output_box)const override {
		output_box = AABB(point3(k - 0.0001, y0, z0 ), point3(k + 0.0001, y1, z1));
		return true;
	}

public:
	shared_ptr<Material> material;
	float y0, y1, z0, z1, k;
};

bool yz_rect::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const {
	auto t = (k - ray.origin().x) / ray.direction().x;
	if (t<tmin || t > tmax) return false;

	auto y = ray.origin().y + t * ray.direction().y;
	auto z = ray.origin().z + t * ray.direction().z;
	if (y < y0 || y>y1 || z < z0 || z>z1)return false;

	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;

	auto out_normal = vec3(1, 0, 0);
	rec.set_face_normal(ray, out_normal);
	rec.material = material;
	rec.p = ray.at(t);
	return true;
}