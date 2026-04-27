local vecDefaultGravityDirection = fmath.vec3:new(0, -1, 0)
local vecCurrentGravityDirection = fmath.vec3:new(0, -1, 0)

local oOwnerBody
local oGravityApplier
local oActiveGravityField

local function NormalizeSafe(v)
    if not v then
        return fmath.vec3:new(0, -1, 0)
    end

    local len = fmath.Sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
    if len < 0.0001 then
        return fmath.vec3:new(0, -1, 0)
    end

    return fmath.vec3:new(v.x / len, v.y / len, v.z / len)
end

local function ApplyGravityDirection()
    if oGravityApplier and oGravityApplier.ApplyGravityDirection then
        oGravityApplier:ApplyGravityDirection(vecCurrentGravityDirection)
    end
end

function self:Setup(oNewOwnerBody)
    oOwnerBody = oNewOwnerBody
    vecCurrentGravityDirection = NormalizeSafe(vecDefaultGravityDirection)

    local oCompContainer = self:GetParent()
    if not oCompContainer then
        return
    end

    oGravityApplier = oCompContainer:FindChild("GravityApplierComponent")
    if not oGravityApplier then
        return
    end

    if oGravityApplier.Setup then
        oGravityApplier:Setup(oOwnerBody)
    end

    ApplyGravityDirection()
end

function self:ReceiveGravityFromField(oField, vecNewGravityDirection)
    oActiveGravityField = oField
    vecCurrentGravityDirection = NormalizeSafe(vecNewGravityDirection)
    ApplyGravityDirection()
end

function self:ClearGravityFromField(oField)
    if oActiveGravityField ~= oField then
        return
    end

    oActiveGravityField = nil
    self:ResetToDefaultGravity()
end

function self:ResetToDefaultGravity()
    vecCurrentGravityDirection = NormalizeSafe(vecDefaultGravityDirection)
    ApplyGravityDirection()
end

function self:GetCurrentGravityDirection()
    return vecCurrentGravityDirection
end

function self:HasActiveGravityField()
    return oActiveGravityField ~= nil
end
