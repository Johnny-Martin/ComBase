
local Json = {}
-- t={
	-- ["key_a"] = value_a,
	-- ["key_b"] = {["key_c"] = value_c},
-- }

-- json data: 

--be careful about the sub-table which point to it's parent-table
function Json:Encode(tInfo)
	if "table" ~= type(tInfo) then
		return nil
	end
	
	local result = ""
	for key, value in pairs(tInfo) do
		--The '"' can not in "value"
		if "table" == type(value) then
			result = result.."\""..tostring(key).."\":"..tostring(self:Encode(value))..","
		else
			result = result.."\""..tostring(key).."\":\""..tostring(value).."\","
		end
	end
	
	result = string.sub(result,1,string.len(result)-1)
	return "{"..result.."}"
end

function Json:Decode(sInfo)

end