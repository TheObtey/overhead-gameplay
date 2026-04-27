local audioEm
local audioEm2
local audioListen
local acmAudio

--local mixer
local music
local sfx

function OnInit()
	print("audio init !");

	--- Audio Server ---
	music = audioserver.CreateChannel("Music");
	sfx =  audioserver.CreateChannel("Sfx");

	--- Mixer ---

	-- Audio Emitter
    --audioEm  = nodeaudioemitter:new("nodeaudioemitter")
	audioEm = CreateNodeAudioEmitter("nodeaudioemitter")
	audioEm2 = CreateNodeAudioEmitter("nodeaudioemitter")
    --audioEm2 = nodeaudioemitter:new("nodeaudioemitter")

	audioEm:Load("res/freeman.mp3", music);
	audioEm2:Load("res/applause.mp3", sfx);

	local v = fmath.vec3:new(100,0,0)

	audioEm2:SetSourcePosition(v); --allways after load

	--audioEm->SetLoop(true);
	--audioEm2:SetLoop(true);

	audioEm:Play();
	audioEm2:Play();

	--- Audio Listener ---
	audioListen = CreateNodeAudioListener("nodeaudiolistener")
    v = fmath.vec3:new(0,0,0)
	audioListen:SetListenerPosition(v);

	print("audio1 x:".. tostring(audioEm:GetSourcePosition().x));
	print("audio2 x:".. tostring(audioEm2:GetSourcePosition().x));
	print("listener x:".. tostring(audioListen:GetListenerPosition().x));

	-- ACTIONS

	audioserver.SetMasterVolume(0.5)
end

function OnUpdate(dt) end
function OnDestroy() end