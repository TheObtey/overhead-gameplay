---@meta

---@class physics
Physics = {}

---@class raycast
---@field impactPos fmath.vec3
---@field normal fmath.vec3
---@field dist number
---@field node NodeRigidBody
Physics.raycast = {}


--- ---@param origin fmath.vec3
--- ---@param direction fmath.vec3
--- function Physics:Raycast(origin, direction) end

--- ---@param origin fmath.vec3
--- ---@param direction fmath.vec3
--- ---@param maxDist number
--- function Physics:Raycast(origin, direction, maxDist) end

---@param origin fmath.vec3
---@param direction fmath.vec3
---@param maxDist number
---@param mask bitsMask
function Physics:Raycast(origin, direction, maxDist, mask) end


---@param origin fmath.vec3
---@param direction fmath.vec3
---@param maxDist number
---@param mask bitsMask
function Physics:RaycastAll(origin, direction, maxDist, mask) end.

---@param gravity fmath.vec3
function Physics:SetGravity(gravity) end

function Physics:GetGravity() end

