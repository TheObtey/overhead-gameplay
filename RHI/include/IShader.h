#ifndef RHI_ISHADER__H_
#define RHI_ISHADER__H_

#include "Define.h"
#include <filesystem>

class IShader
{
public:
    virtual void Load(std::filesystem::path const& path) = 0;
    virtual void Unload() = 0;

    virtual uint32 GetId() const = 0;
};

#endif
