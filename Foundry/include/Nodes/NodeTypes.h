#ifndef FOUNDRY_NODETYPE__H_
#define FOUNDRY_NODETYPE__H_
#include "Define.h"

enum class NodeTypes : uint8
{
    NODE,
    NODE2D,
    NODE3D,
    NODE_CAMERA,
    NODE_MESH,
    NODE_ANIMATED_MESH,
    NODE_COLLIDER,
    NODE_RIGIDBODY,
    NODE_VIEWPORT,
    NODE_VISUAL,
    NODE_WINDOW
};

#endif