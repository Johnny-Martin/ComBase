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

function MakeTableFromFile(path)
	local file = io.open(path, "r+");
	if not file then
		print("ReadFile error: "..tostring(err))
		return nil, err
	end
	local t = {}
	for line in file:lines() do
		table.insert(t, line)
	end
	
	return t
end

function GetNewFolderNum(name)
	if not name or "" == name then
		return
	end
	local num = string.match(name, "(No%.%d+)")
	if not num then
		num = string.match(name, "%d+")
		if num then
			num = "No."..tostring(num)
		end
	end
	return num
end

function OnLoadLuaFile(a, b)
	-- local env = {}
	-- local fun = loadfile("I:\\UIEngine\\example\\luacode\\config.lua", "t", env)
	-- fun()
	-- MsgBox("height: "..tostring(env.height))
	
	os.execute("dir /ad/b  > temp.txt")
	
	local folderTable = MakeTableFromFile("temp.txt")
	for i=0, #folderTable do
		local newName = GetNewFolderNum(folderTable[i])
		if newName then
			-- local status,_= pcall(os.execute("REN "..folderTable[i].." "..newName.." 2>null"))--重命名
			local status,_= pcall(os.rename(folderTable[i], newName))--重命名
			local info = newName .."  ==>  ".. folderTable[i].."\r\n"
			WriteFile("info.txt", info)
		end
	end
	-- MsgBox(tostring(GetNewFolderNum(folderTable and folderTable[2])))
end