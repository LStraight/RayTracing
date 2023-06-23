#pragma once

#include <RTW.h>
#include <Perlin.h>
#include <stb_image/rtw_stb_image.h>

class Texture {
public:
	virtual color3 value(float u, float v, const point3& p)const = 0;
};



class SolidColor :public Texture {
public:
	SolidColor(){}
	SolidColor(color3 c) :color_value(c) {}

	SolidColor(double red, double green, float blue) : SolidColor(color3(red, green, blue)) {}

	virtual color3 value(float u, float v, const vec3& p) const override {
		return color_value;
	}

public:
	color3 color_value;
};



class CheckerTexture :public Texture {
public:
	CheckerTexture() {}

	CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
		:even(_even), odd(_odd) {}

	CheckerTexture(color3 c1, color3 c2)
		:even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {}

	virtual color3 value(float u, float v, const point3& p) const override;

public:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};


color3 CheckerTexture::value(float u, float v, const point3& p)const {
	auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
	if (sines < 0) return odd->value(u, v, p);
	else return even->value(u, v, p);
}


class NoiseTexture : public Texture {
public:
	NoiseTexture() {}
	NoiseTexture(float sc) : scale(sc) {}
	virtual color3 value(float u, float v, const point3& p) const override {
		return color3(1, 1, 1) * 0.5f * (1 + sin(scale * p.z + 10 * noise.turb(p)));
	}

public:
	Perlin noise;
	float scale;
};

class ImageTexture :public Texture {
public:
	const static int bytes_per_pixel = 3;

	ImageTexture():data(nullptr), width(0), height(0), bytes_per_scanline(0){}
	ImageTexture(const char* filename);
	~ImageTexture() { delete data; }
	virtual color3 value(float u, float v, const vec3& p) const override;

public:
	unsigned char* data;
	int width, height;
	int bytes_per_scanline;
};

ImageTexture::ImageTexture(const char* filename) {
	auto components_per_pixel = bytes_per_pixel;

	data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
	if (!data) {
		std::cerr << "could not load the image in " << filename << std::endl;
		width = height = 0;
	}
	bytes_per_scanline = bytes_per_pixel * width;
}

color3 ImageTexture::value(float u, float v, const vec3& p) const {
	if (data==nullptr) return color3(0, 1, 1);

	u = clamp(u, 0.0, 1.0);
	v = 1.0 - clamp(v, 0.0, 1.0);

	auto i = static_cast<int>(u * width);
	auto j = static_cast<int>(v * height);

	if (i >= width) i = width - 1;
	if (j >= height) j = height - 1;

	//std::cout << "width: " << width << "  height: " << height << std::endl;

	const auto cs = 1.0 / 255.0;
	auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
	return color3(cs * pixel[0], cs * pixel[1], cs * pixel[2]);
}

