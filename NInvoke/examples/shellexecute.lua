local ShellExecute = function (path)
	--[[
		BOOL ShellExecuteEx(
 			SHELLEXECUTEINFO *pExecInfo
		);
	--]]
	local ShellExecuteExW_Sig = {
		dll = "Shell32.dll",
		name = "ShellExecuteExW",
		calling_convention = "stdcall",
		return_type = "int",
		arg_types = {"pointer"}
	}

	local ShellExecuteExW = NInvoke.dllimport(ShellExecuteExW_Sig)
	-- SHELLEXECUTEINFO se_info;
	local se_info = nil
	local path_buffer = NInvoke.str_to_byte_array(path .. "\0", "utf16le")
	local parameters_buffer = NInvoke.byte_array.new(NInvoke.ctypes.wchar_t.type_size(), 0)
	local SW_SHOWNORMAL = NInvoke.ctypes.int.new(1)
	local is_x64 = NInvoke.ctypes.pointer.type_size() == 8
	if is_x64 then
		-- sizeof(SHELLEXECUTEINFO) == 112
		se_info = NInvoke.byte_array.new(112)
		-- se_info.cbSize = sizeof(SHELLEXECUTEINFO) [offsetof(SHELLEXECUTEINFO, cbSize) == 0]
		NInvoke.array_view.new(se_info:get_pointer(), "unsigned long", 1)[0] = NInvoke.ctypes.unsigned_long.new(#se_info)
		-- se_info.lpFile = path_buffer [offsetof(SHELLEXECUTEINFO, lpFile) == 24]
		NInvoke.array_view.new(se_info:get_pointer():fetch_increase(24), "pointer", 1)[0] = path_buffer:get_pointer()
		-- se_info.lpParameters = parameters_buffer [offsetof(SHELLEXECUTEINFO, lpParameters) == 32]
		NInvoke.array_view.new(se_info:get_pointer():fetch_increase(32), "pointer", 1)[0] = parameters_buffer:get_pointer()
		-- se_info.nShow = SW_SHOWNORMAL; [offsetof(SHELLEXECUTEINFO, nShow) == 48]
		NInvoke.array_view.new(se_info:get_pointer():fetch_increase(48), "int", 1)[0] = SW_SHOWNORMAL
	else
		-- sizeof(SHELLEXECUTEINFO) == 60
		se_info = NInvoke.byte_array.new(60)
		-- se_info.cbSize = sizeof(SHELLEXECUTEINFO) [offsetof(SHELLEXECUTEINFO, cbSize) == 0]
		NInvoke.array_view.new(se_info:get_pointer(), "unsigned long", 1)[0] = NInvoke.ctypes.unsigned_long.new(#se_info)
		-- se_info.lpFile = path_buffer [offsetof(SHELLEXECUTEINFO, lpFile) == 16]
		NInvoke.array_view.new(se_info:get_pointer():fetch_increase(16), "pointer", 1)[0] = path_buffer:get_pointer()
		-- se_info.lpParameters = parameters_buffer [offsetof(SHELLEXECUTEINFO, lpParameters) == 20]
		NInvoke.array_view.new(se_info:get_pointer():fetch_increase(20), "pointer", 1)[0] = parameters_buffer:get_pointer()
		-- se_info.nShow = SW_SHOWNORMAL; [offsetof(SHELLEXECUTEINFO, nShow) == 28]
		NInvoke.array_view.new(se_info:get_pointer():fetch_increase(28), "int", 1)[0] = SW_SHOWNORMAL
	end
	return ShellExecuteExW(se_info:get_pointer())
end

ShellExecute("notepad.exe")
