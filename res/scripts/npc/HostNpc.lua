local sNpcName = "K'sOS"
local bCanTalk = true

function self:SetNPCName(sNewName)
    sNpcName = sNewName
end

function self:GetNPCName()
    return sNpcName
end

function self:SetCanTalk(bNewCanTalk)
    bCanTalk = bNewCanTalk
end

function self:CanTalk()
    return bCanTalk
end

function self:CanInteract()
    return self:CanTalk()
end

function self:Interact(oInteractor)
    if not oInteractor then
        return
    end

    if not bCanTalk then
        print(sNpcName.." doesn't want to talk right now")
        return
    end

    self:OnInteract(oInteractor)
end

function self:OnInteract(oInteractor)
    print("Talking with "..sNpcName)
end

function self:GetPrompt()
    return "Talk with "..sNpcName
end

function OnInit()
end

function OnUpdate(iDeltaTime)
end

function OnDetroy()
end