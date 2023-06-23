#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <Texture.h>

struct HitRecord;

class Material {
public:
	virtual color3 emitted(float u, float v, const point3& p) const {
		return color3(0, 0, 0);
	}
	virtual bool scatter(const Ray& ray, const HitRecord& rec, color3& attenuation, Ray& scattered)const {
		return false;
	}
};

class Lambertian :public Material {
public:
	Lambertian(const color3& a) : albedo(make_shared<SolidColor>(a)) {}
	Lambertian(shared_ptr<Texture> a) : albedo(a) {}

	virtual bool scatter(const Ray& ray, const HitRecord& rec, color3& attenuation, Ray& scattered)const override {
		point3 scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near2zero()) scatter_direction = rec.normal;
		scattered = Ray(rec.p, scatter_direction, ray.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	shared_ptr<Texture> albedo;
};

class Metal :public Material {
public:
	Metal(const color3& a, float f) :albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const Ray& ray, const HitRecord& rec, color3& attenuation, Ray& scattered
	) const override {
		vec3 reflected = reflect(normalize(ray.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), ray.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color3 albedo;
	float fuzz;
};

class Dielectric :public Material {
public:
	Dielectric(float rate) : ir(rate) {}
	virtual bool scatter(
		const Ray& ray, const HitRecord& rec, color3& attenuation, Ray& scattered
	) const override {
		attenuation = color3(1.0f, 1.0f, 1.0f);
		float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

		vec3 unit_direction = normalize(ray.direction());
		float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
		float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = Ray(rec.p, direction, ray.time());
		return true;
	}

public:
	float ir;

private:
	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

class DiffuseLight : public Material {
public:
	DiffuseLight(shared_ptr<Texture> a):emit(0) {}
	DiffuseLight(color3 c): emit(make_shared<SolidColor>(c)) {}

	virtual bool scatter(const Ray& ray, const HitRecord& rec, color3& attenuation, Ray& scattered) const override {
		return false;
	}

	virtual color3 emitted(float u, float v, const point3& p) const override {
		return emit->value(u, v, p);
	}
	

public:
	shared_ptr<Texture> emit;
};

class Isotropic :public Material {
public:
	Isotropic(color3 c):albedo(make_shared<SolidColor>(c)) {}

	Isotropic(shared_ptr<Texture> a) :albedo(a) {}

	virtual bool scatter(const Ray& ray, const HitRecord& rec, color3& attenuation, Ray& scattered)const override {
		scattered = Ray(rec.p, random_in_unit_sphere(), ray.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	shared_ptr<Texture> albedo;
};
