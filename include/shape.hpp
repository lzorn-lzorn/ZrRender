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
      // 计算纹理坐标
      get_sphere_uv(outward_normal, rec.u, rec.v);
      // 记录材质
      rec.mat_ptr = mat_ptr;
      return true;
    }
    point3 center(double time) const 
    {
      auto time_diff = time - time0;
      if (time_diff == 0) return center0;
      auto inv_time_diff = 1.0 / (time1 - time0);
      return center0 + (time_diff * inv_time_diff) * (center1 - center0);
    }

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override 
    {
      aabb box0(center(time0)-vec3(radius,radius,radius)
              , center(time0)+vec3(radius,radius,radius));
      aabb box1(center(time1)-vec3(radius,radius,radius)
              , center(time1)+vec3(radius,radius,radius));

      output_box = surrounding_box(box0, box1);
      return true;
    }

public:
    point3 center0, center1;
    double radius;
    double time0, time1;
    std::shared_ptr<material> mat_ptr;  // 材质

private:
    // 计算给定球面上的点p的纹理坐标，p是圆心在原点的单位球面上的坐标，一般用归一化的法线
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        auto theta = std::acos(-p.y);
        auto phi = std::atan2(-p.z, p.x) + pi;

        auto inv_pi = 1.0 / pi;
        u = phi * inv_pi * 0.5;
        v = theta * inv_pi;
    }
};
}  // namespace ZrRender