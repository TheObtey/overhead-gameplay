#include "Nodes/NodeAudioEmitter.h"
#include "Define.h"

int main()
{
    auto audioEm = Node::CreateNode<NodeAudioEmitter>("AudioEmitter");
    audioEm->AudioSetFile("res/bass-wiggle.mp3"); // TEST ONLY (replace with your file)
    audioEm->AudioSetLoop(true);

    audioEm->AudioPlay();

    while (true)
    {
        
    }

    audioEm->AudioStop();

    return 0;
}