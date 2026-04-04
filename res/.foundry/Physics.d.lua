--@meta

--@class physics
physics = {}

--@class raycast
--@field impactPos maths.vec3
--@field normal maths.vec3
--@field dist number
--@field node NodeRigidBody
physics.raycast = {}


-- --@param origin maths.vec3
-- --@param direction maths.vec3
-- function physics:Raycast(origin, direction) end

-- --@param origin maths.vec3
-- --@param direction maths.vec3
-- --@param maxDist number
-- function physics:Raycast(origin, direction, maxDist) end

--@param origin maths.vec3
--@param direction maths.vec3
--@param maxDist number
--@param mask bitsMask
function physics:Raycast(origin, direction, maxDist, mask) end


--@param origin maths.vec3
--@param direction maths.vec3
--@param maxDist number
--@param mask bitsMask
function physics:RaycastAll(origin, direction, maxDist, mask) end.

--@param gravity maths.vec3
function physics:SetGravity(gravity) end

function physics:GetGravity() end

