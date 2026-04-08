#ifndef RHI_IMESH__H_
#define RHI_IMESH__H_

namespace Ore
{
class IProgram;
class IMesh 
{
public:
    virtual void Draw(IProgram const& pProgram) const = 0;
};
}
#endif
