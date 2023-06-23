#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <AABB.h>

class Material;

struct HitRecord {
	point3 p; // 坐标
	vec3 normal; //法向
	shared_ptr<Material> material; //材质
	float t; //光线命中物体的时间
	float u;
	float v; // 纹理坐标
	bool front_face; // 正反面

	void set_face_normal(const Ray& ray, const vec3& outward_normal) {
		front_face = dot(ray.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hitable {
public:
	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const=0;
	virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
};

class Translate :public Hitable {
public:
	Translate(shared_ptr<Hitable> p, const Vec3& of)
		:object(p), offset(of) {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const override;
	virtual bool bounding_box(float t0, float t1, AABB& output_box)const override;

public:
	shared_ptr<Hitable> object;
	Vec3 offset;
};

bool Translate::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const {
	Ray r_t(ray.origin() - offset, ray.direction(), ray.time());
	if (!object->hit(r_t, tmin, tmax, rec)) return false;

	rec.p = rec.p + offset;
	rec.set_face_normal(r_t, rec.normal);
	return true;
}

bool Translate::bounding_box(float t0, float t1, AABB& output_box) const {
	if (!object->bounding_box(t0, t1, output_box)) return false;
	output_box = AABB(output_box.min() + offset, output_box.max() + offset);

	return true;
}

class Rotatey :public Hitable {
public:
	Rotatey(shared_ptr<Hitable> p, float angle);

	virtual bool hit(const Ray& ray, float tmin, float max, HitRecord& rec)const override;
	virtual bool bounding_box(float t1, float t2, AABB& output_box) const override {
		output_box = box;
		return hasbox;
	}

public:
	shared_ptr<Hitable> object;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	AABB box;
};

Rotatey::Rotatey(shared_ptr<Hitable> p, float angle) :object(p) {
	auto radians = degress2radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);

	hasbox = object->bounding_box(0, 1, box);
	point3 min(infinity, infinity, infinity);
	point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				auto x = i * box.max().x + (1 - i) * box.min().x;
				auto y = j * box.max().y + (1 - j) * box.min().y;
				auto z = k * box.max().z + (1 - k) * box.min().z;

				auto newx = cos_theta * x + sin_theta * z;
				auto newz = -sin_theta * x + cos_theta * z;

				vec3 tester(newx, y, newz);

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	box = AABB(min, max);
}


bool Rotatey::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const {
	point3 origin = ray.origin();
	point3 direction = ray.direction();

	origin.x = cos_theta * ray.origin().x - sin_theta * ray.origin().z;
	origin.z = sin_theta * ray.origin().x + cos_theta * ray.origin().z;

	direction.x = cos_theta * ray.direction().x - sin_theta * ray.direction().z;
	direction.z = sin_theta * ray.direction().x + cos_theta * ray.direction().z;

	Ray had_rotate(origin, direction, ray.time());

	if (!object->hit(had_rotate, tmin, tmax, rec))
		return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p[0] =  cos_theta * rec.p.x + sin_theta * rec.p.z;
	p[2] = -sin_theta * rec.p.x + cos_theta * rec.p.z;

	normal[0] =  cos_theta * rec.normal.x + sin_theta * rec.normal.z;
	normal[2] = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;

	rec.p = p;
	rec.set_face_normal(had_rotate, normal);

	return true;
}