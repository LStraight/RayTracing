#pragma once

#include <Hitable.h>

class Sphere : public Hitable {
#define Ptr std::shared_ptr
public:
	Sphere(){}
	Sphere(const Vec3& center, float radius, shared_ptr<Material> material)
	: center(center), radius(radius), material(material) {}

	virtual bool Hit(Ray& ray, HitRecord& rec) const override;

private:
	Vec3 center = 0;
	float radius = 0;
	shared_ptr<Material> material;
};

bool Sphere::Hit(Ray& ray, HitRecord& rec) const {
	auto oc = ray.GetOrigin() - center;
	float a = ray.GetDirection().Dot(ray.GetDirection());
	float b = ray.GetDirection().Dot(oc);
	float c = oc.Dot(oc) - radius * radius;

	float discriminant = b * b - a * c;
	if (discriminant < 0.0f) return false;

	float sqrtDiscriminant = sqrt(discriminant);

	float t0 = (-b - sqrtDiscriminant) / a;
	if (t0 <= ray.GetTMin() || t0 >= ray.GetTMax()) {
		float t1 = (-b + sqrtDiscriminant) / a;
		if (t1 <= ray.GetTMin() || t1 >= ray.GetTMax())
			return false; //即两个根都在范围外
		ray.SetTMax(t1);
	}
	else ray.SetTMax(t0);

	rec.p = ray.EndPos();
	Vec3 outward_normal = (rec.p - center) / radius;
	rec.SetFaceNormal(ray, outward_normal);
	rec.material = material;

	return true;
}