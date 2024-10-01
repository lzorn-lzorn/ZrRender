#pragma once

#include "../include/ray.hpp"


// #include "../include/utilities.hpp"
#include "../include/vec3.hpp"
namespace ZrRender {

struct hit_record;
class material {
 public:
   virtual bool scatter(const ray &r_in,
                        const hit_record &rec,
                        color &attenuation,
                        ray &scattered) const = 0;
};
 
}  // namespace ZrRender
