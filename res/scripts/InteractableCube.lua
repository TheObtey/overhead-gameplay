-- Interaction behaviour

-- ---@class noderigidbody
-- self = self

local oGravityComponent
local oPlayer
local pRootNode

-- local iThrowingForce = 6000

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
    local vec = oPlayerGGunComp.refHeldObject:GetPosition()
    local vec2 = oPlayerGGunComp.refHeldObject:GetWorldPosition()



    bIsEquipped = true
end

function self.GetThrown(iThrowForce)
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

    local curPos = self:GetWorldPosition()
    self:Reparent(pRootNode, true)
    self:SetWorldPosition(curPos)
    self:SetBodyType(2)
    self:SetBounciness(1)

    local oCameraRoot = oPlayer:FindChild("CameraRoot"):As(NodeTypes.NODE3D)
    local oCamera = oCameraRoot:FindChild("Camera"):As(NodeTypes.NODE_CAMERA)
    if not oCamera then
        print("PB CAM interactable cube")
        return
    end
    local vecForward = oPlayer:GetLocalForward()
    vecForward.y = oCameraRoot:GetLocalForward().y
    vecForward.z = vecForward.z * -1
    vecForward.x = vecForward.x * oPlayer.gravity

    local throwForce = vecForward *6000
    self:ApplyWorldForceAtCenterOfMass(throwForce)

    local vec = oPlayerGGunComp.refHeldObject:GetPosition()
    local vec2 = oPlayerGGunComp.refHeldObject:GetWorldPosition()

    oPlayerGGunComp.bIsHoldingObject = 0
    oPlayerGGunComp.refHeldObject = nil

    bIsEquipped = false
end

function self:SetGravityDirection(iNewGravity)
    if not oGravityComponent then return end
    oGravityComponent:SetGravityDirection(iNewGravity)
end

function OnInit()
    self:SetBodyType(2)
    self:SetIsGravityEnabled(true)
    pRootNode = self:GetParent()

    while pRootNode:GetName() ~= "SceneRoot" do
        pRootNode = pRootNode:GetParent()
    end

    oGravityComponent = self:GetNode("components/GravityComponent")
    if oGravityComponent then 
        local vecPos = self:GetWorldPosition()
        if not vecPos or type(vecPos.x) ~= "number" then return end

        local iDefaultGravity = vecPos.y < 10.0 and 1 or -1
        oGravityComponent:Setup(self, iDefaultGravity)
     end
end

local count = 0
local i = 0
function OnUpdate(dt)
    self:SetBounciness(0.6)
end

function OnDestroy() end
