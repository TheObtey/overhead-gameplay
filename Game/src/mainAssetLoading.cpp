////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include <AssetLoading/FBXLoader.h>

int main()
{
	uptr<SceneData> scene = FBXLoader::LoadFile("res/Test.fbx");
}