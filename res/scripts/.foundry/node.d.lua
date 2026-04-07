---@meta

--========================
-- NODE
--========================

---@class Node
local Node = {}

-- Constructor (factory)
---@overload fun(name:string):Node
function CreateNode(name) end

--========================
-- METHODS
--========================

---@param child Node
function Node:AddChild(child) end

---@overload fun(child:Node)
---@overload fun(name:string)
function Node:RemoveChild(arg) end

---@param name string
---@return Node|nil
function Node:FindChild(name) end

---@param index integer
---@return Node
function Node:GetChild(index) end

---@return Node[]
function Node:GetChildren() end

---@return integer
function Node:GetChildCount() end

---@param path string
---@return Node
function Node:GetNode(path) end

function Node:Destroy() end

---@param newParent Node
---@param keepGlobalTransform boolean
function Node:Reparent(newParent, keepGlobalTransform) end

---@param child Node
---@param to integer
function Node:MoveChild(child, to) end

---@return Node
function Node:Clone() end

---@return string
function Node:GetName() end

---@return Node
function Node:GetParent() end

---@return boolean
function Node:HasParent() end

---@return SceneTree
function Node:GetSceneTree() end

---@meta

--========================
-- SCENE TREE
--========================

---@class SceneTree
local SceneTree = {}

--========================
-- METHODS
--========================

---@param node Node
function SceneTree:ChangeSceneToNode(node) end

---@return Node
function SceneTree:GetCurrentScene() end

---@return Node
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