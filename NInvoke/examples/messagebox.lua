local MessageBox = function (text, caption)
	--[[
	int WINAPI MessageBox(HWND hWnd,
		LPCTSTR lpText, 
		LPCTSTR lpCaption, 
		UINT uType
	);
	--]]
	local MessageBoxW_Sig = {
		dll = "user32.dll",
		name = "MessageBoxW",
		calling_convention = "stdcall",
		return_type = "int",
		arg_types = {"pointer", "pointer", "pointer", "unsigned int"}
	}
	local MessageBoxW = NInvoke.dllimport(MessageBoxW_Sig)
	local text_arr = NInvoke.str_to_byte_array(text .. "\0", "utf16le")
	local caption_arr = NInvoke.str_to_byte_array(caption .. "\0", "utf16le")
	local MB_OK = NInvoke.ctypes.unsigned_int.new(0)
	return MessageBoxW(NInvoke.ctypes.pointer.new(0), text_arr:get_pointer(), caption_arr:get_pointer(), MB_OK)
end

MessageBox("hello messagebox", "title")
