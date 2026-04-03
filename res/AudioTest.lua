-----@type NodeAudioEmitter
---self = Node

local audioEm = nil
local audioEm2 = nil
local audioListen = nil

local mixer = nil
local music = nil
local sfx = nil

function OnInit()
	--- Audio Server ---
	AudioServer:Init();

	music = AudioServer:CreateChannel("Music");
	sfx = AudioServer:CreateChannel("Sfx");

	--- Mixer ---
	mixer = MixerAudio:new()

	--mixer.AddDelay(sfx, 0.3f, 0.4f, 0.3f);
	mixer.AddReverb(music, 0.85, 0.3);

	--auto audioEm = Node::CreateNode<NodeAudioEmitter>("AudioEmitter");
	--auto audioEm2 = Node::CreateNode<NodeAudioEmitter>("AudioEmitter2");
	-- Audio Emitter
    audioEm  = NodeAudioEmitter:new("AudioEmitter")
    audioEm2 = NodeAudioEmitter:new("AudioEmitter2")

	audioEm:Load("res/freeman.mp3", music);
	audioEm2:Load("res/applause.mp3", sfx);

	audioEm2:SetSourcePosition({ x=100,y=0,z=0 }); --allways after load

	--audioEm->SetLoop(true);
	audioEm2:SetLoop(true);

	audioEm:Play();
	audioEm2:Play();

	--- Audio Listener ---
	audioListen = NodeAudioListener:new("AudioListener")
	audioListen:SetListenerPosition({ x=0,y=0,z=0 });

	--printf("audio1 x:%.6f\n", audioEm->GetSourcePosition().x);
	--printf("audio2 x:%.6f\n", audioEm2->GetSourcePosition().x);
	--printf("listener x:%.6f\n", audioListen->GetListenerPosition().x);

	while (true)
	do
		if (GetAsyncKeyState('P') & 0x8000) then
			if (audioEm:IsPlaying()) then 
				audioEm:Stop();
				print("Pause\n");
			else
				audioEm:Play();
				print("Play\n");
			end
			Sleep(200);
		elseif (GetAsyncKeyState(VK_ADD) & 0x8000) then
			--Group volume
			local next = AudioServer:GetGroupVolume(sfx) + 0.01;
			AudioServer:SetGroupVolume(sfx, next);
			--print("GroupVolume: " + AudioServer::GetGroupVolume(*sfx));
		elseif (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) then
			--Group volume
			local next = AudioServer:GetGroupVolume(sfx) - 0.01;
			AudioServer:SetGroupVolume(sfx, next);
			--print("GroupVolume: " + AudioServer::GetGroupVolume(*sfx));
		elseif (GetAsyncKeyState('T') & 0x8000) then
			--Master volume
			local next = AudioServer:GetMasterVolume() + 0.05;
			AudioServer:SetMasterVolume(next);
			--print("MasterVolume: " + AudioServer::GetMasterVolume());
		elseif (GetAsyncKeyState('Y') & 0x8000) then
			--Master volume
			local next = AudioServer:GetMasterVolume() - 0.05;
			AudioServer:SetMasterVolume(next);
			--print("MasterVolume: " + AudioServer::GetMasterVolume());
		elseif (GetAsyncKeyState('W') & 0x8000) then --PositionAudio3D Emitter
			audioListen:SetListenerPosition({ x=0,y=0,z=0 });
		elseif (GetAsyncKeyState('X') & 0x8000) then
			audioEm:SetSourcePosition({ x=50,y=0,z=0 });
		elseif (GetAsyncKeyState('Z') & 0x8000) then --PositionAudio3D Listener
			local newPos = audioListen:GetListenerPosition();
			newPos.y = newPos.y + 0.1;
			audioListen:SetListenerPosition(newPos);
		elseif (GetAsyncKeyState('Q') & 0x8000) then
			local newPos = audioListen:GetListenerPosition();
			newPos.x = newPos.x - 0.1;
			audioListen:SetListenerPosition(newPos);
		elseif (GetAsyncKeyState('S') & 0x8000) then
			local newPos = audioListen:GetListenerPosition();
			newPos.y = newPos.y - 0.1;
			audioListen:SetListenerPosition(newPos);
		elseif (GetAsyncKeyState('D') & 0x8000) then
			local newPos = audioListen:GetListenerPosition();
			newPos.x = newPos.x + 0.1;
			audioListen:SetListenerPosition(newPos);
		end

		Sleep(1);
	end

	mixer.Shutdown();
	AudioServer:Shutdown();
end

function OnUpdate(dt)


end