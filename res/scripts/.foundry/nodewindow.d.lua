---@meta

---@enum CursorState
CursorState = {
    FREE   = 0x00034001,
    HIDDEN = 0x00034002,
    LOCKED = 0x00034003
}

---@class nodewindow : node2d
nodewindow = {}

---@param cursorState CursorState
function nodewindow:SetCursorState(cursorState) end
