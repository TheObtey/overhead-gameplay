#ifndef __INSPECTOR_H
#define __INSPECTOR_H

#include <string>
#include <Serialization/json.hpp>

using json = nlohmann::json;

class InspectorNodePropreties {
public:
    bool Draw(json& publicDataJson);
    void SetWindow(int width, int height) { m_screenWidth = width; m_screenHeight = height; }

private:
    int m_screenWidth = 0;
    int m_screenHeight = 0;
};

#endif