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

local arrFolder = {}
function GetNewFileName(path)
	path = "E:\\DISI\\test\\0df3d7ca7bcb0a46c468b3aa6b63f6246a60afde.jpg"
	-- 匹配出jpg所在的文件夹
	local folderPos = string.find(path, "%\$")
	--匹配后缀
	
	MsgBox(tostring(folderPos)
	local folderPath = string.sub(path, 1, folderPos)
	if folderPath then
		if not arrFolder.folderPath then
			arrFolder.folderPath = 0
		end
		arrFolder.folderPath = arrFolder.folderPath + 1
		local newName = nil
		if arrFolder.folderPath < 10 then
			newName = "00"..tostring(arrFolder.folderPath)
		elseif arrFolder.folderPath < 100 then
			newName = "0"..tostring(arrFolder.folderPath)
		else
			newName = tostring(arrFolder.folderPath)
		end
	end
	MsgBox(tostring(folderName)
end
--[[--]]
-- local status,_= pcall(os.execute("REN "..folderTable[i].." "..newName.." 2>null"))--重命名


--重命名当前文件夹里的所有子文件夹，不递归
--该方法只适合用于那些有规律的文件夹，如Beay No.201
function RenameCurRegularFolders()
	local function GetNewFolderNum(name)
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
	
	os.execute("dir /r  > folder.txt")--列出当前文件夹下的子文件夹，不递归
	local folderTable = MakeTableFromFile("folder.txt")
	for i=1, #folderTable do
		local newName = GetNewFolderNum(folderTable[i])
		if newName then
			local status,_= pcall(os.rename(folderTable[i], newName))--重命名
			local info = newName .."  ==>  ".. folderTable[i].."\r\n"
			WriteFile("rename_info.txt", info)
		end
	end
	
	--删掉folder.txt
	os.execute("del /F /Q folder.txt")
end

function RenameCurMessyFolders()
	
end

--将当前目录下的所有文件(递归),重命名
function RenameAllMessyFiles()
	--递归列出当前文件夹下的所有文件
	os.execute(" for /r %i in (*.jpg, *.png, *.jpeg, *.gif, *.bmp) do (echo %i >> allfiles.txt)")
	local fileTable = MakeTableFromFile("allfiles.txt")
	MsgBox(tostring(fileTable and #fileTable)
	for i=1, #fileTable do
		local newName = GetNewFileName(fileTable[i])
		if newName then
			local status,_= pcall(os.rename(fileTable[i], newName))--重命名
		end
	end
	
	--删掉allfiles.txt
	os.execute("del /F /Q allfiles.txt")
end

function RenameAllRegularFiles()
	
end

function OnLoadLuaFile(a, b)
MsgBox(tostring(123)
	-- RenameAllFiles()
	-- GetNewFileName()
end