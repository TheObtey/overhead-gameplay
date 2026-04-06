---@meta

---@class NodeBoxCollider
NodeBoxCollider = {}

---@param name string The node name.
function CreateBoxCollider(name) end
---@param halfExtents vec3
function NodeBoxCollider:SetShape(halfExtents) end

---@class NodeSphereCollider
NodeSphereCollider = {}

---@param name string The node name.
function CreateSphereCollider(name) end
---@param radius number
function NodeSphereCollider:SetShape(radius) end



---@class NodeCapsuleCollider
NodeCapsuleCollider = {}

---@param name string The node name.
function CreateCapsuleCollider(name) end
---@param radius number
---@param height number
function NodeCapsuleCollider:SetShape(radius, height) end
