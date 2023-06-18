#pragma once

#include <Hitable.h>

#include <vector>
#include <memory>

using std::make_shared;
using std::shared_ptr;

class HitableList : public Hitable {
public:
	HitableList() {};
	HitableList(shared_ptr<Hitable>& object) {
		Add(object);
	}

	void Add(shared_ptr<Hitable> object) {
		objects.push_back(object);
	}

	void clear() { objects.clear(); }

	virtual bool Hit(Ray& ray, HitRecord& rec)const override;

public:
	std::vector<shared_ptr<Hitable>> objects;
};

bool HitableList::Hit(Ray& ray, HitRecord& rec)const {
	HitRecord temp_rec;
	bool is_hit = false;
	auto closest_so_far = ray.GetTMax();

	for (const auto& object : objects) {
		if (object->Hit(ray, temp_rec)) {
			is_hit = true;
			closest_so_far = ray.GetTMax();
			rec = temp_rec;
		}
	}

	return is_hit;
}


