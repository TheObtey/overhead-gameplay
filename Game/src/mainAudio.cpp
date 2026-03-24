#include "Nodes/NodeAudioEmitter.h"
#include "Define.h" //temp

int main(int argc, char** argv)
{
    auto audioEm = Node::CreateNode<NodeAudioEmitter>("AudioEmitter");

    if (argc > 1) 
    {
        audioEm->PlayAudio(argv[1]);
    }
    else
    {
        printf("Empty filepath\n");
    }


    return 0;
}