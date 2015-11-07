--[[
HANDLE WINAPI CreateToolhelp32Snapshot(
	DWORD dwFlags,
	DWORD th32ProcessID
);
--]]
local CreateToolhelp32Snapshot_Sig = {
	dll = "kernel32.dll",
	name = "CreateToolhelp32Snapshot",
	calling_convention = "stdcall",
	return_type = "pointer",
	arg_types = {"unsigned long", "unsigned long"}
}
--[[
BOOL WINAPI Process32First(
	HANDLE hSnapshot,
	LPPROCESSENTRY32 lppe
);
--]]
local Process32FirstW_Sig = {
	dll = "kernel32.dll",
	name = "Process32FirstW",
	calling_convention = "stdcall",
	return_type = "int",
	arg_types = {"pointer", "pointer"}
}
--[[
BOOL WINAPI Process32Next(
	HANDLE hSnapshot,
	LPPROCESSENTRY32 lppe
);
--]]
local Process32NextW_Sig = {
	dll = "kernel32.dll",
	name = "Process32NextW",
	calling_convention = "stdcall",
	return_type = "int",
	arg_types = {"pointer", "pointer"}
};
--[[
BOOL WINAPI CloseHandle(
	HANDLE hObject
);
--]]
local CloseHandle_Sig = {
	dll = "kernel32.dll",
	name = "CloseHandle",
	calling_convention = "stdcall",
	return_type = "int",
	arg_types = {"pointer"}
};

local CreateToolhelp32Snapshot = NInvoke.dllimport(CreateToolhelp32Snapshot_Sig)
local Process32FirstW = NInvoke.dllimport(Process32FirstW_Sig)
local Process32NextW = NInvoke.dllimport(Process32NextW_Sig)
local CloseHandle = NInvoke.dllimport(CloseHandle_Sig)

local hProcessSnap = CreateToolhelp32Snapshot(NInvoke.ctypes.unsigned_long.new(2), NInvoke.ctypes.unsigned_long.new(0))
local INVALID_HANDLE_VALUE = NInvoke.byte_array.new(NInvoke.ctypes.pointer.type_size(), 0xff):get_pointer():deref_as_pointer()
if hProcessSnap ~= INVALID_HANDLE_VALUE then
	-- PROCESSENTRY32 pe32;
	local pe32 = nil
	local is_x64 = NInvoke.ctypes.pointer.type_size() == 8
	if is_x64 then
		-- sizeof(PROCESSENTRY32) == 568
		pe32 = NInvoke.byte_array.new(568)
	else
		-- sizeof(PROCESSENTRY32) = 556
		pe32 = NInvoke.byte_array.new(556)
	end
	-- pe32.dwSize = sizeof(PROCESSENTRY32) [offsetof(PROCESSENTRY32, dwSize) == 0]
	NInvoke.array_view.new(pe32:get_pointer(), "unsigned long", 1)[0] = NInvoke.ctypes.unsigned_long.new(#pe32)
	local open_result = Process32FirstW(hProcessSnap, pe32:get_pointer())
	if open_result:to_number() == 0 then
		CloseHandle(hProcessSnap)
		return nil
	end
	repeat
		-- [offsetof(PROCESSENTRY32, th32ProcessID) == 8]
		local process_id = NInvoke.array_view.new(pe32:get_pointer():fetch_increase(8), "unsigned long", 1)[0]
		local szExeFile
		if is_x64 then
			-- [offsetof(PROCESSENTRY32, szExeFile) == 44]
			szExeFile = NInvoke.array_view.new(pe32:get_pointer():fetch_increase(44), "wchar_t", 260)
		else
			-- [offsetof(PROCESSENTRY32, szExeFile) == 36]
			szExeFile = NInvoke.array_view.new(pe32:get_pointer():fetch_increase(36), "wchar_t", 260)
		end
		local temp_arr = NInvoke.byte_array.new(NInvoke.clib.wcslen(szExeFile:get_pointer()):to_number() * 2)
		NInvoke.clib.memcpy(temp_arr:get_pointer(), szExeFile:get_pointer(), #temp_arr)
		print(NInvoke.byte_array_to_str(temp_arr, "utf16le") .. "[" .. tostring(process_id) .. "]")
		open_result = Process32NextW(hProcessSnap, pe32:get_pointer())
	until open_result:to_number() == 0
	CloseHandle(hProcessSnap)
end
