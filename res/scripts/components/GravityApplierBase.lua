-- Base interface for gravity appliers.
-- Components extending this should override Setup and ApplyGravityDirection.

function self:Setup(oOwnerBody)
    -- Interface method
end

function self:ApplyGravityDirection(vecGravityDirection)
    -- Interface method
end
