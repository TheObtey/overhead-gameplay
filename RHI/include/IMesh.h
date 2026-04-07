#ifndef RHI_IMESH__H_
#define RHI_IMESH__H_

class IProgram;
class IMesh 
{
public:
    virtual void Draw(IProgram const& pProgram) const = 0;
};
#endif
