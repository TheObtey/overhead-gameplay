---@meta

---@class audioChannel
audioChannel = {}

-- -- AudioServer -- --
---@class audioServer
audioServer = {}

---@return boolean
function audioServer:Init() end
function audioServer:Shutdown() end

---@param name string
---@return AudioChannel
function audioServer:CreateChannel(name) end

---@param volume number
function audioServer:SetMasterVolume(volume) end

---@return number
function audioServer:GetMasterVolume() end

---@param channel AudioChannel
---@param volume number
function audioServer:SetGroupVolume(channel, volume) end

---@param channel AudioChannel
---@return number
function audioServer:GetGroupVolume(channel) end


-- -- AudioMixer -- --
---@class audioMixer
audioMixer = {}

---@param channel AudioChannel
---@param delaySeconds number
---@param decay number
---@param wet number
function audioMixer:AddDelay(channel, delaySeconds, decay, wet) end

---@param channel AudioChannel
---@param wet number
function audioMixer:SetDelayWet(channel, wet) end

---@param channel AudioChannel
function audioMixer:RemoveDelay(channel) end

---@param channel AudioChannel
---@param roomSize number
---@param wet number
function audioMixer:AddReverb(channel, roomSize, wet) end

---@param AudioChannel channel
---@param wet number
function audioMixer:SetReverbWet(channel, wet) end

---@param channel AudioChannel
function audioMixer:RemoveReverb(channel) end

function audioMixer:Shutdown() end

-- -- NodeAudioListener -- --
---@class nodeAudioListener
nodeAudioListener = {}

-- Constructeur de la classe nodeAudioListener
---@param name string
---@return nodeAudioListener
function nodeAudioListener:new(name) end

---@return vec3
function nodeAudioListener:GetListenerPosition() end

---@param position vec3
function nodeAudioListener:SetListenerPosition(position) end

---@return vec3
function nodeAudioListener:GetListenerDirection() end

---@param position vec3
function nodeAudioListener:SetListenerDirection(position) end


-- -- NodeAudioEmitter -- --
---@class nodeAudioEmitter
nodeAudioEmitter = {}

-- Constructeur de la classe NodeAudioEmitter
---@param name string
---@return NodeAudioEmitter
function nodeAudioEmitter:new(name) end

---@param filePath string
---@param channel AudioChannel
---@return boolean
function nodeAudioEmitter:Load(filePath, channel) end

function nodeAudioEmitter:Play() end
function nodeAudioEmitter:Stop() end

---@param isLooping boolean
function nodeAudioEmitter:SetLoop(isLooping) end

---@param position vec3
function nodeAudioEmitter:SetSourcePosition(position) end

---@return vec3
function nodeAudioEmitter:GetSourcePosition() end

---@param direction vec3
function nodeAudioEmitter:SetSourceDirection(direction) end

---@return vec3
function nodeAudioEmitter:GetSourceDirection() end

---@return boolean
function nodeAudioEmitter:IsPlaying() end