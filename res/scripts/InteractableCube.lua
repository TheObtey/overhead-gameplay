-- Interaction behaviour

-- ---@class noderigidbody
-- self = self

local oPlayer
local oRoot
local bIsEquipped = false
local bA = false
local bB = false
function self:Interaction()

end

function self.GetGrabbed()
    print("GRABB OBJ")
    oPlayer = oRoot:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    if not oPlayer then
        print("--- Interactable Obj does not have a ref to Player --- ")
        return
    else
        print("Player found, adding child To -> ")
    end
    print("Player_Name :" .. oPlayer:GetName())

    self:Reparent(oPlayer, true)
    self:SetBodyType(1)
    oPlayer.bIsHoldingObject = 1
    oPlayer.refHeldObject = self
    print("\nName sphere = " .. self:GetName())
    print("++++++++++++++")
    print("Name RefHeldObj = " .. oPlayer.refHeldObject:GetName())
    local vec = oPlayer.refHeldObject:GetPosition()
    local vec2 = oPlayer.refHeldObject:GetWorldPosition()
    print(oPlayer.refHeldObject:GetName() .. " LOCAL POS : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    print(oPlayer.refHeldObject:GetName() .. " WORLD POS : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    print("++++++++++++++")

    -- local vec = self:GetPosition()
    -- print("pos before sphere : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    -- local vec2 = self:GetPosition()
    -- print("POS after sphere : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    -- self:ResetForces()
    bIsEquipped = true
end

function self.GetThrown()
    oPlayer = oRoot:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    print("THROW OBJ")
    if not oPlayer then
        print("--- Interactable Obj does not have a ref to Player --- ")
        return
    else
        print("Player found, adding child ->")
    end
    print("P_Name :" .. oPlayer:GetName())
    local curPos = self:GetWorldPosition()
    self:Reparent(oRoot, true)
    print("\nRESETING POS AFTER REPARENT TO : {" .. curPos.x .. ", " .. curPos.y .. ", " .. curPos.z .. "}")
    self:SetWorldPosition(curPos)
    self:SetBodyType(2)
    self:SetIsGravityEnabled(true)

    print("Name sphere = " .. self:GetName())
    print("****************************")
    print("Name RefHeldObj = " .. oPlayer.refHeldObject:GetName())
    local vec = oPlayer.refHeldObject:GetPosition()
    local vec2 = oPlayer.refHeldObject:GetWorldPosition()
    print(oPlayer.refHeldObject:GetName() .. " LOCAL POS : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    print(oPlayer.refHeldObject:GetName() .. " WORLD POS : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    print("****************************")
    print("||")
    oPlayer.bIsHoldingObject = 0
    oPlayer.refHeldObject = nil
    -- local vec = self:GetPosition()
    -- print("pos before sphere : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    -- local vec2 = self:GetPosition()
    -- print("POS after sphere : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")

    bIsEquipped = false
end

function CheckParent(node)

    return 
end

function OnInit()

    self:SetBodyType(2)

    self:SetIsGravityEnabled(true)
    oRoot = self:GetParent()
    while oRoot:GetName() ~= "SceneRoot" do
    oRoot = oRoot:GetParent()
end
    print("=== INIT INTERCACTABLE CUBE, parent is : ".. oRoot:GetName())

    -- oPlayer = oRoot:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    -- if not oPlayer then
    --     print("--- Interactable Obj did not find Player ref --- ")
    -- else
    --     print("--- Interactable Obj found Player ref successfully --- ")
    -- end
end

local count = 0
local i = 0
function OnUpdate(dt)

        -- DEBUG Pos toutes les 4000 frames
    -- count = count + 1
    -- if count > 4000 then
    --     print( "======")
    --         local vec = self:GetPosition()
    -- local vec2 = self:GetWorldPosition()
    --     print(self:GetName() .. " LOCAL POS : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    --     print(self:GetName() .. " WORLD POS : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    --     print( "======")
    --     -- print("Player Forward : {"..self:GetLocalForward().x..";"..self:GetLocalForward().y..";"..self:GetLocalForward().z.."}")
    --     count = 0
    -- end
end

function OnDestroy() end
