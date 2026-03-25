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

    static Color BLUE;
    static Color SKY_BLUE;
    static Color BLACK;
};

inline Color Color::BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
inline Color Color::SKY_BLUE = {0.0f, 0.482f, 0.631f, 1.0f};
inline Color Color::BLACK = {0.0f, 0.0f, 0.0f, 1.0f};


#endif
