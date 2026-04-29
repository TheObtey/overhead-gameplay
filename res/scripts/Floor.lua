local function InitializeRigidbody()

    self:SetBounciness(0.0)
    self:SetFrictionCoefficient(0.7)
    self:SetCollideWithMaskBits(1)
end


function OnInit()
    print("Floor Initializing")
    
    InitializeRigidbody()
end