#ifndef RHI_COLOR__H_
#define RHI_COLOR__H_

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color(float red, float green, float blue, float alpha)
    {
        r= red, g = green, b = blue, a = alpha;
    }
};

#endif
