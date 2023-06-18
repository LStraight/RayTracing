#pragma once

#include "RTWeekend.h"

struct HitRecord;

class Material {
public:
	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian :public Material {
public:
	Lambertian(const Color& v) : albedo(v){}

	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
		auto scatter_direction = rec.normal + RandomUnitSphere();
		if (scatter_direction.NearZero()) {
			scatter_direction = rec.normal;
		}

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;

		return true;
	}

public:
	Color albedo;
};

class Metal : public Material {
public:
	Metal(const Color& a, float f) :albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override{
		Vec3 reflected = Reflect(ray_in.GetDirection().Normalize(), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return scattered.GetDirection().Dot(rec.normal) > 0;
	}

public:
	Color albedo;
	float fuzz;
};

class Dielectric :public Material {
public:
	Dielectric(float index_of_refraction) : ir(index_of_refraction){}

	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
		attenuation = Color(1.0, 1.0, 1.0);
		float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		Vec3 unit_direction = ray_in.GetDirection().Normalize();

		double cos_theta = fmin(-unit_direction.Dot(rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;

		Vec3 direction;
		if (cannot_refract || Reflectence(cos_theta, refraction_ratio) > RandomFloat()) {
			direction = Reflect(unit_direction, rec.normal);
		}else {
			direction = Refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = Ray(rec.p, direction);
		return true;
	}
public:
	float ir;

private:
	static float Reflectence(float cosine, float ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 *= r0;
		return r0 + (1 - r0) * pow(1 - cosine, 5);
	}
};