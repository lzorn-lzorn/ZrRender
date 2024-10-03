#include "../include/camera.hpp"
#include "../include/object.hpp"
#include "../include/ray.hpp"
#include "../include/shape.hpp"
#include "../include/utilities.hpp"
#include "../include/utilities.hpp"
#include "../include/vec3.hpp"
#include "../include/material.hpp"
#include "../include/lambertian.hpp"
#include "../include/metal.hpp"
#include "../include/color.hpp"
#include "../include/dielectric.hpp"
#include "../include/random.hpp"
#include "../include/clock.hpp"
#include "../include/texture.hpp"
#include "../include/aabb.hpp"
#include "../include/BVH.hpp"
#include "../include/noise.hpp"


using namespace ZrRender;
static object_group world;
static bvh_node world_box;
// 随机构建场景
object_group& random_scene() {
    

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(
        point3(0, -1000, 0), point3(0, -1000, 0), 0.0, 1.0, 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec() * random_vec();
                    sphere_material = make_shared<lambertian>(albedo);
                    // 移动的小球
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = random_vec(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(
                        center, center, 0.0, 1.0,  0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(
                        center, center, 0.0, 1.0, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(
        point3(0, 1, 0), point3(0, 1, 0), 0.0, 1.0, 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(
        point3(-4, 1, 0), point3(-4, 1, 0), 0.0, 1.0, 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(
        point3(4, 1, 0), point3(4, 1, 0), 0.0, 1.0, 1.0, material3));

    return world;
}
object_group& two_spheres() {
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), point3(0, -1000, 0), 0.0, 1.0, 1000, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 2, 0), point3(0, 2, 0), 0.0, 1.0, 2, make_shared<lambertian>(checker)));

    return world;
}
// Perlin噪声测试场景
object_group& two_perlin_spheres() {
    auto pertext = make_shared<perlin_noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), point3(0, -1000, 0), 0.0, 1.0, 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), point3(0, 2, 0), 0.0, 1.0, 2, make_shared<lambertian>(pertext)));

    return world;
}
int main()
{
    ZrRender::clock timer;
    timer.start();

    /****图片保存，保存为png格式****/
    std::string SavePath = "/home/lzorn/ZrRender/output/";
    std::string filename = "BlendColor2.png";
    std::string filepath = SavePath + filename;
    
    /*******图片属性*******/ 
    // 宽高比
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    // 使用static_cast可以明确告诉编译器，这种损失精度的转换是在知情的情况下进行的
    // 也可以让阅读程序的其他程序员明确你转换的目的而不是由于疏忽
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int channel = 3;
    // 每个像素的采样数量
    const int samples_per_pixel = 100;
    // 光线至少弹射次数
    const int min_bounce = 45;
    // 俄罗斯轮盘赌算法生存概率
    const double RR = 0.9;
    
    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch(2)
    {
        case 0:
            world = random_scene();
            world_box = bvh_node(world.group, 0, world.group.size(), 0.0, 0.1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 1:
            world = two_spheres();
            world_box = bvh_node(world.group, 0, world.group.size(), 0.0, 0.1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;
        case 2:
            world = two_perlin_spheres();
            world_box = bvh_node(world.group, 0, world.group.size(), 0.0, 0.1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;
    }

    vec3 vup(0, 1, 0);
    double dist_to_focus = 10.0;


    /*******创建相机*******/
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 0.001);

    
    
    /******渲染部分*****/
    // 3通道图像存在一维数组中
    unsigned char* odata = (unsigned char*)malloc(image_width * image_height * channel);
    unsigned char* p = odata;
    for (int j = image_height - 1; j >= 0; --j) {
        // 标准错误流显示进度信息，单行刷新显示
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // 在像素内部随机采样
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color_with_bvh(r, world_box, min_bounce, RR);
            }
            write_color(p, pixel_color, samples_per_pixel);
        }
    }
    stbi_write_png(filepath.c_str(), image_width, image_height, channel, odata, 0);
    timer.stop();
    auto [minutes, seconds] = timer.elapsed_minutes_and_seconds();
    std::cerr << "\nDone.Elapsed time: " << minutes << " minutes and " << seconds << " seconds." << std::endl;
    return 0;
}