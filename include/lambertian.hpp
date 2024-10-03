#pragma once

#include "../include/object.hpp"
#include "../include/random.hpp"
#include "../include/texture.hpp"
#include "../include/vec3.hpp"

namespace ZrRender {
class lambertian : public material {
 public:
  lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
  lambertian(const shared_ptr<texture>& a) : albedo(a) {}
  virtual bool scatter(const ray& r_in,
                       const hit_record& rec, 
                       color& attenuation, 
                       ray& scattered) const override 
  {
    // 这里省略了rec.p + rec.normal + random_unit_vector() - rec.p中的rec.p;
    auto scatter_direction = rec.normal + random_unit_sphere();
     // 如果散射方向为0，则取法线方向作为散射方向
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        // 散射光线时刻和入射光线一样
        scattered = ray(rec.p, scatter_direction, r_in.time);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }

 public:
  shared_ptr<texture> albedo;
};


}  // namespace ZrRender