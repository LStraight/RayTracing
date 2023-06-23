#pragma once

#include <RTW.h>
#include <Vec3.h>

class Perlin {
public:
	Perlin();
	~Perlin();

	float noise(const point3& p) const;
	float turb(const point3& p, int depth = 7) const;

private:
	static const int point_count = 256;
	Vec3* ranvec;
	int* perm_x;
	int* perm_y;
	int* perm_z;

private:
	static int* perlin_generate_perm();
	static void permute(int* p, int n);
	static float perlin_interp(vec3 c[2][2][2], float u, float v, float w);
};


Perlin::Perlin() {
	ranvec = new Vec3[point_count];
	for (int i = 0; i < point_count; ++i) {
		ranvec[i] = normalize(Vec3::random(-1,1));
	}

	perm_x = perlin_generate_perm();
	perm_y = perlin_generate_perm();
	perm_z = perlin_generate_perm();

}

Perlin::~Perlin() {
	delete[] ranvec;
	delete[] perm_x;
	delete[] perm_y;
	delete[] perm_z;
}

float Perlin::noise(const point3& p) const {
	float u = p.x - floor(p.x);
	float v = p.y - floor(p.y);
	float w = p.z - floor(p.z);

	auto i = static_cast<int>(floor(p.x));
	auto j = static_cast<int>(floor(p.y));
	auto k = static_cast<int>(floor(p.z));
	vec3 c[2][2][2];

	for (int ii = 0; ii < 2; ++ii) {
		for (int jj = 0; jj < 2; ++jj) {
			for (int kk = 0; kk < 2; ++kk) {
				c[ii][jj][kk] = ranvec[
					perm_x[(i + ii) & 255] ^
					perm_y[(j + jj) & 255] ^
					perm_z[(k + kk) & 255]
				];
			}
		}
	}

	return perlin_interp(c, u, v, w);
}

int* Perlin:: perlin_generate_perm() {
	auto p = new int[point_count];
	for (int i = 0; i < point_count; ++i) {
		p[i] = i;
	}
	permute(p, point_count);
	return p;
}

void Perlin::permute(int* p, int n) {
	for (int i = n - 1; i > 0; --i) {
		int target = random_int(0, i);
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

float Perlin::perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
	float uu = u * u * (3 - 2 * u);
	float vv = v * v * (3 - 2 * v);
	float ww = w * w * (3 - 2 * w);
	float accum = 0.f;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				vec3 weight_v(u - i, v - j, w - k);
				accum +=  (i * uu + (1 - i) * (1 - uu))
						* (j * vv + (1 - j) * (1 - vv))
						* (k * ww + (1 - k) * (1 - ww))
						* (dot(c[i][j][k], weight_v));

			}
		}
	}
	return accum;

}


float Perlin::turb(const point3& p, int depth) const {
	auto accum = 0.f;
	auto temp_p = p;
	auto weight = 1.f;

	for (int i = 0; i < depth; ++i) {
		accum += weight * noise(temp_p);
		weight *= 0.5;
		temp_p *= 2;
	}
	return fabs(accum);
}

