#ifndef FOUNDRY_SCRIPT_INSTANCE__H_
#define FOUNDRY_SCRIPT_INSTANCE__H_

class Node;

class ScriptInstance
{
public:
	virtual ~ScriptInstance() = default;

	virtual void CallScriptOnInit() = 0;
	virtual void CallScriptOnUpdate(double dt) = 0;
	virtual void CallScriptOnPhysicsUpdate(double dt) = 0;
	virtual void CallScriptOnDestroy() = 0;

};

#endif // !FOUNDRY_SCRIPT_INSTANCE__H_
