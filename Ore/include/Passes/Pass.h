#ifndef ORE_PASS__H_
#define ORE_PASS__H_

#include "IPass.h"
#include "Shader.h"

class Pass : public IPass
{
public:
    Pass(Shader const& shader);
    Pass(Pass& other) {m_pShader = other.m_pShader;}
    ~Pass() override;

    virtual void Execute() override {};
    virtual void SetGBuffer(uint32 gbuffer) override {m_gBuffer = gbuffer;}

protected:
    sptr<Shader> m_pShader;
    uint32 m_gBuffer;

};

#endif //!RHI_IPASS__H_
