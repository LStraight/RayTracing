#pragma once

#include <RTW.h>
#include <Aarect.h>
#include <HitableList.h>

class Box :public Hitable {
public:
	Box() {}
	Box(const point3& p1, const point3& p2, shared_ptr<Material> material);

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;
	virtual bool bounding_box(float t1, float t2, AABB& output_box) const override {
		output_box = AABB(box_min, box_max);
		return true;
	}

public:
	point3 box_min;
	point3 box_max;
	HitableList sides;
};


Box::Box(const point3& p1, const point3& p2, shared_ptr<Material> material) {
	box_min = p1;
	box_max = p2;

	sides.add(make_shared<xy_rect>(p1.x, p2.x, p1.y, p2.y, p2.z, material));
	sides.add(make_shared<xy_rect>(p1.x, p2.x, p1.y, p2.y, p1.z, material));

	sides.add(make_shared<xz_rect>(p1.x, p2.x, p1.z, p2.z, p2.y, material));
	sides.add(make_shared<xz_rect>(p1.x, p2.x, p1.z, p2.z, p1.y, material));

	sides.add(make_shared<yz_rect>(p1.y, p2.y, p1.z, p2.z, p2.x, material));
	sides.add(make_shared<yz_rect>(p1.y, p2.y, p1.z, p2.z, p1.x, material));

}

bool Box::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const {
	return sides.hit(ray, tmin, tmax, rec);
}
