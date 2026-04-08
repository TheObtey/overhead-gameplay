---@meta

---@class physics
physics = {}

---@class raycast
---@field impactPos fmaths.vec3
---@field normal fmaths.vec3
---@field dist number
---@field node noderigidbody
physics.raycast = {}

---@overload fun(origin:fmath.vec3, direction:fmath.vec3)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number, mask:bitsmask)
function physics.Raycast(origin, direction, maxDist, mask) end

---@overload fun(origin:fmath.vec3, direction:fmath.vec3)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number, mask:bitsmask)
function physics:RaycastAll(origin, direction, maxDist, mask) end

---@param gravity fmaths.vec3
function physics:SetGravity(gravity) end

function physics:GetGravity() end

