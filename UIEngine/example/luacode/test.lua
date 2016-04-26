-------------------LRT------------------
--share identifiers between lua files
--GetGlobal()
--SetGlobal()

--get engine inner object, like HTTP, objFactory, util, timerMgr etc.
--GetObject()

--singleton timerMgr's function
--KillTimer()
--SetTimer()
--SetDelay()

--function for debug
--MsgBox()
--Log()

--load and unload lua file, call OnUnloadLuaFile(if exists) before unload
--LoadLuaFile()

--UnloadLuaFile()

function LuaFun4Cpp(a, b)
	-- return a + b
	return CFun4Lua(a, b) + 100
end

function SaveDataToFile(fileName, data)
	local file = io.open(fileName, "a")
	file:write(data)
	file:close()
end

function err(msg)
	MsgBox(msg)
end
function IsFullPath(filePath)
	local ret = string.match(filePath, "(%a):.*")
	if ret and ret ~= "" then
		return true
	else
		return false
	end
end

function ReadFile(filePath, readMode)
	-- if not IsFullPath(filePath) then
		-- filePath = __document.."\\..\\"..filePath
	-- end
	-- print("ReadFile filePath: "..tostring(filePath))
	
	local file, err = io.open(filePath,'r')
	if not file then
		print("ReadFile error: "..tostring(err))
		return nil, err
	end
	
	readMode = readMode or "*all"
	local data = file:read(readMode)
	file:close()
	
	return data
end

function WriteFile(filePath, data, openMode)
	-- if not IsFullPath(filePath) then
		-- filePath = __document.."\\..\\"..filePath
	-- end
	-- print("WriteFile filePath: "..tostring(filePath))
	
	openMode = openMode or "a"
	local file, err = io.open(filePath, openMode)
	if not file then
		print("WriteFile error: "..tostring(err))
		return nil, err
	end
	
	file:write(data)
	file:close()
end

function OnLoadLuaFile(a, b)
	local env = {}
	local fun = loadfile("I:\\UIEngine\\example\\luacode\\config.lua", "t", env)
	-- fun()
	MsgBox("height: "..tostring(env.height))
end