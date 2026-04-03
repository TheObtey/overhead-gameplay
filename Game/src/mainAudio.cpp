#include "Nodes/NodeAudioEmitter.h"
#include "Nodes/NodeAudioListener.h"
#include "MixerAudio.h"
#include "Define.h"

#include "ActionMap.h"
#include "Action.h"
#include "GameLoop.h"

#include <Nodes/NodeWindow.h>

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Root");
    uptr<Node> scene = Node::CreateNode<Node>("Scene");
    root->AddChild(scene);
    SceneTree defaultSceneTree(root);

	// --- Audio Server ---
	AudioServer::Init();

	AudioChannel* music = AudioServer::CreateChannel("Music");
	AudioChannel* sfx = AudioServer::CreateChannel("Sfx");

	// --- Mixer ---
	MixerAudio mixer;

	//mixer.AddDelay(sfx, 0.3f, 0.4f, 0.3f);
	mixer.AddReverb(music, 0.85f, 0.3f);

	// --- Audio Emitter ---
	auto audioEm = Node::CreateNode<NodeAudioEmitter>("AudioEmitter");
	auto audioEm2 = Node::CreateNode<NodeAudioEmitter>("AudioEmitter2");

	audioEm->Load("res/freeman.mp3", music);
	audioEm2->Load("res/applause.mp3", sfx);

	audioEm2->SetSourcePosition({ 100,0,0 }); // allways after load

	//audioEm->SetLoop(true);
	audioEm2->SetLoop(true);

	audioEm->Play();
	audioEm2->Play();

	// --- Audio Listener ---
	auto audioListen = Node::CreateNode<NodeAudioListener>("AudioListener");
	audioListen->SetListenerPosition({ 0,0,0 });

	Logger::Log("audio1 x:" + std::to_string(audioEm->GetSourcePosition().x));
	Logger::Log("audio2 x:" + std::to_string(audioEm2->GetSourcePosition().x));
	Logger::Log("listener x:" + std::to_string(audioListen->GetListenerPosition().x));

    // --- Actions ---
    ActionMap actionMap;

    Action* pauseAction = new Action(ControlType::BUTTON, EventInput::KEY_P);
    pauseAction->Event += [&](IControl& control)
        {
            if (audioEm->IsPlaying())
            {
                audioEm->Stop();
                printf("Pause\n");
            }
            else
            {
                audioEm->Play();
                printf("Play\n");
            }
        };
    actionMap.Emplace("Pause", pauseAction);

    Action* volUpSfx = new Action(ControlType::BUTTON, EventInput::KEY_KP_ADD);
    volUpSfx->Event += [&](IControl& control)
        {
            float next = AudioServer::GetGroupVolume(*sfx) + 0.01f;
            AudioServer::SetGroupVolume(sfx, next);
            printf("GroupVolume: %.6f\n", AudioServer::GetGroupVolume(*sfx));
        };
    actionMap.Emplace("VolUpSfx", volUpSfx);

    Action* volDownSfx = new Action(ControlType::BUTTON, EventInput::KEY_KP_SUBTRACT);
    volDownSfx->Event += [&](IControl& control)
        {
            float next = AudioServer::GetGroupVolume(*sfx) - 0.01f;
            AudioServer::SetGroupVolume(sfx, next);
            printf("GroupVolume: %.6f\n", AudioServer::GetGroupVolume(*sfx));
        };
    actionMap.Emplace("VolDownSfx", volDownSfx);

    Action* masterUp = new Action(ControlType::BUTTON, EventInput::KEY_T);
    masterUp->Event += [&](IControl& control)
        {
            float next = AudioServer::GetMasterVolume() + 0.05f;
            AudioServer::SetMasterVolume(next);
            printf("MasterVolume: %.6f\n", AudioServer::GetMasterVolume());
        };
    actionMap.Emplace("MasterUp", masterUp);

    Action* masterDown = new Action(ControlType::BUTTON, EventInput::KEY_Y);
    masterDown->Event += [&](IControl& control)
        {
            float next = AudioServer::GetMasterVolume() - 0.05f;
            AudioServer::SetMasterVolume(next);
            printf("MasterVolume: %.6f\n", AudioServer::GetMasterVolume());
        };
    actionMap.Emplace("MasterDown", masterDown);

    Action* resetListener = new Action(ControlType::BUTTON, EventInput::KEY_W);
    resetListener->Event += [&](IControl& control)
        {
            audioListen->SetListenerPosition({ 0, 0, 0 });
        };
    actionMap.Emplace("ResetListener", resetListener);

    Action* moveEmitter = new Action(ControlType::BUTTON, EventInput::KEY_X);
    moveEmitter->Event += [&](IControl& control)
        {
            audioEm->SetSourcePosition({ 50, 0, 0 });
        };
    actionMap.Emplace("MoveEmitter", moveEmitter);

    Action* moveUp = new Action(ControlType::BUTTON, EventInput::KEY_Z);
    moveUp->Event += [&](IControl& control)
        {
            glm::vec3 newPos = audioListen->GetListenerPosition();
            newPos.y += 0.1f;
            audioListen->SetListenerPosition(newPos);
        };
    actionMap.Emplace("MoveUp", moveUp);

    Action* moveLeft = new Action(ControlType::BUTTON, EventInput::KEY_Q);
    moveLeft->Event += [&](IControl& control)
        {
            glm::vec3 newPos = audioListen->GetListenerPosition();
            newPos.x -= 0.1f;
            audioListen->SetListenerPosition(newPos);
        };
    actionMap.Emplace("MoveLeft", moveLeft);

    Action* moveDown = new Action(ControlType::BUTTON, EventInput::KEY_S);
    moveDown->Event += [&](IControl& control)
        {
            glm::vec3 newPos = audioListen->GetListenerPosition();
            newPos.y -= 0.1f;
            audioListen->SetListenerPosition(newPos);
        };
    actionMap.Emplace("MoveDown", moveDown);

    Action* moveRight = new Action(ControlType::BUTTON, EventInput::KEY_D);
    moveRight->Event += [&](IControl& control)
        {
            glm::vec3 newPos = audioListen->GetListenerPosition();
            newPos.x += 0.1f;
            audioListen->SetListenerPosition(newPos);
        };
    actionMap.Emplace("MoveRight", moveRight);

    // --- Main Loop ---
    //while (true)
    //{
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //}

    GameLoop loop;
    loop.StartGame(defaultSceneTree);

	mixer.Shutdown();
	AudioServer::Shutdown();

	return 0;
}