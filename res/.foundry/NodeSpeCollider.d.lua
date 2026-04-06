---@meta

---@class nodeboxcollider
nodeboxcollider = {}

---@param name string The node name.
function CreateBoxCollider(name) end
---@param halfExtents vec3
function nodeboxcollider:SetShape(halfExtents) end

---@class nodespherecollider
nodespherecollider = {}

---@param name string The node name.
function CreateSphereCollider(name) end
---@param radius number
function nodespherecollider:SetShape(radius) end



---@class nodecapsulecollider
nodecapsulecollider = {}

---@param name string The node name.
function CreateCapsuleCollider(name) end
---@param radius number
---@param height number
function nodecapsulecollider:SetShape(radius, height) end
