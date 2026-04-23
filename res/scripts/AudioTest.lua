local audioEm
local audioEm2
local audioListen
local acmPlayer

--local mixer
local music
local sfx

local function InitializeActionMap()
    acmPlayer = actionmap:new("PLAYER")
    acmPlayer:CreateAction("VOLUME_UP", 1, EventInput.KEY_W)
    acmPlayer:CreateAction("VOLUME_DOWN", 1, EventInput.KEY_X)
end

self.AddVolume = function(icX)
    if icX:IsPressed() then
        audioserver.SetMasterVolume(1)
    end
end

self.DownVolume = function(icW)
    if icW:IsPressed() then
        audioserver.SetMasterVolume(0)
    end
end

local function BindActions()
    if audioserver then
       acmPlayer:GetAction("VOLUME_UP").Event     = self.AddVolume or
           function() print("PlayerBase: VOLUME_UP callback missing") end
       acmPlayer:GetAction("VOLUME_DOWN").Event   = self.DownVolume or
           function() print("PlayerBase: VOLUME_DOWN callback missing") end
    else
        print("PlayerBase: audioserver missing")
	end
end

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

	-- ACTIONS
	InitializeActionMap()
    BindActions()
end

function OnUpdate(dt) end
function OnDestroy() end