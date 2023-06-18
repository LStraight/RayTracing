#pragma once

#include <RTWeekend.h>

class Material;

struct HitRecord {
	Vec3 p;
	Vec3 normal;
	shared_ptr<Material> material;
	bool front_face;

	void SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
		front_face = ray.GetDirection().Dot(outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}

};

class Hitable {
public:
	virtual bool Hit(Ray& ray, HitRecord& rec)const = 0;
	//virtual const Box GetBox() const = 0;
};
