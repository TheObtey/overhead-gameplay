#ifndef FOUNDRY_NODEVISUAL__H_
#define FOUNDRY_NODEVISUAL__H_

#include "Node3D.h"

class NodeViewport;

class NodeVisual : public Node3D
{
public:
    NodeVisual(std::string const& name);
    ~NodeVisual() override = default;

    virtual void OnUpdate(double delta) override;
    virtual void Serialize(SerializedObject& datas) const override {};
    virtual void Deserialize(SerializedObject const& datas) override {};

    static ISerializable* CreateInstance();

private:
    bool Visible();

protected:
    NodeViewport* m_pViewport; //nearest viewport to render to
};

REGISTER_ISERIALIZABLE(NodeVisual, NodeVisual::CreateInstance);

#endif