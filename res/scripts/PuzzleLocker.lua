local oCollider

local inseredPieces = 0
local piecesRequired = 3

function PieceCollector(rigidbody)
    print("NICE ANOTHER PIECE !!!!")
    --local shortName = string.sub(rigidbodyName, 1, 5)
    --print("Short Name : ".. shortName)
    
    if rigidbody:GetName() == "Piece1" then -- todo: change
        if inseredPieces < piecesRequired then
            inseredPieces = inseredPieces + 1 
            print("PIECES: " .. inseredPieces .. "/" .. piecesRequired)
        end
    end
end

function OnInit()
    oCollider = self:FindChild("PuzzleTrigger"):As(NodeTypes.NODE_COLLIDER)
    if oCollider ~= nil then
        print("Iintializing oCollider with name : ".. oCollider:GetName())

        oCollider:SetIsTrigger(false) -- foncitonne pas dans le init => le collider est probablement pas existant au init du rigidbody

        print(oCollider:IsTrigger(), oCollider:IsSimulationCollider(), oCollider:IsWorldQueryCollider())

        -- Attach OnBodyEntered method of collider to a callback
        local id = oCollider:SubscribeOnTrigger(function(collider, rigidbody) -- Exemple Event 1
            print("TRIGGER avec : ".. rigidbody:GetName())
            PieceCollector(rigidbody)
        end)
        oCollider:SubscribeOnTrigger(function(collider, rigidbody) -- Exemple Event 2
            print("ANOTHER ONE " .. rigidbody:GetName())
        end)
    else
        print("oCollider est nil dans OnInit (PuzzleTrigger introuvable ou pas encore un collider)")
    end
end

function OnBodyEntered(oBody)
    -- Do shit
    print("YAA ZEBI")
end

local bColliderInitialized = false
local bColliderInitialized2 = false
function OnUpdate(iDelta)
 
    if not bColliderInitialized then
        oCollider:SetIsTrigger(true)
        oCollider:SetIsWorldQueryCollider(false)
        bColliderInitialized = true
    end
end