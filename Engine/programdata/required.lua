print("Loading script");

-- Global function definition, move elsewhere later.
function CreateScriptClass(baseClass, body)
	local class = body or {};

	if (baseClass ~= nil) then
		setmetatable(class, class);
		class.__index = baseClass;
		class.base = baseClass;
	end

	class.Create = function(self, data, subClass)
		local obj;
		if (self.__index ~= nil) then
			if (subClass ~= nil) then
				obj = self.__index:Create(data, subClass);
			else
				obj = self.__index:Create(data, self);
			end
		else
			obj = data or {};
		end

		setmetatable(obj, obj);
		obj.__index = self;

		if (self.__operator ~= nil) then
			for key, val in pairs(self.__operators) do
				obj[key] = val;
			end
		end

		return obj;
	end

	-- Instance function?

	return class;
end

print("Loaded script");