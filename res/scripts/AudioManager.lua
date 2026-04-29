-- ─── Audio Volume ───────────────────────────────────────────────────────────
local globalVolume = 1
local musicVolume = 0.3
local soundVolume = 0.7


--local audioListen

-- ─── Channels ───────────────────────────────────────────────────────────
local channelMusic
local channelSound

-- ─── Audio files ───────────────────────────────────────────────────────────
local audioMusic
local audioWalk
local audioGravGun
local audioJump

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

function PlayJumpSound()
    if audioJump then
        audioJump:Stop()
    	audioJump:Play()
    end
end

function OnInit()
	print("audio init !");

	--- Audio Server ---
	channelMusic = audioserver.CreateChannel("Music")
	channelSound = audioserver.CreateChannel("Sound")

	-- Audio Emitter
	audioMusic = CreateNodeAudioEmitter("nodeaudioemitter")
	audioWalk = CreateNodeAudioEmitter("nodeaudioemitter")
	audioGravGun = CreateNodeAudioEmitter("nodeaudioemitter")
	audioJump = CreateNodeAudioEmitter("nodeaudioemitter")

	audioMusic:Load("res/audio/mainTheme.mp3", channelMusic);
	audioWalk:Load("res/audio/moonBoots.mp3", channelSound);
	audioGravGun:Load("res/audio/gravityGunActivation.wav", channelSound);
	audioJump:Load("res/audio/jump.wav", channelSound);

	--local audioEmPos = fmath.vec3:new(0,0,0)
	--audioEm:SetSourcePosition(audioEmPos); --allways after load
	audioMusic:SetLoop(true);

	audioMusic:Play()

	--- Audio Listener ---
	--audioListen = CreateNodeAudioListener("nodeaudiolistener")
    --audioEmPos = fmath.vec3:new(0,0,0)
	--audioListen:SetListenerPosition(audioEmPos);
	--print("audio1 x:".. tostring(musicEm:GetSourcePosition().x))
	--print("listener x:".. tostring(audioListen:GetListenerPosition().x))

	audioserver.SetMasterVolume(globalVolume)
	audioserver.SetGroupVolume(channelMusic, musicVolume)
	audioserver.SetGroupVolume(channelSound, soundVolume)

	-- ─── Hooks from other scripts ───────────────────────────────────────────────────────────
	hook.Add("OnPlayerMove", "OnPlayerMove:PlayWalkSound", function() 
		PlayWalkSound()
	end)
	hook.Add("OnPlayerStop", "OnPlayerStop:StopWalkSound", function() 
		StopWalkSound()
	end)
	hook.Add("OnUseGun", "OnUseGun:PlayGravGunSound", function() 
		PlayGravGunSound()
	end)
	hook.Add("OnJump", "OnJump:PlayJumpSound", function() 
		PlayJumpSound()
	end)


end

function OnUpdate(dt) end
function OnDestroy() end