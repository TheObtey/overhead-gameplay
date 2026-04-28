local musicEm
local audioListen

local channelMusic
local channelSound

local globalVolume = 1
local musicVolume = 0.3
local soundVolume = 0.7

local acmAudio

local audioWalk
local audioGravGun

function PlayWalkSound()
    if audioWalk then
		audioWalk:Stop()
        audioWalk:Play()
    end
end
function StopWalkSound()
    if audioWalk then
        audioWalk:Stop()
    end
end

function PlayGravGunSound()
    if audioGravGun then
        audioGravGun:Stop()
    	audioGravGun:Play()
    end
end
function StopGravGunSound()
    if audioGravGun then
        audioGravGun:Stop()
    end
end


local function InitializeActionMap()
    acmAudio = actionmap:new("AUDIO")
    acmAudio:CreateAction("WALK_SOUND", 1, EventInput.KEY_Z)

	acmAudio:GetAction("WALK_SOUND").Event = function() 
		audioWalk:Play()
	end
end

function OnInit()
	print("audio init !");

	--- Audio Server ---
	channelMusic = audioserver.CreateChannel("Music")
	channelSound = audioserver.CreateChannel("Sound")

	-- Audio Emitter
	musicEm = CreateNodeAudioEmitter("nodeaudioemitter")
	audioWalk = CreateNodeAudioEmitter("nodeaudioemitter")
	audioGravGun = CreateNodeAudioEmitter("nodeaudioemitter")

	musicEm:Load("res/audio/mainTheme.mp3", channelMusic);
	audioWalk:Load("res/audio/moonBoots.mp3", channelSound);
	audioGravGun:Load("res/audio/gravityGunActivation.wav", channelSound);

	local audioEmPos = fmath.vec3:new(0,0,0)

	--audioEm:SetSourcePosition(audioEmPos); --allways after load

	--audioEm->SetLoop(true);

	musicEm:Play()

	--- Audio Listener ---
	audioListen = CreateNodeAudioListener("nodeaudiolistener")
    audioEmPos = fmath.vec3:new(0,0,0)
	--audioListen:SetListenerPosition(audioEmPos);

	print("audio1 x:".. tostring(musicEm:GetSourcePosition().x))
	print("listener x:".. tostring(audioListen:GetListenerPosition().x))

	audioserver.SetMasterVolume(globalVolume)
	audioserver.SetGroupVolume(channelMusic, musicVolume)
	audioserver.SetGroupVolume(channelSound, soundVolume)

	--InitializeActionMap() cannot use multiple action map in project

	-- Hooks from other scripts
	hook.Add("OnPlayerMove", "OnPlayerMove:PlayWalkSound", function() 
		PlayWalkSound()
	end)
	hook.Add("OnPlayerStop", "OnPlayerStop:StopWalkSound", function() 
		StopWalkSound()
	end)

	hook.Add("OnUseGun", "OnUseGun:PlayGravGunSound", function() 
		PlayGravGunSound()
	end)
	hook.Add("OnStopGun", "OnStopGun:StopGravGunSound", function() 
		StopGravGunSound()
	end)
end

function OnUpdate(dt) end
function OnDestroy() end