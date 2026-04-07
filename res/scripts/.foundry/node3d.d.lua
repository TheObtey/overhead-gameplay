---@meta

---@class node3d : Node
node3d = {}

---@param name string The node name.
---@return node3d
function CreateNode3D(name) end

---@return vec3
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
---@return vec3
function node3d:GetWorldPosition() end
function node3d:GetWorldScale() end
--- Pitch, Yaw, Roll / x, y, z									
--- Angles in Degrees 										
function node3d:GetWorldRotation() end
--- { w, x, y, z }												
function node3d:GetWorldRotationQuaternion() end

--- =========== Setters ===========

---@param pos vec3
function node3d:SetLocalPosition(pos) end

---@param x number
function node3d:SetLocalX(x) end

---@param y number
function node3d:SetLocalY(y) end

---@param z number
function node3d:SetLocalZ(z) end

---@param x number
---@param y number
---@param z number
function node3d:SetLocalRotationDeg(x, y, z) end

---@param pitch number
---@param yaw number
---@param roll number
function node3d:SetLocalRotationRad(pitch, yaw, roll) end

---@param rot vec4
function node3d:SetLocalRotationQuat(rot) end

---@param scale vec3
function node3d:SetScale(scale) end

---@param worldPos vec3
function node3d:SetWorldPosition(worldPos) end

---@param worldScale vec3
function node3d:SetWorldScale(worldScale) end

--- Angles in Degrees
---@param worldRot vec3
function node3d:SetWorldRotation(worldRot) end

--- Angles in Radians
---@param worldRotQuat vec4 { w, x, y, z }
function node3d:SetWorldRotationQuaternion(worldRotQuat) end

--- =========== Adders ===========

---@param scale vec3
function node3d:AddScale(scale) end

---@param pos vec3
function node3d:AddLocalPosition(pos) end

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
