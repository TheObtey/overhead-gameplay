---@meta

---@class physics
physics = {}

---@class raycast
---@field impactPos fmath.vec3
---@field normal fmath.vec3
---@field dist number
---@field node noderigidbody
physics.raycast = {}

---@overload fun(origin:fmath.vec3, direction:fmath.vec3)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number, mask:bitsmask)
---@return raycast
function physics.Raycast(origin, direction, maxDist, mask) end

---@overload fun(origin:fmath.vec3, direction:fmath.vec3)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number)
---@overload fun(origin:fmath.vec3, direction:fmath.vec3, maxDist:number, mask:bitsmask)
---@return raycast LIST
function physics.RaycastAll(origin, direction, maxDist, mask) end

---@param gravity fmath.vec3
function physics.SetGravity(gravity) end

function physics.GetGravity() end
