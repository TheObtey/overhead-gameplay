#ifndef RHI_IMESH__H_
#define RHI_IMESH__H_

#include "Define.h" 

class IShader;
class IMesh 
{
public:
    virtual ~IMesh() = 0;

    virtual void Draw(sptr<IShader> const shader) = 0;
};

inline IMesh::~IMesh() {};
#endif
