#pragma once

#include "../include/vec3.hpp"
namespace ZrRender {
  


inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

inline vec3 random_vec() {
  return vec3(random_double(), random_double(), random_double());
}
inline vec3 random_vec(double min, double max) {
  return vec3(random_double(min, max), random_double(min, max),
              random_double(min, max));
}
inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}
inline point3 random_in_unit_sphere() {
  point3 p{};
  do {
    p = random_vec(-1.0, 1.0);
  } while (p.length_squared() >= 1.0);
  return p;
}
inline vec3 random_unit_sphere() { return normalize(random_in_unit_sphere()); }
// 在半球内随机取点
inline vec3 random_in_hemisphere(const vec3 &normal) {
  vec3 in_unit_sphere = random_in_unit_sphere();
  // 判断该偏移量是否落入了下半球，如果落入下半球则偏移量应该取反
  if (dot(in_unit_sphere, normal) > 0.0)
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

// 生成单位圆盘内随机一点
inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
};