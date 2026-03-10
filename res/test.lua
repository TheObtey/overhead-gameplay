function OnUpdate(dt)
    local node = CreateNode("Created from lua")
    print(node:GetName())
    local vec = maths.vec3.new()
    print(vec.x)
end
