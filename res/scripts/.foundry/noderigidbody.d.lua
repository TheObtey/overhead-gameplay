---@meta

---@class noderigidbody : node
noderigidbody = {}

---@param name string 
function CreateNodeRigidBody(name) end

---@overload fun(force:fmath.vec3)
---@overload fun(forceX:number, forceY:number, forceZ:number)
function noderigidbody:ApplyLocalForceAtCenterOfMass(forceX, forceY, forceZ) end

---@param force fmath.vec3
---@param point fmath.vec3
function noderigidbody:ApplyLocalForceAtLocalPosition(force, point) end

---@param force fmath.vec3
---@param point fmath.vec3
function noderigidbody:ApplyLocalForceAtWorldPosition(force, point) end


---@overload fun(force:fmath.vec3)
---@overload fun(forceX:number, forceY:number, forceZ:number)
function noderigidbody:ApplyWorldForceAtCenterOfMass(forceX, forceY, forceZ) end

---@param force fmath.vec3
---@param point fmath.vec3
function noderigidbody:ApplyWorldForceAtLocalPosition(force, point) end

---@param force fmath.vec3
---@param point fmath.vec3
function noderigidbody:ApplyWorldForceAtWorldPosition(force, point) end


---@overload fun(torque:fmath.vec3)
---@overload fun(torqueX:number, torqueY:number, torqueZ:number)
function noderigidbody:ApplyLocalTorque(torqueX,torqueY,torqueZ) end

---@overload fun(torque:fmath.vec3)
---@overload fun(torqueX:number, torqueY:number, torqueZ:number)
function noderigidbody:ApplyWorldTorque(torqueX,torqueY,torqueZ) end


---@return fmath.vec3
function noderigidbody:GetLinearVelocity() end
---@return fmath.vec3
function noderigidbody:GetAngularVelocity() end
--- Return the linear decelerating factor
---@return number
function noderigidbody:GetLinearDamping() end
---/ Return the angular velocity damping factor
---@return number
function noderigidbody:GetAngularDamping() end
---@return fmath.vec3
function noderigidbody:GetTotalForce() end


---@overload fun(velocity:fmath.vec3)
---@overload fun(velocityX:number, velocityY:number, velocityZ:number)
function noderigidbody:SetLinearVelocity(velocityX,velocityY,velocityZ) end

---@overload fun(velocity:fmath.vec3)
---@overload fun(velocityX:number, velocityY:number, velocityZ:number)
function noderigidbody:SetAngularVelocity(velocityX,velocityY,velocityZ) end

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

---@return number
function noderigidbody:GetMass() end

---@param mass number
function noderigidbody:SetMass(mass) end

---@return number
function noderigidbody:GetBodyType() end

---@param type number
function noderigidbody:SetBodyType(type) end

--- =========== Material ===========

--- global ---

---@param bounciness number [0,1]
function noderigidbody:SetBounciness(bounciness) end
---@return number
function noderigidbody:GetBounciness() end

---@param friction number
function noderigidbody:SetFrictionCoefficient(friction) end
---@return number
function noderigidbody:GetFrictionCoefficient() end

---@param density number
function noderigidbody:SetMassDensity(density) end
---@return number
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

---@return boolean
function noderigidbody:IsAllowedToSleep() end
---@return boolean
function noderigidbody:IsSleeping() end
---@return boolean
function noderigidbody:IsGravityEnabled() end

---@param enabled boolean
function noderigidbody:SetSleepingEnabled(enabled) end

---@param isSleeping boolean
function noderigidbody:SetSleepingState(isSleeping) end

---@param enabled boolean
function noderigidbody:SetIsGravityEnabled(enabled) end
