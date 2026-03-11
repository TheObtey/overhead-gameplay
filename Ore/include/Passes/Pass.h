#ifndef ORE_PASS__H_
#define ORE_PASS__H_

#include "IPass.h"
#include "Shader.h"

class Pass : public IPass
{
public:
    Pass(Shader& shader);
    ~Pass() override;

    virtual void Execute() override = 0;

protected:
    sptr<Shader> m_pShader;

};

#endif //!RHI_IPASS__H_
