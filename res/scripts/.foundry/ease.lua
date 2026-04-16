ease = {}
ease.In = {}
ease.Out = {}
ease.InOut = {}

-- Linear
function ease.Linear(iT)
    return iT
end

-- Sine
function ease.In.Sine(iT)
    return 1 - fmath.Cos((iT * fmath.Pi) / 2)
end

function ease.Out.Sine(iT)
    return fmath.Sin((iT * fmath.Pi) / 2)
end

function ease.InOut.Sine(iT)
    return -(fmath.Cos(fmath.Pi * iT) - 1) / 2
end

-- Quad
function ease.In.Quad(iT)
    return iT * iT
end

function ease.Out.Quad(iT)
    return 1 - (1 - iT) * (1 - iT)
end

function ease.InOut.Quad(iT)

    if iT < 0.5 then
        return 2 * iT * iT
    end

    return 1 - ((-2 * iT + 2) ^ 2) / 2

end

-- Cubic
function ease.In.Cubic(iT)
    return iT * iT * iT
end

function ease.Out.Cubic(iT)
    return 1 - ((1 - iT) ^ 3)
end

function ease.InOut.Cubic(iT)

    if iT < 0.5 then
        return 4 * iT * iT * iT
    end

    return 1 - ((-2 * iT + 2) ^ 3) / 2

end

-- Quart
function ease.In.Quart(iT)
    return iT ^ 4
end

function ease.Out.Quart(iT)
    return 1 - (1 - iT) ^ 4
end

function ease.InOut.Quart(iT)

    if iT < 0.5 then
        return 8 * iT ^ 4
    end

    return 1 - ((-2 * iT + 2) ^ 4) / 2

end

-- Quint
function ease.In.Quint(iT)
    return iT ^ 5
end

function ease.Out.Quint(iT)
    return 1 - (1 - iT) ^ 5
end

function ease.InOut.Quint(iT)

    if iT < 0.5 then
        return 16 * iT ^ 5
    end

    return 1 - ((-2 * iT + 2) ^ 5) / 2

end

-- Expo
function ease.In.Expo(iT)
    if iT == 0 then return 0 end
    return 2 ^ (10 * iT - 10)
end

function ease.Out.Expo(iT)
    if iT == 1 then return 1 end
    return 1 - 2 ^ (-10 * iT)
end

function ease.InOut.Expo(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    if iT < 0.5 then
        return (2 ^ (20 * iT - 10)) / 2
    end

    return (2 - 2 ^ (-20 * iT + 10)) / 2

end

-- Circ
function ease.In.Circ(iT)
    return 1 - fmath.Sqrt(1 - iT * iT)
end

function ease.Out.Circ(iT)
    return fmath.Sqrt(1 - (iT - 1) ^ 2)
end

function ease.InOut.Circ(iT)

    if iT < 0.5 then
        return (1 - fmath.Sqrt(1 - (2 * iT) ^ 2)) / 2
    end

    return (fmath.Sqrt(1 - (-2 * iT + 2) ^ 2) + 1) / 2

end

-- Back
local iC1 = 1.70158
local iC2 = iC1 * 1.525

function ease.In.Back(iT)
    return (iC1 + 1) * iT * iT * iT - iC1 * iT * iT
end

function ease.Out.Back(iT)
    return 1 + (iC1 + 1) * (iT - 1) ^ 3 + iC1 * (iT - 1) ^ 2
end

function ease.InOut.Back(iT)

    if iT < 0.5 then
        return ((2 * iT) ^ 2 * ((iC2 + 1) * 2 * iT - iC2)) / 2
    end

    return ((2 * iT - 2) ^ 2 * ((iC2 + 1) * (iT * 2 - 2) + iC2) + 2) / 2

end

-- Bounce
local function _bounceOut(iT)

    if iT < 1 / 2.75 then
        return 7.5625 * iT * iT
    elseif iT < 2 / 2.75 then
        iT = iT - (1.5 / 2.75)
        return 7.5625 * iT * iT + 0.75
    elseif iT < 2.5 / 2.75 then
        iT = iT - (2.25 / 2.75)
        return 7.5625 * iT * iT + 0.9375
    end

    iT = iT - (2.625 / 2.75)
    return 7.5625 * iT * iT + 0.984375

end

function ease.Out.Bounce(iT)
    return _bounceOut(iT)
end

function ease.In.Bounce(iT)
    return 1 - _bounceOut(1 - iT)
end

function ease.InOut.Bounce(iT)

    if iT < 0.5 then
        return (1 - _bounceOut(1 - 2 * iT)) / 2
    end

    return (1 + _bounceOut(2 * iT - 1)) / 2

end

-- Elastic
local iC4 = (2 * fmath.Pi) / 3
local iC5 = (2 * fmath.Pi) / 4.5

function ease.In.Elastic(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    return -2 ^ (10 * iT - 10) * fmath.Sin((iT * 10 - 10.75) * iC4)

end

function ease.Out.Elastic(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    return 2 ^ (-10 * iT) * fmath.Sin((iT * 10 - 0.75) * iC4) + 1

end

function ease.InOut.Elastic(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    if iT < 0.5 then
        return -(2 ^ (20 * iT - 10) *
            fmath.Sin((20 * iT - 11.125) * iC5)) / 2
    end

    return (2 ^ (-20 * iT + 10) *
        fmath.Sin((20 * iT - 11.125) * iC5)) / 2 + 1

end

return ease
