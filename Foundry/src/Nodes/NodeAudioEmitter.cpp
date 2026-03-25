#include "Nodes/NodeAudioEmitter.h"

NodeAudioEmitter::NodeAudioEmitter(std::string const& name) : Node(name)
{

}

void NodeAudioEmitter::AudioSetFile(const char* filePath)
{
	AudioServer::SetAudioFile(filePath);
}

void NodeAudioEmitter::AudioPlay()
{
	AudioServer::Play();
}

void NodeAudioEmitter::AudioStop()
{
	AudioServer::Stop();
}

void NodeAudioEmitter::AudioSetLoop(bool value)
{
	AudioServer::SetLoop(value);
}

void NodeAudioEmitter::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
	AudioServer::Update();
}

ISerializable* NodeAudioEmitter::CreateInstance()
{
	return CreateNode<NodeAudioEmitter>("NodeAudioEmitter").release();
}