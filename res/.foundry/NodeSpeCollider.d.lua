--@meta

--@class NodeBoxCollider
NodeBoxCollider = {}

--@param halfExtents maths.vec3
function NodeBoxCollider:SetShape(halfExtents) end

--@class NodeSphereCollider
NodeSphereCollider = {}

--@param radius number
function NodeSphereCollider:SetShape(radius) end



--@class NodeCapsuleCollider
NodeCapsuleCollider = {}

--@param radius number
--@param height number
function NodeCapsuleCollider:SetShape(radius, height) end
