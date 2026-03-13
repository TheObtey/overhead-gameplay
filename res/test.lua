
---@type Node3D
self = Node3D

test = 0;

function OnUpdate(dt)
    local node = CreateNode("Created from lua")
    print(node:GetName())
    local vec = maths.vec3.new()
    print(vec.x)

    print(hook)

    hook.Add("MyEvent", "MyFunc", function()
        print("Hello")
    end)

    hook.Call("MyEvent")
    print(timer)

    timer.Create("Test" .. test, 0, 1, function()
        print("executed from timer")
    end)
    test = test + 1;
end
