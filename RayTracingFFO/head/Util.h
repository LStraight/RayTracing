#pragma once

#include "Vec3.h"

#include <random>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#else 
#error not suport system
#endif

namespace Util {
	constexpr float PI = 3.1415926f;
	constexpr float LARGE_FLT = 98e8f;
	constexpr float DELTA_FLT = 98e-8f;
	constexpr float DEFAULT_TMIN = 0.001;

	float RandF() {
		static std::default_random_engine engine;
		static std::uniform_real_distribution<float> fMap(0.0f, 1.0f - DELTA_FLT);
		return fMap(engine);
	}

	const Vec3 RandInSphere() {
		Vec3 p;
		do {
			p = 2.0 * Vec3(RandF(), RandF(), RandF()) - Vec3(1.0f);
			//为什么要乘2？
		} while (p.Norm2() >= 1.f);

		return p;
	}

	const Vec3 RandOnSphere() {
		Vec3 p;
		do {
			p = 2.0 * Vec3(RandF(), RandF(), RandF()) - Vec3(1.0f);
		} while (p.Norm2() >= 1.0f);

		return p.Normalize();
	}

	const Vec3 RandInDisk() {
		Vec3 p;
		do {
			p = 2.0f * Vec3(RandF(), RandF(), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
		} while (p.Norm2() >= 1.0f);

		return p;
	}

	const Vec3 Gamma(const Vec3& color) {
		float r = std::pow(color.r, 1.0f / 2.2f);
		float g = std::pow(color.g, 1.0f / 2.2f);
		float b = std::pow(color.b, 1.0f / 2.2f);
		return { r,g,b };
	}

	//光线反射
	const Vec3 Reflect(const Vec3& I, const Vec3& N) {
		return I - 2.0f * I.Dot(N) * N;
	}

	template<typename U>
	const bool Refract(const Vec3& I, Vec3 N, float ior, Vec3& T) {
		float etai_etat;
		float IoN = I.Dot(N);
		bool isEntering = IoN < 0;
		if (isEntering) {
			N = -N;
			IoN = -IoN;
			etai_etat = ior;
		}
		else {
			etai_etat = 1.0f / ior;
		}
		float disctiminant = 1.0f - etai_etat * etai_etat * (1.0f - IoN * IoN);
		if (disctiminant)return false;

		T = etai_etat * (I - IoN * N) - sqrt(disctiminant) * N;
		return true;
	}

	float Fresnel_Schlick(float ior, float cosTheta) {
		float F0 = pow((ior - 1.0f) / (ior + 1.0f), 2);
		return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5);
	}

	int NumCPU() {
#ifdef WIN32
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return static_cast<int>(info.dwNumberOfProcessors);
#elif defined (__linux__)
		int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
		return cpu_num;
#else 
#error not support system
#endif
	}

	template<typename T>
	T Mean(const std::vector<T>& vals) {
		auto res = static_cast<T>(0);
		for (const auto& val : vals)
			res += val;
		return res / static_cast<T>(vals.size());
	}

	template<typename T>
	T Var(const std::vector<T>& vals) {
		auto res = static_cast<T>(0);
		auto mean = Mean(vals);
		for (const auto& val : vals) {
			auto diff = val - mean;
			res += diff * diff;
		}
		return res / static_cast<T>(vals.size());
	}

}