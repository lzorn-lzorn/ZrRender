#pragma once

#include "../include/material.hpp"
#include "../include/utilities.hpp"
#include "../include/aabb.hpp"
#include "../include/texture.hpp"

namespace ZrRender {

struct hit_record 
{
  bool front_face;                    // 交点是否在正面
  double t;                           // 交点距离原点的距离
  double u, v;                        // 纹理坐标
  point3 p;  // 交点位置
  vec3 normal;
  std::shared_ptr<material> mat_ptr;  // 材质

  hit_record() = default;
  
  // 如果交点在物体的背面，则法线应该取反方向，以用于计算光照
  inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot(r.direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class object 
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const = 0;
};

class object_group : public object{
 public:
    object_group() {}
    object_group(std::shared_ptr<object> obj) { group.push_back(obj); }
    object_group(const object_group &other) { group = other.group; }
    void add(std::shared_ptr<object> obj) { group.push_back(obj); }
    // void add(object &obj) { group.push_back(std::make_shared<object>(obj)); }

    void clear() { group.clear(); }

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override 
    {
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
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const override
    {
      if(group.empty()) 
        return false;

      aabb temp_box;
      bool is_first_box = true;

      for(const auto &object : group){
        if(!object->bounding_box(t0, t1, temp_box))
          return false;

        output_box = is_first_box ? temp_box : surrounding_box(output_box, temp_box);
        is_first_box = false;
      }
      return true;
    }
 public:
    std::vector<std::shared_ptr<object>> group;
};

}  // namespace ZrRender