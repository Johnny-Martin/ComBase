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

function OnLoadLuaFile(a, b)
	local info = ""
	local count = 0
	local tmp = {[1] = "sha", [2] = "be"}
	for key, value in pairs(_G) do
		if "function" ~= type(value) then
			info = info.." key: "..tostring(key).." value: "..tostring(value).."\n"
			count = count + 1
		end
	end
	
	local memCount = collectgarbage("count")
	MsgBox("mem cost: "tostring(memCount).."KB  functions count: "..tostring(count).."\n"..info)
	return 1
	--return SetGlobalObject(a, b) + 230
end

