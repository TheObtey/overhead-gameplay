---@meta

---@enum NodeTypes
NodeTypes = {
    NODE = 1,
    NODE2D = 2,
    NODE3D = 3,
    NODE_CAMERA = 4,
    NODE_MESH = 5,
    NODE_ANIMATED_MESH = 6,
    NODE_COLLIDER = 7,
    NODE_RIGIDBODY = 8,
    NODE_VIEWPORT = 9,
    NODE_VISUAL = 10,
    NODE_WINDOW = 11
}

--========================
-- NODE
--========================

---@class node
local node = {}

-- Constructor (factory)
---@overload fun(name:string):node
function CreateNode(name) end

--========================
-- METHODS
--========================

---@param child node
function node:AddChild(child) end

---@overload fun(child:node)
---@overload fun(name:string)
function node:RemoveChild(arg) end

---@param name string
---@return node|nil
function node:FindChild(name) end

---@param index integer
---@return node
function node:GetChild(index) end

---@return node[]
function node:GetChildren() end

---@return integer
function node:GetChildCount() end

---@param path string
---@return node
function node:Getnode(path) end

function node:Destroy() end

---@param newParent node
---@param keepGlobalTransform boolean
function node:Reparent(newParent, keepGlobalTransform) end

---@param child node
---@param to integer
function node:MoveChild(child, to) end

---@return node
function node:Clone() end

---@return string
function node:GetName() end

---@return node
function node:GetParent() end

---@return boolean
function node:HasParent() end

---@return SceneTree
function node:GetSceneTree() end

---@meta

--========================
-- SCENE TREE
--========================

---@class SceneTree
local SceneTree = {}

--========================
-- METHODS
--========================

---@param node node
function SceneTree:ChangeSceneTonode(node) end

---@return node
function SceneTree:GetCurrentScene() end

---@return node
function SceneTree:GetRoot() end


--========================
-- EVENTS
--========================

---@type fun()|nil
SceneTree.OnGameStarted = nil

---@type fun()|nil
SceneTree.OnGameEnded = nil

---@type fun()|nil
SceneTree.OnSceneChanged = nil
