#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <Hitable.h>

#include <memory>
#include <vector>

using std::vector;

class HitableList :public Hitable {
public:
	HitableList() {}
	HitableList(shared_ptr<Hitable> object) { add(object); }

	void add(shared_ptr<Hitable> object) { objects.push_back(object); }
	void clear() { objects.clear(); }
	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const override;
	virtual bool bounding_box(float t1, float t2, AABB& box)const override;

public:
	vector<shared_ptr<Hitable>> objects;
};

bool HitableList::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const {
	HitRecord temp_rec;
	auto is_hit = false;
	auto closest_so_far = tmax;

	for (const auto& object : objects) {
		if (object->hit(ray, tmin, closest_so_far, temp_rec)) {
			is_hit = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return is_hit;
}

bool HitableList::bounding_box(float t1, float t2, AABB& output_box)const {
	if (objects.empty()) return false;

	AABB temp_box;
	bool is_first = true;

	for (const auto& object : objects) {
		if (!object->bounding_box(t1, t2, temp_box)) return false;
		output_box = is_first ? temp_box : surrounding_box(output_box, temp_box);
		is_first = false;	//如果是第一个容器的第一个对象，就直接用临时构造的temp_box赋值给给output_box
	}
	//如果有容器中有多个对象，后续的对象的包围盒对与前面对象的包围盒融合，复制给返回对象
	return true;
}




