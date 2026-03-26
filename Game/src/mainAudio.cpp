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
        
    }

    AudioServer::Shutdown();

    return 0;
}