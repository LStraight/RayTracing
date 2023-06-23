#pragma once

#include <RTW.h>
#include <Vec3.h>
#include <Camera.h>
#include <HitableList.h>
#include <Material.h>
#include <Sphere.h>
#include <MovingSphere.h>
#include <Aarect.h>
#include <Box.h>
#include <ConstantMedium.h>
#include <BVH.h>

#include <stb_image/rtw_stb_image.h>

void write_color(std::ostream& out, color3& color, int sample);
color3 ray_color(const Ray& ray, const color3& background, const Hitable& scene, int depth);
HitableList random_scene();
HitableList two_spheres();
HitableList two_perlin_sphere();
HitableList earth();
HitableList simple_light();
HitableList cornell_box();
HitableList cornell_smoke();
HitableList final_scene();


void write_color(std::ostream& out, color3& color, int sample) {
	float scale = 1.0f / sample;
	
	auto r = color.r;
	auto g = color.g;
	auto b = color.b;

	if (r != r) r = 0.0;
	if (g != g) g = 0.0;
	if (b != b) b = 0.0;

	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	out << static_cast<int>(256 * clamp(r, 0.0f, 0.99f)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0f, 0.99f)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0f, 0.99f)) << '\n';
}

color3 ray_color(const Ray& ray, const color3& background, const Hitable& scene, int depth) {
	HitRecord rec;
	if (depth <= 0) return color3(0, 0, 0);

    if (!scene.hit(ray, 0.001, infinity, rec))
        return background;

	//命中物体后会将命中点的信息存储在rec中，包含坐标，材质，光线到达时间，法向，正反面
	Ray scattered;
	color3 attenuation;
    color3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
    if (!rec.material->scatter(ray, rec, attenuation, scattered))
        return emitted;
    //scatter会根据rec，计算反射或折射的光线，存入scattered
	return emitted + attenuation * ray_color(scattered, background, scene, depth - 1);
}

HitableList random_scene() {
    HitableList world;

    auto checker = make_shared<CheckerTexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    auto ground_material = make_shared<Lambertian>(checker);
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

            if ((center - point3(4.f, 0.2f, 0.f)).length() > 0.9f) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    //auto center2 = center + vec3(0.f, random_float(0.f, 0.5f), 0.f);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5f, 1.f);
                    auto fuzz = random_float(0.f, 0.5f);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5f);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5f);
    world.add(make_shared<Sphere>(point3(0.f, 1.f, 0.f), 1.0f, material1));

    auto material2 = make_shared<Lambertian>(color3(0.4f, 0.2f, 0.1f));
    world.add(make_shared<Sphere>(point3(-4.f, 1.f, 0.f), 1.0f, material2));

    auto material3 = make_shared<Metal>(color3(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<Sphere>(point3(4.f, 1.f, 0.f), 1.0f, material3));

    return world;
}

HitableList two_spheres() {
    HitableList objects;
    auto checker = make_shared<CheckerTexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.0));

    objects.add(make_shared<Sphere>(point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    objects.add(make_shared<Sphere>(point3(0,  10, 0), 10, make_shared<Lambertian>(checker)));

    return objects;

}


HitableList two_perlin_sphere() {
    HitableList res;
    auto pertext = make_shared<NoiseTexture>(4);
    res.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    res.add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Metal>(color3(0.8, 0.8, 0.8), 0.0)));

    return res;
}

HitableList earth() {
    auto earth_texture = make_shared<ImageTexture>("assert/picnic.png");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto earth = make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface);

    return HitableList(earth);
}

HitableList simple_light() {
    HitableList scene;
    auto pertext = make_shared<NoiseTexture>(4);
    scene.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    scene.add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Metal>(color3(0.9, 0.9, 0.9), 0.0)));

    auto difflight = make_shared<DiffuseLight>(color3(4, 4, 4));
    scene.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    scene.add(make_shared<Sphere>(point3(0, 7, 0), 2, difflight));
    return scene;
}

HitableList cornell_box() {
    HitableList scene;

    auto red = make_shared<Lambertian>(color3(.65, .05, .05));
    auto white = make_shared<Lambertian>(color3(.73, .73, .73));
    auto green = make_shared<Lambertian>(color3(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(color3(15, 15, 15));

   scene.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
   scene.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
   scene.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
   scene.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
   scene.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
   scene.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

   shared_ptr<Hitable> box1 = make_shared<Box>(point3(0, 0, 0), point3(165, 330, 165), white);
   box1 = make_shared<Rotatey>(box1, 15);
   box1 = make_shared<Translate>(box1, vec3(256, 0, 295));
   scene.add(box1);

   shared_ptr<Hitable> box2 = make_shared<Box>(point3(0, 0, 0), point3(165, 165, 165), white);
   box2 = make_shared<Rotatey>(box2, -18);
   box2 = make_shared<Translate>(box2, vec3(130, 0, 65));
   scene.add(box2);
   return scene;
}

HitableList cornell_smoke() {
    HitableList scene;

    auto red = make_shared<Lambertian>(color3(.65, .05, .05));
    auto white = make_shared<Lambertian>(color3(.73, .73, .73));
    auto green = make_shared<Lambertian>(color3(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(color3(7, 7, 7));

    scene.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    scene.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    scene.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    scene.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    scene.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    scene.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hitable> box1 = make_shared<Box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<Rotatey>(box1, 15);
    box1 = make_shared<Translate>(box1, vec3(256, 0, 295));

    shared_ptr<Hitable> box2 = make_shared<Box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<Rotatey>(box2, -18);
    box2 = make_shared<Translate>(box2, vec3(130, 0, 65));

    scene.add(make_shared<ConstantMedium>(box1, 0.01, color3(0, 0, 0)));
    scene.add(make_shared<ConstantMedium>(box2, 0.01, color3(1, 1, 1)));

    return scene;
}

HitableList final_scene() {
    HitableList boxes1;
    auto ground = make_shared<Lambertian>(color3(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; ++i) {
        for (int j = 0; j < boxes_per_side; ++j) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_float(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<Box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }
    HitableList scene;
    scene.add(make_shared<BVHNode>(boxes1, 0, 1));
    auto light = make_shared<DiffuseLight>(color3(7, 7, 7));
    scene.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(color3(0.7, 0.3, 0.1));
    scene.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    scene.add(make_shared<Sphere>(point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    scene.add(make_shared<Sphere>(point3(0, 150, 145), 50, make_shared<Metal>(color3(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<Sphere>(point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    scene.add(boundary);
    scene.add(make_shared<ConstantMedium>(boundary, 0.2, color3(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    scene.add(make_shared<ConstantMedium>(boundary, 0.0001, color3(1, 1, 1)));

    auto imagetexture = make_shared<Lambertian>(make_shared<ImageTexture>("assert/picnic.png"));
    scene.add(make_shared<Sphere>(point3(400, 200, 400), 100, imagetexture));
    auto pertext = make_shared<NoiseTexture>(0.1);
    scene.add(make_shared<Sphere>(point3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    HitableList boxes2;
    auto white = make_shared<Lambertian>(color3(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; ++j) {
        boxes2.add(make_shared<Sphere>(point3::random(0, 165), 10, white));
    }

    scene.add(make_shared<Translate>(make_shared<Rotatey>(make_shared<BVHNode>(boxes2, 0.0, 1.0), 1.5), vec3(-100, 270, 395)));
    return scene;
}