---@meta

---@class physics
physics = {}

---@class raycast
---@field impactPos fmaths:vec3
---@field normal fmaths:vec3
---@field dist number
---@field node noderigidbody
physics.raycast = {}

--- @param origin fmaths:vec3
--- @param direction fmaths:vec3
function physics:Raycast(origin, direction) end

--- @param origin fmaths:vec3
--- @param direction fmaths:vec3
--- @param maxDist number
function physics:Raycast(origin, direction, maxDist) end

---@param origin fmaths:vec3
---@param direction fmaths:vec3
---@param maxDist number
---@param mask bitsMask
function physics:Raycast(origin, direction, maxDist, mask) end


---@param origin fmaths:vec3
---@param direction fmaths:vec3
---@param maxDist . number
---@param mask : bitsMask
function physics:RaycastAll(origin, direction, maxDist, mask) end

---@param gravity fmaths:vec3
function physics:SetGravity(gravity) end

function physics:GetGravity() end

