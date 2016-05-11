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
	-- 匹配出jpg所在的文件夹
	local fileExtPosBegin, fileExtPosEnd = string.find(path, "(%.%w+)", string.len(path) - 5)
	if not fileExtPosBegin or not fileExtPosEnd then
		return
	end
	local fileExt = string.sub(path, fileExtPosBegin, fileExtPosEnd)
	
	local folderPosBegin, folderPosEnd = string.find(path, "\\") 
	while (folderPosBegin) do
		if string.find(path, "\\", folderPosBegin + 1) then
			folderPosEnd   = folderPosBegin
			folderPosBegin = string.find(path, "\\", folderPosBegin + 1)
		else
			break
		end
		 
	end
	
	local folderName = string.sub(path, folderPosEnd+1, folderPosBegin-1)
	local tmpPath = string.sub(path, 1, folderPosBegin)
	if not tmpPath then
		return
	end
	
	if not arrFolder[tmpPath] then
		arrFolder[tmpPath] = 0
	end
	
	arrFolder[tmpPath] = arrFolder[tmpPath] + 1
	
	local newName = nil
	if arrFolder[tmpPath] < 10 then
		newName = "00"..tostring(arrFolder[tmpPath])
	elseif arrFolder[tmpPath] < 100 then
		newName = "0"..tostring(arrFolder[tmpPath])
	else
		newName = tostring(arrFolder[tmpPath])
	end
	
	
	local newFullPath = tmpPath..newName..fileExt
	return newFullPath, newName
end

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
	
	os.execute("del /F /Q folder.txt")
	os.execute("dir /ad/b  > folder.txt")--列出当前文件夹下的子文件夹，不递归
	local folderTable = MakeTableFromFile("folder.txt")
	for i=1, #folderTable do
		local newName = GetNewFolderNum(folderTable[i])
		if newName then
			local status,_= pcall(os.rename(folderTable[i], newName))--重命名
			local info = newName .."  ==>  ".. folderTable[i].."\r\n"
			WriteFile("rename_info.txt", info)
		end
	end
	
	-- 删掉folder.txt
	os.execute("del /F /Q folder.txt")
end

function FormatIndex(index)
	local newIndex = nil
	if index < 10 then
		newIndex = "00"..tostring(index)
	elseif index < 100 then
		newIndex = "0"..tostring(index)
	else
		newIndex = tostring(index)
	end
	return newIndex
end

function RenameCurMessyFolders()
	os.execute("del /F /Q messyfolder.txt")
	os.execute("dir /ad/b  > messyfolder.txt")--列出当前文件夹下的子文件夹，不递归
	local folderTable = MakeTableFromFile("messyfolder.txt")
	local index = 0
	for i=1, #folderTable do
		index = index + 1
		local newName = FormatIndex(index)
		if newName then
			newName = "No."..newName
			local status,_= pcall(os.rename(folderTable[i], newName))--重命名
			local info = newName .."  ==>  ".. folderTable[i].."\r\n"
			WriteFile("rename_info.txt", info)
		end
	end
	os.execute("del /F /Q messyfolder.txt")
end

--将当前目录下的所有名字不规则的文件(递归),重命名
function RenameAllMessyFiles()
	--递归列出当前文件夹下的所有文件
	os.execute("del /F /Q allfiles.txt")
	os.execute(" for /r %i in (*.jpg, *.png, *.jpeg, *.gif, *.bmp) do (echo %i >> allfiles.txt)")
	
	local fileTable = MakeTableFromFile("allfiles.txt")
	
	for i=1, #fileTable do
		local newFullPath, newName = GetNewFileName(fileTable[i])
		if newFullPath then
			local status,_= pcall(os.rename(fileTable[i], newFullPath))--重命名
		end
	end
	
	--删掉allfiles.txt
	os.execute("del /F /Q allfiles.txt")
end

function RenameAllRegularFiles()
	
end
--[[--]]
function OnLoadLuaFile(a, b)
	-- RenameAllMessyFiles()
	RenameCurMessyFolders()
end