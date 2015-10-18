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
	return a + b
end
