local nTest
local nTestChild

function OnGameStarted()
    print("GameStarted")
end

function OnInit()
    nTest = CreateNode3D("bob")
    nTestChild = CreateNode3D("bobChild")

    print("billy")

    LoadNode("res/scripts/prefabs/TestEdit.nd.json")
    print("billy")

    -- local vec = fmaths.vec3:new(0,0,2)
    -- print("vec = (", vec.x,",", vec.y,",", vec.z,")" )
    -- nTest:SetWorldPosition(vec)
    -- vec.z = -vec.z
    -- print("vec = (", vec.x,",", vec.y,",", vec.z,")" )
    -- nTestChild.SetWorldPosition(nTestChild, vec)
    -- print("bob" )
    -- local pos = nTest:GetWorldPosition()
    -- print("Init pos = (", pos.x,",", pos.y,",", pos.z,")" )

    -- pos = nTestChild:GetWorldPosition()
    -- print("InitChild pos = (", pos.x,",", pos.y,",", pos.z,")" )

    -- self:AddChild(nTest)
    -- self:AddChild(nTestChild)

    -- self:GetSceneTree().OnGameStarted = OnGameStarted
end

function OnUpdate(dt)
    --  local vec1 = fmaths.vec3:new(0,0,7)
    -- nTest.SetWorldPosition(nTest, vec1)

    -- local vec = fmaths.vec3:new(0,2,0)
    -- -- nTestChild.SetLocalPosition(nTestChild, vec)
    -- nTestChild.AddLocalPosition(nTestChild, vec)
    -- local pos = nTestChild:GetWorldPosition()

    -- print("Update pos = (", pos.x,",", pos.y,",", pos.z,")" )
end
