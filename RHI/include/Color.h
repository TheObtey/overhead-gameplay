#ifndef RHI_COLOR__H_
#define RHI_COLOR__H_

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color(float const red, float const green, float const blue, float const alpha)
    {
        r= red, g = green, b = blue, a = alpha;
    }

    static Color BLUE;
    static Color SKY_BLUE;
    static Color BLACK;
    static Color RED;
};

inline Color Color::BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
inline Color Color::SKY_BLUE = {0.0f, 0.482f, 0.631f, 1.0f};
inline Color Color::BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
inline Color Color::RED = {1.0f, 0.0f, 0.0f, 1.0f};


#endif
