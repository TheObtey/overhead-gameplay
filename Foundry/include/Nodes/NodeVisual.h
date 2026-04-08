#ifndef FOUNDRY_NODEVISUAL__H_
#define FOUNDRY_NODEVISUAL__H_

#include "Node3D.h"

class NodeViewport;

class NodeVisual : public Node3D
{
public:
    explicit NodeVisual(std::string const& name);
    ~NodeVisual() override = default;

    virtual void Serialize(SerializedObject& datas) const override {};
    virtual void Deserialize(SerializedObject const& datas) override {};

    static ISerializable* CreateInstance();

protected:
    virtual bool IsVisible();
    void TryAttachToViewport();

protected:
    NodeViewport* m_pViewport = nullptr; //nearest viewport to render to
};

REGISTER_ISERIALIZABLE(NodeVisual, NodeVisual::CreateInstance);

#endif
