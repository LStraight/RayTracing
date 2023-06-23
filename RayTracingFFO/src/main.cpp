#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>

#include <Function.h>

int main(){
	std::ofstream res("../output/image/022.ppm");
	if (!res) std::cerr << "failed to open file" << std::endl;

	auto wh_rate = 16.0 / 9.0;
	int image_width = 1080;
	int image_height = static_cast<int>(image_width / wh_rate);
	int samples = 500;
	int max_depth = 50;

	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	float vfov = 20.f;
	float aperture = 0.f;
	float dist_to_focus = 10.0f;
	color3 background(0, 0, 0);

	HitableList scene;

	switch (6) {
	case 1:
		background = color3(0.7, 0.8, 1.0);
		scene = random_scene();
		break;

	case 2:
		background = color3(0.7, 0.8, 1.0);
		scene = two_spheres();
		break;

	case 3:
		background = color3(0.7, 0.8, 1.0);
		scene = two_perlin_sphere();
		break;

	case 4:
		background = color3(0.7, 0.8, 1.0);
		scene = earth();
		break;

	case 5:
		background = color3(0.0, 0.0, 0.0);
		scene = simple_light();
		lookfrom = point3(26, 3, 6);
		lookat = point3(0, 2, 0);
		break;

	case 6:
		scene = cornell_box();
		wh_rate = 1.0;
		image_width = 500;
		image_height = 500;
		samples = 100;
		background = color3(0, 0, 0);
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
		break;

	case 7:
		scene = cornell_smoke();
		wh_rate = 1.0;
		image_width = 600;
		image_height = 600;
		samples = 500;
		background = color3(0, 0, 0);
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
		break;
	case 8:
		scene = final_scene();
		wh_rate = 1.0;
		wh_rate = 1.0;
		image_width = 800;
		image_height = 800;
		samples = 10000;
		background = color3(0, 0, 0);
		lookfrom = point3(478, 278, -600);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
		break;
	}

	Camera cam(lookfrom, lookat, vup, vfov, wh_rate, aperture, dist_to_focus, 0.0f, 1.0f);

	clock_t start, end;
	start = clock();
	std::cout << scene.objects.size() << std::endl;
	res << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {

		std::cerr << std::setprecision(4) << std::setiosflags(std::ios::fixed);
		std::cerr << "\rScanlines remaining: " << image_height - j <<'/' << image_height <<' '
			<< float(image_height-j) /float(image_height) * 100 << "% " << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color3 pixel_color(0, 0, 0);
			for (int s = 0; s < samples; ++s) {
				auto u = (i + random_float()) / (image_width - 1);
				auto v = (j + random_float()) / (image_height - 1);
				Ray r = cam.get_ray(u, v);
				pixel_color = pixel_color + ray_color(r, background, scene, max_depth);
			}
			write_color(res, pixel_color, samples);
		}
	}

	end = clock();
	std::cout << '\n' << "time = " << (end - start) / (CLOCKS_PER_SEC * 60) << "min"
			  << ((end - start) / (CLOCKS_PER_SEC)) % 60 << 's' << std::endl;

	return 0;
}
