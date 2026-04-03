
---@type Node3D
self = Node3D

test = 0;


function OnInit()
    local node = CreateNode("Created from lua")
    print("billy")
    local n = CreateNode3D("bob")
    print("bob")
end

function OnUpdate(dt)
    -- local vec = fmaths.vec3.new()
    -- print(vec.x)

    -- print(hook)

    -- hook.Add("MyEvent", "MyFunc", function()
    --     print("Hello")
    -- end)

    -- hook.Call("MyEvent")
    -- print(timer)

    -- timer.Create("Test" .. test, 0, 1, function()
    --     print("executed from timer")
    -- end)
    -- test = test + 1;
end
