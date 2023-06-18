#pragma once

#include <RTWeekend.h>

class Camera {
public:
	Camera();
	Camera(Position lookfrom, Position lookat, Vec3 vup, float vfov, float aspectRatio, float aperture, float focus_dist);
	Ray GetRay(float s, float t) const;
	
public:
	Vec3 camPos;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lensRadius;
};


Camera::Camera() {
	auto aspectRatio = 16.0 / 9.0;
	auto viewportHeight = 2.0;
	auto viewportWidth = aspectRatio * viewportHeight;
	auto focal_length = 1.0;

	camPos = Vec3(0, 0, 0);
	horizontal = Vec3(viewportWidth, 0.0, 0.0);
	vertical = Vec3(0.0, viewportHeight, 0.0);
	lowerLeftCorner = camPos - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
}

Camera::Camera(
		Position lookfrom, Position lookat, 
		Vec3 vup, float vfov, 
		float aspectRatio, float aperture,
		float focus_dist) {
	auto theta = Degrees2Radians(vfov);
	auto h = tan(theta / 2);
	auto viewportHeight = 2.0 * h;
	auto viewportWidth = aspectRatio * viewportHeight;

	auto w = (lookfrom - lookat).Normalize();
	auto u = vup.Cross(w).Normalize();
	auto v = w.Cross(u);

	camPos = lookfrom;
	horizontal = focus_dist * viewportWidth * u;
	vertical = focus_dist * viewportHeight * v;
	lowerLeftCorner = camPos - horizontal / 2 - vertical / 2 - focus_dist * w;

	lensRadius = aperture / 2;
}

Ray Camera::GetRay(float u, float v) const{
	Vec3 rd = lensRadius * RandomInUnitSphere();
	Vec3 offset = u * rd.x + v * rd.y;

	return Ray(camPos + offset, lowerLeftCorner + u * horizontal + v * vertical - camPos - offset);
}