---@meta

---@class node3d : Node
node3d = {}

---@param name string The node name.
---@return node3d
function CreateNode3D(name) end

---@return fmath.vec3
function node3d:GetPosition() end
function node3d:GetX() end
function node3d:GetY() end
function node3d:GetZ() end
function node3d:GetMatrixRotation() end

---Euler angles
function node3d:GetLocalRotationRad() end
function node3d:GetLocalRotationDeg() end

--- { w, x, y, z }
function node3d:GetLocalRotationQuat() end
--- Angle around Y axis in Degrees
function node3d:GetLocalYaw() end
--- AnNgle around X axis in Degrees
function node3d:GetLocalPitch() end
--- Angle around Z axis in Degrees
function node3d:GetLocalRoll() end
function node3d:GetMaxScale() end
function node3d:GetMinScale() end
function node3d:GetScale() end
function node3d:GetLocalRight() end
function node3d:GetLocalUp() end
function node3d:GetLocalForward() end

function node3d:GetWorldMatrix() end
---@return fmath.vec3
function node3d:GetWorldPosition() end
function node3d:GetWorldScale() end
--- Pitch, Yaw, Roll / x, y, z									
--- Angles in Degrees 										
function node3d:GetWorldRotation() end
--- { w, x, y, z }												
function node3d:GetWorldRotationQuaternion() end

--- =========== Setters ===========

---@overload fun(pos:fmath.vec3)
---@overload fun(posX:number, posY:number, posZ:number)
function node3d:SetLocalPosition(posX,posY,posZ) end

---@param x number
function node3d:SetLocalX(x) end

---@param y number
function node3d:SetLocalY(y) end

---@param z number
function node3d:SetLocalZ(z) end

---@overload fun(rotDeg:fmath.vec3)
---@overload fun(rX:number, rY:number, rZ:number)
function node3d:SetLocalRotationDeg(rX, rY, rZ) end

---@overload fun(rotRad:fmath.vec3)
---@overload fun(rX:number, rY:number, rZ:number)
function node3d:SetLocalRotationRad(rX, rY, rZ) end

---@param rot fmath.vec4
function node3d:SetLocalRotationQuat(rot) end

---@overload fun(scale:fmath.vec3)
---@overload fun(sX:number, sY:number, sZ:number)
function node3d:SetScale(sX, sY, sZ) end

---@overload fun(pos:fmath.vec3)
---@overload fun(posX:number, posY:number, posZ:number)
function node3d:SetWorldPosition(posX,posY,posZ) end

---@overload fun(scale:fmath.vec3)
---@overload fun(sX:number, sY:number, sZ:number)
function node3d:SetWorldScale(sX,sY,sZ) end

--- Angles in Degrees
---@overload fun(angle:fmath.vec3)
---@overload fun(rX:number, rY:number, rZ:number)
function node3d:SetWorldRotation(rX, rY, rZ) end

--- Angles in Radians
---@param worldRotQuat fmath.vec4 { w, x, y, z }
function node3d:SetWorldRotationQuaternion(worldRotQuat) end

--- =========== Adders ===========

---@overload fun(scale:fmath.vec3)
---@overload fun(sX:number, sY:number, sZ:number)
function node3d:AddScale(sX,sY,sZ) end

---@overload fun(pos:fmath.vec3)
---@overload fun(posX:number, posY:number, posZ:number)
function node3d:AddLocalPosition(posX,posY,posZ) end

---@param x number
function node3d:AddLocalX(x) end

---@param y number
function node3d:AddLocalY(y) end

---@param z number
function node3d:AddLocalZ(z) end


--- Angles in Radians											
function node3d:AddLocalRotation(rot) end

--- Angles in Radians											
---@param yaw number
function node3d:AddLocalYaw(yaw) end

--- Angles in Radians
---@param pitch number									
function node3d:AddLocalPitch(pitch) end

--- Angles in Radians
---@param roll number
function node3d:AddLocalRoll(roll) end
