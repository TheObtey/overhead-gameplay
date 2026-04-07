---@meta

---@class noderigidbody
noderigidbody = {}

---@param name string 
function CreateNodeRigidBody(name) end

---@param force vec3
function noderigidbody:ApplyLocalForceAtCenterOfMass(force) end

---@param force vec3
---@param point vec3
function noderigidbody:ApplyLocalForceAtLocalPosition(force, point) end

---@param force vec3
---@param point vec3
function noderigidbody:ApplyLocalForceAtWorldPosition(force, point) end


---@param force vec3
function noderigidbody:ApplyWorldForceAtCenterOfMass(force) end

---@param force vec3
---@param point vec3
function noderigidbody:ApplyWorldForceAtLocalPosition(force, point) end

---@param force vec3
---@param point vec3
function noderigidbody:ApplyWorldForceAtWorldPosition(force, point) end


---@param torque vec3
function noderigidbody:ApplyLocalTorque(torque) end

---@param torque vec3
function noderigidbody:ApplyWorldTorque(torque) end


function noderigidbody:GetLinearVelocity() end
function noderigidbody:GetAngularVelocity() end
---/ Return the linear decelerating factor
function noderigidbody:GetLinearDamping() end
---/ Return the angular velocity damping factor
function noderigidbody:GetAngularDamping() end
function noderigidbody:GetTotalForce() end


---@param velocity vec3
function noderigidbody:SetLinearVelocity(velocity) end

---@param velocity vec3
function noderigidbody:SetAngularVelocity(velocity) end

--- set the linear decelerating factor
---@param linearDamping number
function noderigidbody:SetLinearDamping(linearDamping) end

--- set the angular decelerating factor
---@param angularDamping number
function noderigidbody:SetAngularDamping(angularDamping) end


---@param x number
---@param y number
---@param z number
function noderigidbody:LockLinearAxis(x, y, z) end

---@param x number
---@param y number
---@param z number
function noderigidbody:LockAngularAxis(x, y, z) end

function noderigidbody:ResetForces() end
function noderigidbody:ResetTorque() end

--- =========== Mass and Body Type ===========

function noderigidbody:GetMass() end

---@param mass number
function noderigidbody:SetMass(mass) end

function noderigidbody:GetBodyType() end

---@param type number
function noderigidbody:SetBodyType(type) end

--- =========== Material ===========

--- global ---

---@param bounciness number [0,1]
function noderigidbody:SetBounciness(bounciness) end
function noderigidbody:GetBounciness() end

---@param friction number
function noderigidbody:SetFrictionCoefficient(friction) end
function noderigidbody:GetFrictionCoefficient() end

---@param density number
function noderigidbody:SetMassDensity(density) end
function noderigidbody:GetMassDensity() end

--- specific ---

---@param colliderIndex number
---@param bounciness number
function noderigidbody:SetBounciness(colliderIndex, bounciness) end

---@param colliderIndex number
function noderigidbody:GetBounciness(colliderIndex) end

---@param colliderIndex number
---@param friction number
function noderigidbody:SetFrictionCoefficient(colliderIndex, friction) end

---@param colliderIndex number
function noderigidbody:GetFrictionCoefficient(colliderIndex) end								

---@param colliderIndex number
---@param density number
function noderigidbody:SetMassDensity(colliderIndex, density) end

---@param colliderIndex number
function noderigidbody:GetMassDensity(colliderIndex) end

--- =========== Sleeping and Gravity ===========

function noderigidbody:IsAllowedToSleep() end
function noderigidbody:IsSleeping() end
function noderigidbody:IsGravityEnabled() end

---@param enabled boolean
function noderigidbody:SetSleepingEnabled(enabled) end

---@param isSleeping boolean
function noderigidbody:SetSleepingState(isSleeping) end

---@param enabled boolean
function noderigidbody:SetIsGravityEnabled(enabled) end
