#pragma once

#include "../include/material.hpp"
#include "../include/utilities.hpp"

namespace ZrRender {
struct hit_record {
  point3 p;  // 交点位置
  vec3 normal;
  double t;                           // 交点距离原点的距离
  bool front_face;                    // 交点是否在正面
  std::shared_ptr<material> mat_ptr;  // 材质
  // 如果交点在物体的背面，则法线应该取反方向，以用于计算光照
  inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot(r.direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
 public:
  virtual bool hit(const ray &r, double t_min, double t_max,
                   hit_record &rec) const = 0;
};

class object : public hittable {
 public:
  object() {}
  virtual bool hit(const ray &r, double t_min, double t_max,
                   hit_record &rec) const {
    return true;
  }
};

class object_group {
 public:
  object_group() {}

  void add(std::shared_ptr<object> obj) { group.push_back(obj); }
  void add(object &obj) { group.push_back(std::make_shared<object>(obj)); }

  void clear() { group.clear(); }

  bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    // 记录当前的最近的t
    auto closest_so_far = t_max;
    // 遍历每一个物体
    for (const auto &object : group) {
      if (object->hit(r, t_min, closest_so_far, temp_rec)) {
        hit_anything = true;
        // 更新最近的t
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hit_anything;
  }

 public:
  std::vector<std::shared_ptr<object>> group;
};
}  // namespace ZrRender