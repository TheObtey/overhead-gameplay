-- StateMachineComponent.lua
-- Usage : oSM:Setup(self) puis oSM:Update(dt) dans OnPhysicsUpdate du player

local oPlayer        = nil
local sCurrentState  = "IDLE"
local sPreviousState = nil
local tStates        = {}
local tTransitions   = {}

-- ─── API publique ────────────────────────────────────────────────────────────

-- Enregistre un état avec ses callbacks (tous optionnels)
function RegisterState(sName, tCallbacks)
    tStates[sName] = {
        OnEnter  = tCallbacks.OnEnter  or function()  end,
        OnUpdate = tCallbacks.OnUpdate or function()  end,
        OnExit   = tCallbacks.OnExit   or function()  end,
    }
end

-- Enregistre une transition : depuis sFrom, si fCondition() est vraie -> sTo
function RegisterTransition(sFrom, sTo, fCondition)
    if not tTransitions[sFrom] then
        tTransitions[sFrom] = {}
    end
    -- Remplace table.insert
    local n = #tTransitions[sFrom]
    tTransitions[sFrom][n + 1] = { sTo = sTo, fCondition = fCondition }
end

-- Force une transition immédiate (utile pour les events : saut, hit...)
function self:TransitionTo(sName)
    if not tStates[sName] then
        print("StateMachineComponent: état inconnu de " .. sName)
        return
    end
    if sName == sCurrentState then return end

    tStates[sCurrentState].OnExit()
    sPreviousState = sCurrentState
    sCurrentState  = sName
    tStates[sCurrentState].OnEnter()
end

function self:GetState()    return sCurrentState  end
function self:GetPrevious() return sPreviousState end
function self:IsInState(sName)
    return sCurrentState == sName
end

-- ─── Lifecycle ───────────────────────────────────────────────────────────────

function self:Setup(oOwner)
    oPlayer = oOwner
    _RegisterPlayerStates()
    _RegisterPlayerTransitions()
    -- Entre dans l'état initial
    tStates[sCurrentState].OnEnter()
    print("StateMachineComponent: initialisée sur " .. sCurrentState)
end

function self:OnUpdate(dt)
    local tCandidates = tTransitions[sCurrentState]
    if tCandidates then
        for i = 1, #tCandidates do
            if tCandidates[i].fCondition() then
                self:TransitionTo(tCandidates[i].sTo)
                break
            end
        end
    end
    tStates[sCurrentState].OnUpdate(dt)
end

-- ─── Définition des états joueur ─────────────────────────────────────────────

function _RegisterPlayerStates()
    RegisterState("IDLE", {
        OnEnter  = function() end,
        OnUpdate = function(dt) end
    })

    RegisterState("RUN", {
        OnEnter  = function() end,
        OnUpdate = function(dt) end,
        OnExit   = function() end
    })

    RegisterState("JUMP", {
        OnEnter  = function() end,
        OnUpdate = function(dt) end,
        OnExit   = function() end
    })

    RegisterState("FALL", {
        OnEnter  = function() end,
        OnUpdate = function(dt) end,
        OnExit   = function() end
    })

    RegisterState("LAND", {
        OnEnter  = function() end,
        OnUpdate = function(dt) end
    })
end

-- ─── Définition des transitions automatiques ─────────────────────────────────
-- Les conditions lisent les données du MovementComponent via oPlayer

function _RegisterPlayerTransitions()
    local function oMov()
        return oPlayer:GetNode("components/MovementComponent")
    end

    -- IDLE -> RUN : input horizontal détecté
    RegisterTransition("IDLE", "RUN", function()
        return oMov():IsMoving()
    end)

    -- RUN -> IDLE : plus de mouvement
    RegisterTransition("RUN", "IDLE", function()
        return not oMov():IsMoving()
    end)

    -- IDLE | RUN -> FALL : plus au sol sans avoir sauté
    RegisterTransition("IDLE", "FALL", function()
        return not oMov():IsGrounded()
    end)

    RegisterTransition("RUN", "FALL", function()
        return not oMov():IsGrounded()
    end)

    -- JUMP -> FALL : vitesse verticale passe en négatif
    RegisterTransition("JUMP", "FALL", function()
        return oMov():GetVerticalVelocity() < 0
    end)

    -- FALL -> LAND : atterrissage détecté
    RegisterTransition("FALL", "LAND", function()
        return oMov():IsGrounded()
    end)

    -- LAND -> IDLE ou RUN selon input (après 1 frame)
    RegisterTransition("LAND", "RUN", function()
        return oMov():IsMoving()
    end)

    RegisterTransition("LAND", "IDLE", function()
        return not oMov():IsMoving()
    end)
end