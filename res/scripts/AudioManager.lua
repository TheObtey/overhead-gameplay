local musicEm

local audioListen
local acmAudio

local channelMusic

local volume = 0.01

function OnInit()
	print("audio init !");

	--- Audio Server ---
	channelMusic = audioserver.CreateChannel("Music");

	-- Audio Emitter
	musicEm = CreateNodeAudioEmitter("nodeaudioemitter")

	musicEm:Load("res/audio/mainTheme.mp3", channelMusic);

	local audioEmPos = fmath.vec3:new(0,0,0)

	--audioEm:SetSourcePosition(audioEmPos); --allways after load

	--audioEm->SetLoop(true);

	musicEm:Play();

	--- Audio Listener ---
	audioListen = CreateNodeAudioListener("nodeaudiolistener")
    audioEmPos = fmath.vec3:new(0,0,0)
	--audioListen:SetListenerPosition(audioEmPos);

	print("audio1 x:".. tostring(musicEm:GetSourcePosition().x));
	print("listener x:".. tostring(audioListen:GetListenerPosition().x));

	audioserver.SetMasterVolume(volume)
	--audioserver.SetGroupVolume(channelMusic, 1)
end

function OnUpdate(dt) end
function OnDestroy() end