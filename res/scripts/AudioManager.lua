local audioEm
local audioListen
local acmAudio

--local mixer
local music

function OnInit()
	print("audio init !");

	--- Audio Server ---
	music = audioserver.CreateChannel("Music");

	-- Audio Emitter
	audioEm = CreateNodeAudioEmitter("nodeaudioemitter")

	audioEm:Load("res/audio/mainTheme.mp3", music);

	local audioEmPos = fmath.vec3:new(100,0,0)

	--audioEm:SetSourcePosition(audioEmPos); --allways after load

	--audioEm->SetLoop(true);

	audioEm:Play();

	--- Audio Listener ---
	audioListen = CreateNodeAudioListener("nodeaudiolistener")
    audioEmPos = fmath.vec3:new(0,0,0)
	--audioListen:SetListenerPosition(audioEmPos);

	print("audio1 x:".. tostring(audioEm:GetSourcePosition().x));
	print("listener x:".. tostring(audioListen:GetListenerPosition().x));

	audioserver.SetMasterVolume(1)
end

function OnUpdate(dt) end
function OnDestroy() end