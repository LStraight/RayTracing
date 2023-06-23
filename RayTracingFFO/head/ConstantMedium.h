#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <Hitable.h>
#include <Material.h>
#include <Texture.h>


//恒定介质，赋予hitable对象一种介质或者添加表面散射，可以调节光线在物体中的传播路径
class ConstantMedium :public Hitable {
public:
	ConstantMedium(shared_ptr<Hitable> b, float d, shared_ptr<Texture> a)
		: boundary(b), neg_inv_density(-1/d), phase_function(make_shared<Isotropic>(a)){}

	ConstantMedium(shared_ptr<Hitable> b, float d, color3 c)
		:boundary(b), neg_inv_density(-1/d), phase_function(make_shared<Isotropic>(c)) {}

	virtual bool hit(const Ray &ray, float tmin, float tmax, HitRecord& rec)const override;

	virtual bool bounding_box(float t1, float t2, AABB& output_box)const override {
		return boundary->bounding_box(t1, t2, output_box);
	}
public:
	shared_ptr<Hitable> boundary;
	shared_ptr<Material> phase_function;
	float neg_inv_density;
};


bool ConstantMedium::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const {
	const bool enableDebug = false;
	const bool debugging = enableDebug && random_float() < 0.0001;

	HitRecord rec1, rec2;
	if (!boundary->hit(ray, -infinity, infinity, rec1)) return false;
	if (!boundary->hit(ray, rec1.t + 0.0001, infinity, rec2)) return false;

	if (debugging) std::cerr << "\ntmin = " << rec1.t << ", tmax = " << rec2.t << '\n';

	if (rec1.t < tmin) rec1.t = tmin;
	if (rec2.t > tmax) rec2.t = tmax;

	if (rec1.t >= rec2.t) return false;

	if (rec1.t < 0) rec1.t = 0;

	const auto ray_len = ray.direction().length();
	const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_len;
	const auto hit_distance = neg_inv_density * log(random_float());
	
	if (hit_distance > distance_inside_boundary) return false;
	rec.t = rec1.t + hit_distance / ray_len;
	rec.p = ray.at(rec.t);

	if (debugging) {
		std::cerr << "hit_distance = " << hit_distance << '\n'
			<< "rec.t = " << rec.t << '\n'
			<< "rec.p = " << rec.p << '\n';
	}

	rec.normal = vec3(1, 0, 0);
	rec.front_face = true;
	rec.material = phase_function;

	return true;
}
