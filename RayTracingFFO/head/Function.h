#pragma once

#include <RTWeekend.h>
#include <HitableList.h>
#include <Sphere.h>
#include <Camera.h>
#include <Material.h>
#include <Function.h>


const Vec3 Sky(const Ray& ray);

const Vec3 Trace(HitableList& scene, Ray& ray, int depth);

void WriteColor(std::ostream& out, Vec3 color, int samplesPerPixel);

HitableList RandomScene();

const Vec3 Sky(const Ray& ray) {
    auto normDir = ray.GetDirection().Normalize();
    float t = 0.5f * (normDir.y + 1.0f);

    const Vec3 white(1.0f);
    const Vec3 blue(0.5, 0.7, 1);
    return Vec3::Lerp(white, blue, t);
}

const Vec3 Trace(HitableList& scene, Ray& ray, int depth) {
    HitRecord rec;

    if (depth <= 0) {
        return Vec3(0, 0, 0);
    }

    if (scene.Hit(ray, rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (rec.material->scatter(ray, rec, attenuation, scattered))
            return attenuation * Trace(scene, scattered, depth - 1);
        return Vec3(0, 0, 0);
    }
    return Sky(ray);

}

void WriteColor(std::ostream& out, Vec3 color, int samplesPerPixel) {
    float r = color.r;
    float g = color.g;
    float b = color.b;

    float scale = 1.0f / samplesPerPixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}


HitableList RandomScene() {
    HitableList scene;

    auto material_ground = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    scene.Add(make_shared<Sphere>(Position(0, -1000, 0), 1000, material_ground));

    for (int i = -11; i < 11; ++i) {
        for (int j = -11; j < 11; ++j) {
            auto choose_material = RandomFloat();
            Position center(i + 0.9 * RandomFloat(), 0.2, j + 0.9 * RandomFloat());

            if ((center - Position(4, 0.2, 0)).Norm() > 0.9) {
                shared_ptr<Material> sphere_material;
                if (choose_material < 0.8) {
                    auto albedo = Color::Random() * Color::Random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    scene.Add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if(choose_material > 0.95) {
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomFloat(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    scene.Add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    sphere_material = make_shared<Dielectric>(1.5);
                    scene.Add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    scene.Add(make_shared<Sphere>(Position(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<Lambertian>(Color(1, 0.4, 0.04));
    scene.Add(make_shared<Sphere>(Position(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.Add(make_shared<Sphere>(Position(4, 1, 0), 1.0, material3));
    
    return scene;
}


