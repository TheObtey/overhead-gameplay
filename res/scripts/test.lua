function MyFunc(icontrol)
	print("MyFunc has been called")
end

function OnInit()
	local acmMyActionMap = actionmap:new("MY_ACTION_MAP")
	
	local acMyAction = acmMyActionMap:CreateAction("MyAction", 3, 4080)
	
	acMyAction.Event = MyFunc
end

function OnUpdate()

end