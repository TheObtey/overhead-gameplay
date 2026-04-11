function OnInit()
    print("Box Bounciness: " .. self:GetBounciness())
    self:SetBounciness(0.5)
    print("New Box Bounciness: " .. self:GetBounciness())
end

function OnUpdate(dt)
end

function OnDestroy()
end
