---@meta

---@class audiochannel
audiochannel = {}

-- -- AudioServer -- --
---@class audioserver
audioserver = {}

---@return boolean
function audioserver:Init() end
function audioserver:Shutdown() end

---@param name string
---@return audiochannel
function audioserver:CreateChannel(name) end

---@param volume number
function audioserver:SetMasterVolume(volume) end

---@return number
function audioserver:GetMasterVolume() end

---@param channel audiochannel
---@param volume number
function audioserver:SetGroupVolume(channel, volume) end

---@param channel audiochannel
---@return number
function audioserver:GetGroupVolume(channel) end


-- -- AudioMixer -- --
---@class audiomixer
audiomixer = {}

---@param channel audiochannel
---@param delaySeconds number
---@param decay number
---@param wet number
function audiomixer:AddDelay(channel, delaySeconds, decay, wet) end

---@param channel audiochannel
---@param wet number
function audiomixer:SetDelayWet(channel, wet) end

---@param channel audiochannel
function audiomixer:RemoveDelay(channel) end

---@param channel audiochannel
---@param roomSize number
---@param wet number
function audiomixer:AddReverb(channel, roomSize, wet) end

---@param channel audiochannel
---@param wet number
function audiomixer:SetReverbWet(channel, wet) end

---@param channel audiochannel
function audiomixer:RemoveReverb(channel) end

function audiomixer:Shutdown() end

-- -- NodeAudioListener -- --
---@class nodeaudiolistener
nodeaudiolistener = {}

---@param name string The node name.
---@return nodeaudiolistener
function CreateNodeAudioListener(name) end

---@return vec3
function nodeaudiolistener:GetListenerPosition() end

---@param position vec3
function nodeaudiolistener:SetListenerPosition(position) end

---@return vec3
function nodeaudiolistener:GetListenerDirection() end

---@param position vec3
function nodeaudiolistener:SetListenerDirection(position) end


-- -- NodeAudioEmitter -- --
---@class nodeaudioemitter
nodeaudioemitter = {}

---@param name string The node name.
---@return nodeaudioemitter
function CreateNodeAudioEmitter(name) end

---@param filePath string
---@param channel AudioChannel
---@return boolean
function nodeaudioemitter:Load(filePath, channel) end

function nodeaudioemitter:Play() end
function nodeaudioemitter:Stop() end

---@param isLooping boolean
function nodeaudioemitter:SetLoop(isLooping) end

---@param position vec3
function nodeaudioemitter:SetSourcePosition(position) end

---@return vec3
function nodeaudioemitter:GetSourcePosition() end

---@param direction vec3
function nodeaudioemitter:SetSourceDirection(direction) end

---@return vec3
function nodeaudioemitter:GetSourceDirection() end

---@return boolean
function nodeaudioemitter:IsPlaying() end