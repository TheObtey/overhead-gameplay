#include "AudioEngine.h" // need to make node and server instead
#include "Define.h" //temp


int main(int argc, char** argv)
{
    AudioEngine newAudioEngine;
    newAudioEngine.PlayAudio(argv[1]);

    return 0;
}