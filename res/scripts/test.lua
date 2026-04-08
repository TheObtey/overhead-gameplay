local nTest
local nTestNode
local nTestChild
local nTestRB
local nTestC

function OnGameStarted()
    print("GameStarted")
end

function OnInit()
    nTestNode = CreateNode("bobNode")
    nTest = CreateNode3D("bob")
    nTestChild = CreateNode3D("bobChild")
    nTestRB = CreateNodeRigidBody("RB")
    nTestC = CreateNodeBoxCollider("C")

    print("NAME _ ".. nTestNode.GetName(nTestNode))
    print("NAME _ ".. nTestChild.GetName(nTestChild))



    -- local pos = nTestRB:GetWorldPosition()
    -- print("Init pos RB = (", pos.x,",", pos.y,",", pos.z,")" )
    print("NAME _ "..nTestRB.GetName(nTestRB))
    print("NAME _ ".. nTestC.GetName(nTestC))
    -- print("billy")
    -- local vec = fmath.vec3:new(0,0,2)
    -- print("vec = (", vec.x,",", vec.y,",", vec.z,")" )
    -- nTest:SetWorldPosition(vec)
    -- vec.z = -vec.z
    -- print("vec = (", vec.x,",", vec.y,",", vec.z,")" )
    -- nTestChild.SetWorldPosition(nTestChild, vec)
    -- print("bob" )
    print("billy")

    LoadNode("res/scripts/prefabs/TestEdit.nd.json")
    print("billy")

    -- self:AddChild(nTest)
    -- self:AddChild(nTestChild)

    self:GetSceneTree().OnGameStarted = OnGameStarted
end

function OnUpdate(dt)

end



