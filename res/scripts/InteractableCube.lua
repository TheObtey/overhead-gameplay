-- Interaction behaviour

-- ---@class noderigidbody
-- self = self

local oPlayer
local pRootNode
local bIsEquipped = false
local bA = false
local bB = false
function self:Interaction()

end

function self.GetGrabbed()
    print("GRABB OBJ")
    oPlayer = pRootNode:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    if not oPlayer then
        print("--- Interactable Obj does not have a ref to Player --- ")
        return
    else
        print("Player found, adding child To -> ")
    end
    local oCompContainer = oPlayer:FindChild("components")
    if oCompContainer == nil then print ("CompContainer not found") return end
    local oPlayerGGunComp = oCompContainer:FindChild("GravityGunComponent")
    if oPlayerGGunComp == nil then print ("PlayerGGunComp not found") return end

    print("Player_Name :" .. oPlayer:GetName())

    self:Reparent(oPlayer, true)
    self:SetBodyType(1)

    oPlayerGGunComp.bIsHoldingObject = 1
    oPlayerGGunComp.refHeldObject = self
    -- oPlayer.bIsHoldingObject = 1
    -- oPlayer.refHeldObject = self
    print("\nName sphere = " .. self:GetName())
    print("++++++++++++++")
    print("Name RefHeldObj = " .. oPlayerGGunComp.refHeldObject:GetName())
    local vec = oPlayerGGunComp.refHeldObject:GetPosition()
    local vec2 = oPlayerGGunComp.refHeldObject:GetWorldPosition()
    print(oPlayerGGunComp.refHeldObject:GetName() .. " LOCAL POS : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    print(oPlayerGGunComp.refHeldObject:GetName() .. " WORLD POS : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    print("++++++++++++++")

    -- local vec = self:GetPosition()
    -- print("pos before sphere : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    -- local vec2 = self:GetPosition()
    -- print("POS after sphere : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    -- self:ResetForces()
    bIsEquipped = true
end

function self.GetThrown()
    oPlayer = pRootNode:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    print("THROW OBJ")
    if not oPlayer then
        print("--- Interactable Obj does not have a ref to Player --- ")
        return
    else
        print("Player found, adding child ->")
    end
    local oCompContainer = oPlayer:FindChild("components")
    if oCompContainer == nil then print ("CompContainer not found") return end
    local oPlayerGGunComp = oCompContainer:FindChild("GravityGunComponent")
    if oPlayerGGunComp == nil then print ("PlayerGGunComp not found") return end

    print("P_Name :" .. oPlayer:GetName())
    local curPos = self:GetWorldPosition()
    self:Reparent(pRootNode, true)
    print("\nRESETING POS AFTER REPARENT TO : {" .. curPos.x .. ", " .. curPos.y .. ", " .. curPos.z .. "}")
    self:SetWorldPosition(curPos)
    self:SetBodyType(2)
    self:SetBounciness(1)
    print("Bounciness = ".. self:GetBounciness())
    
    local vecForward = oPlayer:GetLocalForward()
    vecForward.z = vecForward.z * -1
    vecForward.x = vecForward.x * oPlayer.gravity

    local throwForce = vecForward * 8000
    self:ApplyWorldForceAtCenterOfMass(throwForce)

    print("Name sphere = " .. self:GetName())
    print("****************************")
    print("Name RefHeldObj = " .. oPlayerGGunComp.refHeldObject:GetName())
    local vec = oPlayerGGunComp.refHeldObject:GetPosition()
    local vec2 = oPlayerGGunComp.refHeldObject:GetWorldPosition()
    print(oPlayerGGunComp.refHeldObject:GetName() .. " LOCAL POS : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    print(oPlayerGGunComp.refHeldObject:GetName() .. " WORLD POS : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    print("****************************")
    print("||")

    oPlayerGGunComp.bIsHoldingObject = 0
    oPlayerGGunComp.refHeldObject = nil
    -- oPlayer.bIsHoldingObject = 0
    -- oPlayer.refHeldObject = nil
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
    pRootNode = self:GetParent()
    while pRootNode:GetName() ~= "SceneRoot" do
        pRootNode = pRootNode:GetParent()
    end
    print("=== INIT INTERCACTABLE CUBE, parent is : " .. pRootNode:GetName())

    self:SetBounciness(1) -- fonctionne pas dans l'init ?

    -- oPlayer = pRootNode:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    -- if not oPlayer then
    --     print("--- Interactable Obj did not find Player ref --- ")
    -- else
    --     print("--- Interactable Obj found Player ref successfully --- ")
    -- end
end
local count = 0
local i = 0
function OnUpdate(dt)
    self:SetBounciness(0.6)
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
