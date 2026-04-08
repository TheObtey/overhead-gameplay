---@meta

---@class nodeboxbollider
nodeboxbollider = {}

---@param name string The node name.
function CreateBoxCollider(name) end
---@param halfExtents fmath.vec3
function nodeboxbollider:SetShape(halfExtents) end

---@class nodespherebollider
nodespherebollider = {}

---@param name string The node name.
function CreateSphereCollider(name) end
---@param radius number
function nodespherebollider:SetShape(radius) end



---@class nodecapsulebollider
nodecapsulebollider = {}

---@param name string The node name.
function CreateCapsuleCollider(name) end
---@param radius number
---@param height number
function nodecapsulebollider:SetShape(radius, height) end
