#pragma once

#include "../include/ray.hpp"
#include "../include/vec3.hpp"
#include "../include/utilities.hpp"
#include "../include/object.hpp"
#include "../include/random.hpp"
namespace ZrRender{
// 向数组中写入一个颜色，用于最后的图像输出，用到了指针的引用传递
// 注意输入的color是[0,1]范围的
inline void write_color(unsigned char *&p, 
                        color PixelColor,
                        int SamplesPerPixel) 
{
  auto r = PixelColor.x;
  auto g = PixelColor.y;
  auto b = PixelColor.z;

  auto scale = 1.0 / SamplesPerPixel;

  // 线性空间到非线性空间的转换  gamma = 2.0 所以是平方根
  auto inv_gamma = 1.0 / 2.2;
  r = pow(scale * r, 1.0 * inv_gamma);
  g = pow(scale * g, 1.0 * inv_gamma);
  b = pow(scale * b, 1.0 * inv_gamma);

  *p++ = (unsigned char)(256 * clamp(r, 0.0, 0.999));
  *p++ = (unsigned char)(256 * clamp(g, 0.0, 0.999));
  *p++ = (unsigned char)(256 * clamp(b, 0.0, 0.999));
}

inline color ray_color(ray &r, const object_group &world, int depth, double RR) {
  hit_record rec{};

  if (world.hit(r, 0.001, inifinity, rec)) {
    // 一定概率停止弹射
    if (random_double() >= RR) return color(0, 0, 0);
    // 根据物体材质得到光线传播方向和反射率
    ray scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return attenuation * ray_color(scattered, world, depth - 1, RR) / RR;

    return color(0, 0, 0);
  }
  vec3 unit_direction = normalize(r.direction);
  auto t = 0.5 * (unit_direction.y + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

}