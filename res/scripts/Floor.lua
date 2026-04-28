local function InitializeRigidbody()

    self:SetBounciness(0.0)
    self:SetFrictionCoefficient(1)
    self:SetCollideWithMaskBits(1)
end


function OnInit()
    print("Floor Initializing")
    
    InitializeRigidbody()
end