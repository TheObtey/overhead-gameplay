---@meta

---@class Node3D : Node
Node3D = {}

---@param name string The node name.
---@return Node3D
function CreateNode3D(name) end

function Node3D:GetPosition() end
function Node3D:GetX() end
function Node3D:GetY() end
function Node3D:GetZ() end
function Node3D:GetMatrixRotation() end

---Euler angles
function Node3D:GetLocalRotationRad() end
function Node3D:GetLocalRotationDeg() end

--- { w, x, y, z }
function Node3D:GetLocalRotationQuat() end
--- Angle around Y axis in Degrees
function Node3D:GetLocalYaw() end
--- AnNgle around X axis in Degrees
function Node3D:GetLocalPitch() end
--- Angle around Z axis in Degrees
function Node3D:GetLocalRoll() end
function Node3D:GetMaxScale() end
function Node3D:GetMinScale() end
function Node3D:GetScale() end
function Node3D:GetLocalRight() end
function Node3D:GetLocalUp() end
function Node3D:GetLocalForward() end

function Node3D:GetWorldMatrix() end
function Node3D:GetWorldPosition() end
function Node3D:GetWorldScale() end
--- Pitch, Yaw, Roll / x, y, z									
--- Angles in Degrees 										
function Node3D:GetWorldRotation() end
--- { w, x, y, z }												
function Node3D:GetWorldRotationQuaternion() end

--- =========== Setters ===========

---@param pos vec3
function Node3D:SetLocalPosition(pos) end

---@param x number
function Node3D:SetLocalX(x) end

---@param y number
function Node3D:SetLocalY(y) end

---@param z number
function Node3D:SetLocalZ(z) end

---@param x number
---@param y number
---@param z number
function Node3D:SetLocalRotationDeg(x, y, z) end

---@param pitch number
---@param yaw number
---@param roll number
function Node3D:SetLocalRotationRad(pitch, yaw, roll) end

---@param rot vec4
function Node3D:SetLocalRotationQuat(rot) end

---@param scale vec3
function Node3D:SetScale(scale) end

---@param worldPos vec3
function Node3D:SetWorldPosition(worldPos) end

---@param worldScale vec3
function Node3D:SetWorldScale(worldScale) end

--- Angles in Degrees
---@param worldRot vec3
function Node3D:SetWorldRotation(worldRot) end

--- Angles in Radians
---@param worldRotQuat vec4 { w, x, y, z }
function Node3D:SetWorldRotationQuaternion(worldRotQuat) end

--- =========== Adders ===========

---@param scale vec3
function Node3D:AddScale(scale) end

---@param pos vec3
function Node3D:AddLocalPosition(pos) end

---@param x number
function Node3D:AddLocalX(x) end

---@param y number
function Node3D:AddLocalY(y) end

---@param z number
function Node3D:AddLocalZ(z) end


--- Angles in Radians											
function Node3D:AddLocalRotation(rot) end

--- Angles in Radians											
---@param yaw number
function Node3D:AddLocalYaw(yaw) end

--- Angles in Radians
---@param pitch number									
function Node3D:AddLocalPitch(pitch) end

--- Angles in Radians
---@param roll number
function Node3D:AddLocalRoll(roll) end
