#pragma once

// #include "../include/material.hpp"
#include "../include/object.hpp"
#include "../include/utilities.hpp"

namespace ZrRender {
class sphere : public object {
 public:
  sphere() {}

  sphere(point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr<material> m)
        : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
  {}
  // 重载虚函数
  virtual bool hit(const ray &r, 
                   double t_min, 
                   double t_max,
                   hit_record &rec) const override 
  {
    vec3 oc = r.origin - center(r.time);
    auto a = r.direction.length_squared();
    auto half_b = dot(oc, r.direction);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) 
      return false;

    auto sqrtd = sqrt(discriminant);

    // 找到满足条件的最近的交点
    if(a < epsilon){
      // std::cerr <<"ray direction is too small" << std::flush; 
      throw std::runtime_error("from sphere->hit(): ray direction is too small");
    }
      
    auto inv_a = 1.0 / a;
    
    auto root = (-half_b - sqrtd) * inv_a;
    if (root < t_min || t_max < root) {
      root = (-half_b + sqrtd) * inv_a;
      if (root < t_min || t_max < root) 
        return false;
    }
    // 记录该交点的相关信息
    rec.t = root;
    rec.p = r.at(rec.t);
    // 法线记得归一化
    auto inv_radius = 1.0 / radius;
    vec3 outward_normal = (rec.p - center(r.time)) * inv_radius;
    // 判断交点在正面还是背面，并设置正确的法线方向
    rec.set_face_normal(r, outward_normal);
    // 记录材质
    rec.mat_ptr = mat_ptr;
    return true;
  }
  point3 center(double time) const {
    auto time_diff = time - time0;
    if (time_diff == 0) return center0;
    auto inv_time_diff = 1.0 / (time1 - time0);
    return center0 + (time_diff * inv_time_diff) * (center1 - center0);
  }
public:
  point3 center0, center1;
  double radius;
  double time0, time1;
  std::shared_ptr<material> mat_ptr;  // 材质
};
}  // namespace ZrRender