---@meta

---@class NodeRigidBody
NodeRigidBody = {}

---@param name string 
function CreateNodeRigidBody(name) end

---@param force fmath.vec3
function NodeRigidBody:ApplyLocalForceAtCenterOfMass(force) end

---@param force fmath.vec3
---@param point fmath.vec3
function NodeRigidBody:ApplyLocalForceAtLocalPosition(force, point) end

---@param force fmath.vec3
---@param point fmath.vec3
function NodeRigidBody:ApplyLocalForceAtWorldPosition(force, point) end


---@param force fmath.vec3
function NodeRigidBody:ApplyWorldForceAtCenterOfMass(force) end

---@param force fmath.vec3
---@param point fmath.vec3
function NodeRigidBody:ApplyWorldForceAtLocalPosition(force, point) end

---@param force fmath.vec3
---@param point fmath.vec3
function NodeRigidBody:ApplyWorldForceAtWorldPosition(force, point) end


---@param torque fmath.vec3
function NodeRigidBody:ApplyLocalTorque(torque) end

---@param torque fmath.vec3
function NodeRigidBody:ApplyWorldTorque(torque) end


function NodeRigidBody:GetLinearVelocity() end
function NodeRigidBody:GetAngularVelocity() end
---/ Return the linear decelerating factor
function NodeRigidBody:GetLinearDamping() end
---/ Return the angular velocity damping factor
function NodeRigidBody:GetAngularDamping() end
function NodeRigidBody:GetTotalForce() end


---@param velocity fmath.vec3
function NodeRigidBody:SetLinearVelocity(velocity) end

---@param velocity fmath.vec3
function NodeRigidBody:SetAngularVelocity(velocity) end

--- set the linear decelerating factor
---@param linearDamping number
function NodeRigidBody:SetLinearDamping(linearDamping) end

--- set the angular decelerating factor
---@param angularDamping number
function NodeRigidBody:SetAngularDamping(angularDamping) end


---@param x number
---@param y number
---@param z number
function NodeRigidBody:LockLinearAxis(x, y, z) end

---@param x number
---@param y number
---@param z number
function NodeRigidBody:LockAngularAxis(x, y, z) end

function NodeRigidBody:ResetForces() end
function NodeRigidBody:ResetTorque() end

--- =========== Mass and Body Type ===========

function NodeRigidBody:GetMass() end

---@param mass number
function NodeRigidBody:SetMass(mass) end

function NodeRigidBody:GetBodyType() end

---@param type number
function NodeRigidBody:SetBodyType(type) end

--- =========== Material ===========

--- global ---

---@param bounciness number [0,1]
function NodeRigidBody:SetBounciness(bounciness) end
function NodeRigidBody:GetBounciness() end

---@param friction number
function NodeRigidBody:SetFrictionCoefficient(friction) end
function NodeRigidBody:GetFrictionCoefficient() end

---@param density number
function NodeRigidBody:SetMassDensity(density) end
function NodeRigidBody:GetMassDensity() end

--- specific ---

---@param colliderIndex number
---@param bounciness number
function NodeRigidBody:SetBounciness(colliderIndex, bounciness) end

---@param colliderIndex number
function NodeRigidBody:GetBounciness(colliderIndex) end

---@param colliderIndex number
---@param friction number
function NodeRigidBody:SetFrictionCoefficient(colliderIndex, friction) end

---@param colliderIndex number
function NodeRigidBody:GetFrictionCoefficient(colliderIndex) end								

---@param colliderIndex number
---@param density number
function NodeRigidBody:SetMassDensity(colliderIndex, density) end

---@param colliderIndex number
function NodeRigidBody:GetMassDensity(colliderIndex) end

--- =========== Sleeping and Gravity ===========

function NodeRigidBody:IsAllowedToSleep() end
function NodeRigidBody:IsSleeping() end
function NodeRigidBody:IsGravityEnabled() end

---@param enabled boolean
function NodeRigidBody:SetSleepingEnabled(enabled) end

---@param isSleeping boolean
function NodeRigidBody:SetSleepingState(isSleeping) end

---@param enabled boolean
function NodeRigidBody:SetIsGravityEnabled(enabled) end
