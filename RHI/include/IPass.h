#ifndef RHI_IPASS__H_
#define RHI_IPASS__H_

#include "IShader.h"
#include "ITexture.h"
#include "IGeometry.h"
#include "Define.h"

#include <variant>
#include <vector>

struct Resource
{
    std::variant<IGeometry, ITexture> info;
    uint8 type;
};

class IPass 
{
public:
    IPass(IShader const& shader, std::vector<Resource> resources);
    ~IPass();

    virtual void AddShader(IShader const& shader) = 0;

    template <typename ... Args>
    void AddInput(Args ... args); 
    
    virtual uptr<ITexture> Execute() = 0;
protected:

};
#endif //!RHI_IPASS__H_
