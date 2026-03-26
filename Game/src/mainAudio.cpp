#include <windows.h>

#include "Nodes/NodeAudioEmitter.h"
#include "Define.h"

int main()
{
    AudioServer::Init();

    AudioChannel* music = AudioServer::CreateChannel("Music");

    auto audioEm = Node::CreateNode<NodeAudioEmitter>("AudioEmitter");

    audioEm->Load("res/bass-wiggle.mp3", *music); // TEST ONLY (replace with your file)

    audioEm->Play();

    while (true)
    {
        if (GetAsyncKeyState(VK_ADD) & 0x8000)
        {
            std::cout << "test1";
            printf("test");
            AudioServer::SetMasterVolume((AudioServer::GetMasterVolume()+0.2f));
            Sleep(10);
        }
        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
        {
            AudioServer::SetMasterVolume((AudioServer::GetMasterVolume()-0.2f));
            Sleep(10);
        }
        if (GetAsyncKeyState('P') & 0x8000)
        {
            audioEm->Play();
            std::cout << "testplay";
            Sleep(10);
        }
        if (GetAsyncKeyState('S') & 0x8000)
        {
            audioEm->Stop();
            std::cout << "teststop";
            Sleep(10);
        }
        Sleep(1);
    }

    AudioServer::Shutdown();

    return 0;
}