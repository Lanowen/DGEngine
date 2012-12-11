GameActiveScript = CreateScriptClass(ActiveScript, {});

player = nil;
camera = nil;

cameraXOffset = 0;
cameraYOffset = 100;
cameraZOffset = -100;

function GameActiveScript:Initialize()
	--print("Initialize");
end

function GameActiveScript:BeforeUpdate()
	--print("BeforeUpdate");
end

function GameActiveScript:AfterUpdate()
	--print("AfterUpdate");
	if player ~= nil and camera ~= nil then
		ppos = player.getPosition();
		camera.setPosition(ppos.x + cameraXOffset, ppos.y + cameraYOffset, ppos.z + cameraZOffset);
	end
end

function GameActiveScript:OnComplete()
	--print("OnComplete");
end

function CreatePlayerEventHandler(eventData)
	CreatePlayer("programdata/space_frigate_6.modelobj", "programdata/space_frigate_6_color.jpg", "Player");
	print("PlayerCreated");
	player = findGameObjectByName("Player");
	camera = findGameObjectByName("MainCamera");
end

AddEventListener("CreatePlayer", CreatePlayerEventHandler);

function ResetPlayerEventHandler(eventData)
	if player ~= nil then
		player.setPosition(0,0,0);
		angle = 3.141592654/2;
		player.setRotation(0,math.sin(angle/2),0,math.cos(angle/2));
		print("ResetPlayer")
	end
end

AddEventListener("ResetPlayer", ResetPlayerEventHandler);

function ChangeCameraView(eventData)
	if camera ~= nil then
		print("ChangeCameraView");
		if eventData.view == 1 then
			cameraXOffset = 0;
			cameraYOffset = 100;
			cameraZOffset = -100;
			angle = 3.141592654/4;
			camera.setRotation(math.sin(angle/2),0,0,math.cos(angle/2));
		elseif eventData.view == 2 then
			cameraXOffset = -70;
			cameraYOffset = 0;
			cameraZOffset = 0;
			angle = 3.141592654/2;
			camera.setRotation(0,math.sin(angle/2),0,math.cos(angle/2));
		elseif eventData.view == 3 then
			cameraXOffset = 70;
			cameraYOffset = 0;
			cameraZOffset = 0;
			angle = 3.141592654/2;
			camera.setRotation(0,-math.sin(angle/2),0,math.cos(angle/2));
		else
			cameraXOffset = 0;
			cameraYOffset = 0;
			cameraZOffset = 70;
			angle = 3.141592654;
			camera.setRotation(0,math.sin(angle/2),0,math.cos(angle/2));
		end
	end

end

AddEventListener("ChangeCameraView", ChangeCameraView);

GameActiveScript:Create();