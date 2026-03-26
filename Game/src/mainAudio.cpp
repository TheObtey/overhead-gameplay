#include <windows.h>

#include "Nodes/NodeAudioEmitter.h"
#include "Define.h"

int main()
{
    AudioServer::Init();

    AudioChannel* music = AudioServer::CreateChannel("Music");

    auto audioEm = Node::CreateNode<NodeAudioEmitter>("AudioEmitter");

    audioEm->Load("res/bass-wiggle.mp3", music); // TEST ONLY (replace with your file)

    audioEm->Play();

    while (true)
    {
        if (GetAsyncKeyState(VK_ADD) & 0x8000)
        {
            float next = AudioServer::GetMasterVolume() + 0.05f;
            AudioServer::SetMasterVolume(next);
            printf("MasterVolume: %.6f\n", AudioServer::GetMasterVolume());
        }
        else if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
        {
            float next = AudioServer::GetMasterVolume() - 0.05f;
            AudioServer::SetMasterVolume(next);
            printf("MasterVolume: %.6f\n", AudioServer::GetMasterVolume());
        }
        else if (GetAsyncKeyState('P') & 0x8000)
        {
            audioEm->Play();
            std::cout << "testplay";
        }
        else if (GetAsyncKeyState('S') & 0x8000)
        {
            audioEm->Stop();
            std::cout << "teststop";
        }
        Sleep(1);
    }

    AudioServer::Shutdown();

    return 0;
}