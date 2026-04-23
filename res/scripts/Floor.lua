local function InitializeRigidbody(iBodyType, iMass, bGravityEnabled, tAngularAxisLock, iLinearDamping, iAngularDamping)

    self:SetBounciness(0.0)
    self:SetFrictionCoefficient(1)
end


function OnInit()
    print("Floor Initializing")
    
    InitializeRigidbody()
end