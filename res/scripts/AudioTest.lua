local audioEm
local audioEm2
local audioListen

--local mixer
local music
local sfx

function OnInit()
	--- Audio Server ---
	music = audioserver:CreateChannel("Music");
	sfx =  audioserver:CreateChannel("Sfx");

	--- Mixer ---
	--mixer = audioMixer:new()

	--mixer.AddDelay(sfx, 0.3f, 0.4f, 0.3f);
	--mixer:AddReverb(music, 0.85, 0.3);

	-- Audio Emitter
    --audioEm  = nodeaudioemitter:new("nodeaudioemitter")
	audioEm = CreateNodeAudioEmitter("nodeaudioemitter")
	audioEm2 = CreateNodeAudioEmitter("nodeaudioemitter")
    --audioEm2 = nodeaudioemitter:new("nodeaudioemitter")

	audioEm:Load("freeman.mp3", music);
	audioEm2:Load("applause.mp3", sfx);

	local v = fmath.vec3:new(100,0,0)

	audioEm2:SetSourcePosition(v); --allways after load

	--audioEm->SetLoop(true);
	audioEm2:SetLoop(true);

	audioEm:Play();
	audioEm2:Play();

	--- Audio Listener ---
	audioListen = CreateNodeAudioListener("nodeaudiolistener")
    v = fmath.vec3:new(0,0,0)
	audioListen:SetListenerPosition(v);

	print("audio1 x:".. tostring(audioEm:GetSourcePosition().x));
	print("audio2 x:".. tostring(audioEm2:GetSourcePosition().x));
	print("listener x:".. tostring(audioListen:GetListenerPosition().x));

	--mixer:Shutdown();
end

function OnUpdate(dt) end
function OnDestroy() end