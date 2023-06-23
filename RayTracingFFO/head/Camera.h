#pragma once

#include <RTW.h>
#include <Vec3.h>

class Camera{
public:
	Camera(	point3 lookfrom,	point3 lookat, 
			vec3 vup,			float vfov, 
			float wh_rate,		float apeture, 
			float focus_dist,	float t0=0,	float t1=0);

	Ray get_ray(float s, float t) const;

private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w; // 相机坐标
	float lens_radius;
	float time0, time1;
};

Camera::Camera(	point3 lookfrom,	point3 lookat,
				vec3 vup,			float vfov,
				float wh_rate,		float aperture,
				float focus_dist,	float t0, float t1) {
	auto theta = degress2radians(vfov);
	auto h = tan(theta / 2);
	auto viewport_height = 2.0f * h;
	auto viewport_width = wh_rate * viewport_height;

	w = normalize(lookfrom - lookat);
	u = normalize(cross(vup, w));
	v = cross(w, u);

	origin = lookfrom;
	horizontal = focus_dist * viewport_width * u;
	vertical = focus_dist * viewport_height * v;
	lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

	lens_radius = aperture / 2; //光圈
	time0 = t0;
	time1 = t1;
}

Ray Camera::get_ray(float s, float t) const {
	vec3 rd = lens_radius * random_in_unit_disk();
	vec3 offset = u * rd.x + v * rd.y;
	return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_float(time0, time1));
}