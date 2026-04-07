---@meta

---@class Node
Node = {}

---@class Node3D : Node
Node3D = {}

function Node3D:ToTo() end

---@class maths
maths = {}

---@class vec3
---@field x number
---@field y number
---@field z number
maths.vec3 = {}

math = nil

---@param name string The node name.
---@return Node
function CreateNode(name) end

---@param node Node
function Node:AddChild(node) end

---@param node Node
function Node:RemoveChild(node) end

---@param name string
function Node:RemoveChild(name) end

---@param name string
---@return Node
function Node:FindChild(name) end

---@return string
function Node:GetName() end

---@return vec3
function maths.vec3:new() end

---@param x number
---@param y number
---@param z number
---@return vec3
function maths.vec3:new(x, y, z) end

---@return number
function maths.vec3:GetX() end

---@return number
function maths.vec3:GetY() end

---@return number
function maths.vec3:GetZ() end

--- Called when the self node is initialized
function OnInit() end