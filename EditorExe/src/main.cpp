
#include <EditorSerializer.h>
#include <Editor.h>
#include <Scripting/Lua/LuaBindings.h>

int main()
{
	Editor editor;
	editor.Init();
	editor.Run();

	return 0;
}
