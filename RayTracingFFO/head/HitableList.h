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
		is_first = false;	//����ǵ�һ�������ĵ�һ�����󣬾�ֱ������ʱ�����temp_box��ֵ����output_box
	}
	//������������ж�����󣬺����Ķ���İ�Χ�ж���ǰ�����İ�Χ���ںϣ����Ƹ����ض���
	return true;
}




