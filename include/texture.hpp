#pragma once

#include "../include/utilities.hpp"
#include "../include/vec3.hpp"

namespace ZrRender{

class texture{
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};
// 纯色纹理
class solid_color : public texture{
public:
    solid_color(){}
    solid_color(const color& c):color_value(c){}

    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)){}

    virtual color value(double u, double v, const point3& p) const override{
        return color_value;
    }

private:
    color color_value;

};
// 格子纹理
class checker_texture : public texture{
public:
    checker_texture(){}
    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(_even), odd(_odd){}

    checker_texture(const color& c1, const color& c2)
        : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)){}

    virtual color value(double u, double v, const point3& p) const override {
        auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
public:
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};
}