TestActiveScript = CreateScriptClass(ActiveScript, {});

function TestActiveScript:Initialize()
	--print("Initialize");
end

function TestActiveScript:BeforeUpdate()
	--print("BeforeUpdate");
end

function TestActiveScript:AfterUpdate()
	--print("AfterUpdate");
end

function TestActiveScript:OnComplete()
	--print("OnComplete");
end

function TestEvent(eventData)
	print("testing event");
	--test(0);
end

AddEventListener("scriptEvent", TestEvent);

TestActiveScript:Create();