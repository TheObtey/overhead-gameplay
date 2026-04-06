---@meta

---@class node
node = {}

-- ---@class node3D : node
-- node3D = {}

-- function node3D:ToTo() end

---@class fmaths
fmaths = {}

---@class vec3
---@field x number
---@field y number
---@field z number
fmaths.vec3 = {}

math = nil

---@param name string The node name.
---@return node
function CreateNode(name) end

---@param node node
function node:AddChild(node) end

---@param node node
function node:RemoveChild(node) end

---@param name string
function node:RemoveChild(name) end

---@param name string
---@return node
function node:FindChild(name) end

---@return string
function node:GetName() end

---@return vec3
function fmaths.vec3:new() end

---@param x number
---@param y number
---@param z number
---@return vec3
function fmaths.vec3:new(x, y, z) end

---@return number
function fmaths.vec3:GetX() end

---@return number
function fmaths.vec3:GetY() end

---@return number
function fmaths.vec3:GetZ() end

--- Called when the self node is initialized
function OnInit() end




