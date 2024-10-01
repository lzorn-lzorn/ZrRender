#pragma once

#include "../include/object.hpp"
#include "../include/random.hpp"
namespace ZrRender {
class lambertian : public material {
 public:
  lambertian(const color& a) : albedo(a) {}
  virtual bool scatter(const ray& r_in,
                       const hit_record& rec, 
                       color& attenuation, 
                       ray& scattered) const override 
  {
    // 这里省略了rec.p + rec.normal + random_unit_vector() - rec.p中的rec.p;
    auto scatter_direction = rec.normal + random_unit_sphere();
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

 public:
  color albedo;
};
// 在单位球体内部采样得到散射方向的材质类
class lambertian_insphere : public material {
 public:
  lambertian_insphere(const color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec,
                       color& attenuation, ray& scattered) const override {
    auto scatter_direction = rec.normal + random_in_unit_sphere();
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    }
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

 public:
  color albedo;
};

// 在半球采样得到散射方向的材质类
class lambertian_hemisphere : public material {
 public:
  lambertian_hemisphere(const color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec,
                       color& attenuation, ray& scattered) const override {
    auto scatter_direction = random_in_hemisphere(rec.normal);
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

 public:
  color albedo;
};

}  // namespace ZrRender