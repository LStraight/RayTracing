#pragma once

#include <RTW.h>
#include <Hitable.h>
#include <HitableList.h>

#include <algorithm>

class BVHNode : public Hitable {
public:
	BVHNode();
	BVHNode(const HitableList& list, float t0, float t1)
		:BVHNode(list.objects, 0, list.objects.size(), t0, t1) {}
	BVHNode(const std::vector<shared_ptr<Hitable>>& src_objects, size_t start, size_t end, float t0, float t1);

	virtual bool hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;
	virtual bool bounding_box(float t0, float t1, AABB& output_box) const override;

public:
	shared_ptr<Hitable> left;
	shared_ptr<Hitable> right;
	AABB box;
};


bool box_campare(const shared_ptr <Hitable> a, const shared_ptr<Hitable> b, int axis);
bool box_x_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b);
bool box_y_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b);
bool box_z_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b);


//class method
BVHNode::BVHNode(const std::vector<shared_ptr<Hitable>>& src_objects, size_t start, size_t end, float t0, float t1) {
	auto objects = src_objects;
	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
	//比较器，x轴时等于box_x_compare，... ，使用三元条件符做判断
	
	size_t elem_num = end - start; //end 和 start是输入容器的指定区间的端点迭代器
	if (elem_num == 1)
		left = right = objects[start];
	else if (elem_num == 2) // 元素大于两个时，要尽心排序确定左右子树
	{
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		auto mid = start + elem_num / 2;
		left = make_shared<BVHNode>(objects, start, mid, t0, t1);
		right = make_shared<BVHNode>(objects, mid, end, t0, t1);
	}

	AABB box_left, box_right;
	if (!left->bounding_box(t0, t1, box_left) || !right->bounding_box(t0, t1, box_right))
		std::cerr << "No bounding box in bvh_node constrcutor.\n";

	box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray& ray, float tmin, float tmax, HitRecord& rec)const {
	if (!box.hit(ray, tmin, tmax)) return false;

	bool hit_left = left->hit(ray, tmin, tmax, rec);
	bool hit_right = right->hit(ray, tmin, hit_left ? rec.t : tmax, rec);
	return hit_left || hit_right;
}

bool BVHNode::bounding_box(float t0, float t1, AABB& output_box)const {
	output_box = box;
	return true;
}



//funciton
bool box_campare(const shared_ptr <Hitable> a, const shared_ptr<Hitable> b, int axis) {
	AABB box_a, box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node contructor.\n";

	return box_a.min().v[axis] < box_b.min().v[axis];
}

bool box_x_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {
	return box_campare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {
	return box_campare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {
	return box_campare(a, b, 2);
}
