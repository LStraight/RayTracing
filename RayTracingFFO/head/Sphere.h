#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <Hitable.h>

class Sphere :public Hitable {
public:
	Sphere() {}
	Sphere(point3 center, float r, shared_ptr<Material> material)
		: m_center(center), m_radius(r), m_material(material){}

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;
	virtual bool bounding_box(float t0, float t1, AABB& box)const override;

private:
	static void get_sphere_uv(const point3& p, float& u, float v);

public:
	point3 m_center;
	float m_radius;
	shared_ptr<Material> m_material;
};


bool Sphere::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const {
	vec3 oc = ray.origin() - m_center;
	auto a = ray.direction().length_sq();
	auto b = dot(oc, ray.direction());
	auto c = oc.length_sq() - m_radius * m_radius;

	auto d = b * b - a * c;
	if (d < 0) return false;

	auto sqrtd = sqrt(d);
	auto root = (-b - sqrtd) / a;
	if (root < tmin || root > tmax) {
		root = (-b + sqrtd) / a;
		if (root < tmin || root > tmax) {
			return false;
		}
	}

	rec.t = root;
	rec.p = ray.at(rec.t);
	vec3 outward_normal = (rec.p - m_center) / m_radius;
	rec.set_face_normal(ray, outward_normal);
	get_sphere_uv(outward_normal, rec.u, rec.v);
	rec.material = m_material;

	return true;
}

bool Sphere::bounding_box(float t0, float t1, AABB& box) const {
	box = AABB(
		m_center - vec3(m_radius, m_radius, m_radius),
		m_center + vec3(m_radius, m_radius, m_radius)
	);
	return true;

}

void Sphere::get_sphere_uv(const point3& p, float& u, float v) {
	//p是单位球面上的一个点，记录在record中
	auto theta = acos(-p.y);
	auto phi = atan2(-p.z, p.x) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}


