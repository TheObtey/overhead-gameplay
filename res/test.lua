function OnUpdate(dt)
    local node = CreateNode("Created from lua")
    print(node:GetName())
    local vec = Vec3.new()
    print(vec:GetX())
end
