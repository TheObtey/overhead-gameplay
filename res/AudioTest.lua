local audioEm = nil
local audioEm2 = nil
local audioListen = nil

local mixer = nil
local music = nil
local sfx = nil

function OnInit()
	--- Audio Server ---
	audioServer:Init();

	music = audioServer:CreateChannel("Music");
	sfx = audioServer:CreateChannel("Sfx");

	--- Mixer ---
	mixer = audioMixer:new()

	--mixer.AddDelay(sfx, 0.3f, 0.4f, 0.3f);
	mixer:AddReverb(music, 0.85, 0.3);

	-- Audio Emitter
    audioEm  = nodeAudioEmitter:new("AudioEmitter")
    audioEm2 = nodeAudioEmitter:new("AudioEmitter2")

	audioEm:Load("res/freeman.mp3", music);
	audioEm2:Load("res/applause.mp3", sfx);

	audioEm2:SetSourcePosition({ x=100,y=0,z=0 }); --allways after load

	--audioEm->SetLoop(true);
	audioEm2:SetLoop(true);

	audioEm:Play();
	audioEm2:Play();

	--- Audio Listener ---
	audioListen = nodeAudioListener:new("AudioListener")
	audioListen:SetListenerPosition({ x=0,y=0,z=0 });

	print("audio1 x:".. tostring(audioEm:GetSourcePosition().x));
	print("audio2 x:".. tostring(audioEm2:GetSourcePosition().x));
	print("listener x:".. tostring(audioListen:GetListenerPosition().x));

	mixer:Shutdown();
	audioServer:Shutdown();
end

function OnUpdate(dt) end
function OnDestroy() end