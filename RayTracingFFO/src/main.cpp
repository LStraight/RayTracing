#include <iostream>
#include <fstream>
#include <string>

#include <Function.h>

std::string outputPath = "..\\output";

int main()
{
    //图像参数
    const int imageWidth = 1536;
    const auto aspectRatio = 16.0 / 9.0;
    const int imageHeight = static_cast<int>(imageWidth /aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 50;

    //output
    std::ofstream res(outputPath + "\\image\\33.ppm");
    if (!res) {
        std::cout << "failed to create file";
        return -1;
    }


    //scene
    HitableList scene = RandomScene();

    /*
    
    auto material_ground = make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Vec3(255./255., 95./255, 9.0/255.));
    //auto material_left = make_shared<Metal>(Vec3(0.8, 0.8, 0.8), 0.7);
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Vec3(0.2, 0.2, 0.2), 0.0);
    auto material_ahead = make_shared<Metal>(Vec3(0.2, 0.2, 0.2), 0.0);

    scene.Add(make_shared<Sphere>(Vec3( 0.0, -100.5, -1.0), 100, material_ground));
    scene.Add(make_shared<Sphere>(Vec3( 0.0,  0.0,   -1.0), 0.5, material_center));
    scene.Add(make_shared<Sphere>(Vec3(-1.3,  0.0,   -1.0), 0.5, material_left));
    scene.Add(make_shared<Sphere>(Vec3(-1.3, 0.0, -1.0), -0.45, material_left));
    scene.Add(make_shared<Sphere>(Vec3( 1.0,  0.0,   -1.5), 0.5, material_right));
    scene.Add(make_shared<Sphere>(Vec3(-0.5, -0.3, -0.5), 0.2, material_ahead));
    
    */

    //相机参数
    Position lookfrom(-13, 2, 4);
    Position lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    Camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, dist_to_focus);
    //cam.camPos = Position(0, 0, 10);

    res << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (size_t j = 0; j < imageHeight; ++j) {
        for (size_t i = 0; i < imageWidth; ++i) {
            Vec3 pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + RandomFloat()) / (imageWidth - 1);
                auto v = (imageHeight - (j + RandomFloat())) / (imageHeight - 1);
                Ray r = cam.GetRay(u, v);
                pixelColor += Trace(scene, r, maxDepth);
            }
            WriteColor(res, pixelColor, samplesPerPixel);
        }
    }
    res.close();

    return 0;
}

