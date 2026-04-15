local oPlayerBaseComponent

-- Recuperation de la KeyCard
function self:Interaction()
    print("Interacted with KeyCard")
    if oPlayerBaseComponent ~= nil and oPlayerBaseComponent.bHasKeyCard == false then
        oPlayerBaseComponent.bHasKeyCard = true
        print("Player got keycard")
        print(oPlayerBaseComponent.bHasKeyCard)
    end

end


function OnInit()
    print(self:GetNode("/SceneRoot/Player"):GetName())
end

function OnUpdate(dt)
end

function OnDestroy()
end
