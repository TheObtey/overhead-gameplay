---@meta

---@class fmath

--========================
-- VECTORS
--========================

---@class fmath.vec2
---@field x number
---@field y number
local vec2 = {}

---@overload fun():fmath.vec2
---@overload fun(v:number):fmath.vec2
---@overload fun(x:number, y:number):fmath.vec2
fmath.vec2 = vec2

---@operator add(fmath.vec2):fmath.vec2
---@operator sub(fmath.vec2):fmath.vec2
---@operator mul(fmath.vec2|number):fmath.vec2
---@operator div(fmath.vec2|number):fmath.vec2


---@class fmath.vec3
---@field x number
---@field y number
---@field z number
local vec3 = {}

---@overload fun():fmath.vec3
---@overload fun(v:number):fmath.vec3
---@overload fun(x:number, y:number, z:number):fmath.vec3
fmath.vec3 = vec3

---@operator add(fmath.vec3):fmath.vec3
---@operator sub(fmath.vec3):fmath.vec3
---@operator mul(fmath.vec3|number):fmath.vec3
---@operator div(fmath.vec3|number):fmath.vec3


---@class fmath.vec4
---@field x number
---@field y number
---@field z number
---@field w number
local vec4 = {}

---@overload fun():fmath.vec4
---@overload fun(v:number):fmath.vec4
---@overload fun(x:number, y:number, z:number, w:number):fmath.vec4
fmath.vec4 = vec4

---@operator add(fmath.vec4):fmath.vec4
---@operator sub(fmath.vec4):fmath.vec4
---@operator mul(fmath.vec4|number):fmath.vec4
---@operator div(fmath.vec4|number):fmath.vec4


--========================
-- FUNCTIONS
--========================

---@overload fun(v:fmath.vec2|fmath.vec3|fmath.vec4):number
function fmath.Length(v) end

---@overload fun(v:fmath.vec2|fmath.vec3|fmath.vec4):fmath.vec2|fmath.vec3|fmath.vec4
function fmath.Normalized(v) end

---@overload fun(a:any, b:any):boolean
function fmath.Equal(a, b) end

---@overload fun(a:fmath.vec2|fmath.vec3|fmath.vec4, b:fmath.vec2|fmath.vec3|fmath.vec4):number
function fmath.Dot(a, b) end

---@overload fun(a:fmath.vec3, b:fmath.vec3):fmath.vec3
function fmath.Cross(a, b) end

---@overload fun(a:any, b:any, t:number):any
function fmath.Lerp(a, b, t) end

---@overload fun(v:number|fmath.vec2|fmath.vec3|fmath.vec4):any
function fmath.Abs(v) end

---@overload fun(a:any, b:any):any
function fmath.Min(a, b) end

---@overload fun(a:any, b:any):any
function fmath.Max(a, b) end

---@overload fun(v:any, min:any, max:any):any
function fmath.Clamp(v, min, max) end

---@overload fun(v:any):any
function fmath.Floor(v) end

---@overload fun(v:any):any
function fmath.Ceil(v) end

---@overload fun(v:number):number
function fmath.Rad(v) end

---@overload fun(v:number):number
function fmath.Deg(v) end

---@overload fun(min?:number, max?:number):number
function fmath.Random(min, max) end

---@overload fun(v:number):number
function fmath.Cos(v) end

---@overload fun(v:number):number
function fmath.Acos(v) end

---@overload fun(v:number):number
function fmath.Sin(v) end

---@overload fun(v:number):number
function fmath.Asin(v) end

---@overload fun(v:number):number
function fmath.Tan(v) end

---@overload fun(v:number):number
function fmath.Atan(v) end

---@overload fun(y:number, x:number):number
function fmath.Atan2(y, x) end

---@overload fun(v:number):number
function fmath.Sqrt(v) end

---@overload fun(v:number):number
function fmath.Exp(v) end

---@overload fun(a:number, b:number):number
function fmath.Pow(a, b) end

---@overload fun(v:number):number
function fmath.Log(v) end


--========================
-- CONSTANTS
--========================

---@type number
fmath.Pi = 3.141592653589793