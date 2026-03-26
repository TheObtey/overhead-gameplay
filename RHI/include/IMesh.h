#ifndef RHI_IMESH__H_
#define RHI_IMESH__H_

#include "Define.h" 

class IProgram;
class IMesh 
{
public:
    virtual ~IMesh() = 0;

    virtual void Draw(IProgram const* pProgram) = 0;
};

inline IMesh::~IMesh() {};
#endif
