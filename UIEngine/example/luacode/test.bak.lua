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

function GetAllLinkFromFile(fileName)
	local file = assert(io.open("I:\\UIEngine\\example\\luacode\\data.txt",'r'))
	local data  = file:read("*all")
	file:close()
	
	local linkList = ""
	for link in string.gmatch(data, "href=\"(.-)\"") do
		local pos = string.find(link, "http://www.itokoo.com/read.php")
		if pos and pos > 0 then
			linkList = linkList..link.."\n"
		end
    end
	
	SaveDataToFile("I:\\UIEngine\\example\\luacode\\rosi\\result.txt", linkList)
end

function GetKuaipanLink(htmlPath, srcLink)
	local file = assert(io.open(htmlPath,'r'))
	local data  = file:read("*all")
	file:close()
	
	local linkList = ""
	for link in string.gmatch(data, "href=\"(.-)\"") do
		local pos = string.find(link, "kuai.xunlei.com")
		if pos and pos > 0 then
			linkList = linkList..link.."\n"
		end
    end
	
	if linkList == "" then
		print("can not find kuaipan link!")
		SaveDataToFile("I:\\UIEngine\\example\\luacode\\errorLink.txt", srcLink.."\n"..htmlPath.."\n")
	end
	SaveDataToFile("I:\\UIEngine\\example\\luacode\\result2.txt", linkList)
end

function GetLixianLink(htmlPath, srcLink)
	local file = assert(io.open(htmlPath,'r'))
	local data  = file:read("*all")
	file:close()
	
	local linkList = ""
	for link in string.gmatch(data, "file_url=\"(.-)\"") do
		-- local pos = string.find(link, "http://kuai.xunlei.com")
		-- if pos and pos > 0 then
			linkList = linkList..link.."\n"
		-- end
    end
	
	if linkList == "" then
		local title = __GetTitle(data)
		if string.find(title, "删除") or string.find(title, "屏蔽") then
			print("deleted or illegal")
			SaveDataToFile("I:\\UIEngine\\example\\luacode\\Deleted.txt", srcLink.."  "..title.."\n")
			SaveDataToFile("I:\\UIEngine\\example\\luacode\\errorLink2.txt", srcLink.."  "..htmlPath.."\n")
		else
			-- local titleW = ANSIToLuaString(title)
			print("can not find lixian link!yanzhengma "..tostring(title))
			-- titleW = ANSIToLuaString(title)
			assert(false)
		end
	end
	SaveDataToFile("I:\\UIEngine\\example\\luacode\\result3.txt", linkList)
end

function err(msg)
	MsgBox(msg)
end
	
local cntA= 0
local cntB= 0
	
function StepZero()
	--先手动获取站点规整的网页代码，通过GetAllLinkFromFile抓取所有分集链接
	xpcall(function()
		GetAllLinkFromFile("data.txt")
	end, err)
end

function StepOne()
	xpcall(function()
		--通过请求分集链接（result.txt），从中抓取迅雷快盘链接，存入result2.txt
		    local file = assert(io.open("I:\\UIEngine\\example\\luacode\\result.txt",'r'))
		    local line = file:read()
			while line do
				cntA = cntA + 1
				print("cur line: "..tostring(cntA).."\n")
				local filePath = GetHttpContentCacheFile(line)
				if filePath then
					GetKuaipanLink(filePath, line)
					cntB = cntB + 1
				else
					print("bad link! \n")
					SaveDataToFile("I:\\UIEngine\\example\\luacode\\errorLink.txt", line.."\n")
				end
				
				line = file:read()
				Delay(1)
			end
	end, err)
end

function Delay(secondDelay)
	if not tonumber(secondDelay) then
		return
	end
	
	local beginTime = os.time()
	local endTime   = beginTime
	while(endTime - beginTime < tonumber(secondDelay)) do
		endTime = os.time()
	end
end

function StepTwo()
	xpcall(function()
		--依次打开result2.txt里的快传链接，手动下载！
		    local file = assert(io.open("I:\\UIEngine\\example\\luacode\\result2.txt",'r'))
		    local line = file:read()
			while line do
				cntA = cntA + 1
				print("cur line: "..tostring(cntA).."\n")
				--加个延时吧，怪怪的
				Delay(1)
				LuaShellExecute(0, "open", line, 0, 0, "SW_SHOWNORMAL")
				line = file:read()
			end
	end, err)
end

function StepThree()
	xpcall(function()
		--请求快传页面（result2.txt），从中找出file_url=”“这样的url，存入（result3.txt）
		    local file = assert(io.open("I:\\UIEngine\\example\\luacode\\result2.txt",'r'))
		    local line = file:read()
			while line do
				cntA = cntA + 1
				print("cur kuaichuan line: "..tostring(cntA).."\n")
				Delay(1)
				local filePath = GetHttpContentCacheFile(line)--.."&time="..os.time())
				if filePath then
					GetLixianLink(filePath, line)
					cntB = cntB + 1
				else
					print("bad link! \n")
					assert(false)
					-- SaveDataToFile("I:\\UIEngine\\example\\luacode\\errorLink2.txt", line.."\n")
				end
				
				line = file:read()
			end
	end, err)
end


function __GetKuaipanLink(data)
	local linkList = ""
	for link in string.gmatch(data, "href=\"(.-)\"") do
		local pos = string.find(link, "http://kuai.xunlei.com")
		if pos and pos > 0 then
			linkList = linkList..link.."  "
		end
    end
	
	if "" == linkList then
		return nil
	end
	return linkList
end

function __GetBaiduLink(data)
	local linkList = ""
	
	for link in string.gmatch(data, "href=\"(.-)\"") do
		local pos = string.find(link, "http://pan.baidu.com")
		if pos and pos > 0 then
			linkList = linkList..link.."  "
		end
    end
	
	if "" == linkList then
		return nil
	end
	
	--由于编码问题，匹配密码尚未完成
	local password = string.find(data, "密码:%s-(%w%w%w%w)")
		
	MsgBox(tostring(password))
	return linkList, password
end

function __GetHuaweiLink(data)
	local linkList = ""
	for link in string.gmatch(data, "href=\"(.-)\"") do
		local pos = string.find(link, "http://dl.dbank.com")
		if pos and pos > 0 then
			linkList = linkList..link.."  "
		end
    end
	
	if "" == linkList then
		return nil
	end
	return linkList
end
--http://www.vdisk.cn/down/index/9067695A5727   威盘网
function __GetTitle(data)
	local title = string.match(data, "<title>(.-)</title>")
	
	if title then
		local pos = string.find(title, "- Powered by phpwind")
		if pos and pos > 2 then
			title = string.sub(title, 1, pos - 2)
		end
	end
	return title
end

function __DevilSpider()
	xpcall(function()
		local id = 26522--25770--21824--20000
		while id < 28420 do
			print("cur ID: "..tostring(id))
			local url = "http://www.itokoo.com/read.php?tid="..tostring(id)
			
			local filePath = GetHttpContentCacheFile(url)
			if filePath then
				local file = assert(io.open(filePath,'r'))
				local data  = file:read("*all")
				file:close()
				
				local title = __GetTitle(data)
				title = title or "未知标题"
				
				file = io.open("I:\\UIEngine\\example\\luacode\\ID2TitleInfo.txt", "a")
				file:write(tostring(id).."  "..title.."\n")
				file:close()
				
				if string.find(title, "Beautyleg") and not string.find(title, "HD") then
					local xunleiKuaipanUrl 		   = __GetKuaipanLink(data)
					-- local baiduYunUrl, baiduYunPsw = __GetBaiduLink(data)
					-- local hueweiWangPanUrl 		   = __GetHuaweiLink(data)
					
					if not xunleiKuaipanUrl then --and not baiduYunUrl and not hueweiWangPanUrl 
						print("can not find any link! ID="..tostring(id))
						SaveDataToFile("I:\\UIEngine\\example\\luacode\\NoFindLinkID.txt", tostring(id).."\n")
					else
						if xunleiKuaipanUrl then
							file = io.open("I:\\UIEngine\\example\\luacode\\XunleiUrlInfo.txt", "a")
							file:write(tostring(id).."  "..xunleiKuaipanUrl.."\n")
							file:close()
						end
						-- if baiduYunUrl then
							-- file = io.open("I:\\UIEngine\\example\\luacode\\BaiduUrlInfo.txt", "a")
							-- file:write(tostring(id).."  "..baiduYunUrl.."  密码: "..tostring(baiduYunPsw).."\n")
							-- file:close()
						-- end
						-- if hueweiWangPanUrl then
							-- file = io.open("I:\\UIEngine\\example\\luacode\\HuaweiUrlInfo.txt", "a")
							-- file:write(tostring(id).."  "..hueweiWangPanUrl.."\n")
							-- file:close()
						-- end
					end
				else
					print("HD video url")
				end
				id = id + 1
			else
				print("can not get catch file! ID="..tostring(id).."delay 6 second")
				Delay(6)
				--下个循环继续尝试
				-- SaveDataToFile("I:\\UIEngine\\example\\luacode\\NoCatchFileID.txt", tostring(id).."\n")
			end
		end
	end, err)
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

function __Finder(key, fileName)
	local idToTitleList  = ReadFile("I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\ID2TitleInfo.txt")--文件必须是utf8编码，否则匹配不到中文
	local kuaipanUrlList = ReadFile("I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\XunleiUrlInfo.txt")
	local keyInfoFile = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\"..fileName..".txt"
	local keyOnlyKuaiPanUrlFile = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\"..fileName.."_url.txt"
	for id,title in string.gmatch(idToTitleList, "(%d+)%s+(%C+)") do
		-- 
		local pos = string.find(title, key)
		if pos then
			print("title: "..tostring(title))
			local kuaipanUrl = string.match(kuaipanUrlList, id.."%s+(%C+)")
			
			if kuaipanUrl then
				WriteFile(keyInfoFile, id.." "..title.." "..kuaipanUrl.."\n")
				print("find target at id: "..tostring(id))
				WriteFile(keyOnlyKuaiPanUrlFile, kuaipanUrl.."\n")
				-- MsgBox("id: "..tostring(id).." title: "..tostring(title).." kuaipanUrl: "..tostring(kuaipanUrl))
			else
				WriteFile(keyInfoFile, id.." "..title.."\n")
			end
		end
    end
end

function __FindVideo(filePath, resultFileName)
	local data = ReadFile(filePath)--文件必须是utf8编码，否则匹配不到中文
	if not data then
		return
	end
	for id,titleAndUrl in string.gmatch(data, "(%d+)%s-(%C+)") do
		if titleAndUrl and string.find(titleAndUrl, "V/") then
			print("find a video!")
			local url = string.match(titleAndUrl, "(http:%C+)")
			if url then
				print("find a video url!")
				WriteFile(resultFileName, url.."\n")
			end
		end
	end
end

function __FindVideoLineByLine()
	local infoFilePath = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\ID2TitleInfo.txt"
	local urlFilePath = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\XunleiUrlInfo.txt"
	local outFilePath = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\BeautylegVideo208-LostID.txt"
	local outFilePathB = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\BeautylegVideo208-Kuaipan.txt"
	local file = assert(io.open(infoFilePath,'r'))
	local urlFileData = ReadFile(urlFilePath)
	
	local line = file:read()
	local cnt = 0
	while line do
		local id, title = string.match(line, "(%d+)%s-(%C+)")
		if title and string.find(title, "Beautyleg") and string.find(title, "HD") then
			local number = string.match(title, "No.(%d+)")
			if number and tonumber(number) and tonumber(number) > 207 then
				local urlKuaipan = string.match(urlFileData, id.."%s-(%C+)")
				if urlKuaipan then
					WriteFile(outFilePathB, urlKuaipan.."\n")
				else
					WriteFile(outFilePath, id.."\n")
					print("lost number: "..tostring(number))
				end
			else
				-- print(title)
			end
		else
			-- MsgBox(title)
		end
		
		cnt = cnt + 1
		line = file:read()
	end
	print(tostring(cnt))
	file:close()
end

function __SpideSepcificIdLineByLine(filpath)
	local file = assert(io.open(filpath,'r'))
	local line = file:read()
	local url = "http://www.itokoo.com/read.php?tid="
	
	while line do
		if tonumber(line) then
			local filePath = GetHttpContentCacheFile(url..line)
			if filePath then
				local cacheFile = assert(io.open(filePath,'r'))
				local data  = cacheFile:read("*all")
				cacheFile:close()
				local xunleiKuaipanUrl = __GetKuaipanLink(data)
				if not xunleiKuaipanUrl then
					print("can not find xunleiKuaipanUrl link! ID="..tostring(line))
					SaveDataToFile("I:\\UIEngine\\example\\luacode\\BeautylegVideo208_kuaipanLost.txt", tostring(line).."\n")
				else
					print("success !!!")
					SaveDataToFile("I:\\UIEngine\\example\\luacode\\BeautylegVideo208_kuaipanEx.txt", tostring(xunleiKuaipanUrl).."\n")
				end
				line = file:read()
			else
				print("can not get catch file! ID="..tostring(line).."delay 6 second")
				Delay(6)
			end
		else
			line = file:read()
		end
	end
end

local KeyTable = {
	-- {"HD高", "BeautylegVideo"},
	{"Beautyleg"},
	{"丽柜", "LiGui"},
	-- {"上海炫彩时尚摄影沙龙", "XuancaiShalong"},
	-- {"丽阁影像", "LiGe"},
	-- {"V880维拉少女屋", "WeiLaShaoNv"},
	-- {"禁忌摄影", "JinJiSheYing"},
	-- {"TyingArt"},
	-- {"DGC"},
	-- {"丝图阁", "SiTuGe"},
	-- {"RQ-STAR"},
	-- {"TopQueen"},
	-- {"TopQueenEX"},
	-- {"NS Eyes"},
	-- {"YS Web"},
	-- {"Dynamitechannel"},
	-- {"image.tv"},
	-- {"拍美VIP", "PaiMeiVIP"},
	-- {"王朝", "WangChaoGuiZu"},
	-- {"Wanibooks"},
	-- {"BWH"},
}

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
		 local path = "I:\\UIEngine\\example\\luacode\\itokoo10000-28999\\BeautylegVideo208-LostID.txt"
		 -- xpcall(function() __FindVideoLineByLine() end, err)
		 -- xpcall(function() __SpideSepcificIdLineByLine(path) end, err)
	 
	 
	-- StepZero()--从”资源规整“（data.txt） 提取出http://www.itokoo.com/read.php?tid=XXX 存入result.txt
	-- StepOne()--将”result.txt“ 提取出快盘链接 存入result2.txt
	-- StepTwo()--依次打开result2.txt里的快传链接，手动下载！ 很慢很麻烦
	StepThree()--将”result2.txt“ 提取出离线链接 存入result3.txt！！！！可以省掉steptwo了
	
	-- local i = 9921--9823--8627--8518--8502--7273--7110--4834--3937--147--3937   MAX：28399
	-- while i < 10000 do
		-- __DevilSpider()
		-- i = i + 1
	-- end
	-- MsgBox("cntA: "..tostring(cntA).." cntB: "..tostring(cntB))

	-- for index=1, #KeyTable do
		-- local key = KeyTable[index][1]
		-- local file = KeyTable[index][2]
		-- print("cur key: "..tostring(key).." file: "..tostring(file))
		-- xpcall(function() __Finder(key, file or key) end, err)
	-- end
	
	-- local folder = "I:\\UIEngine\\example\\luacode\\itokoo1-9999\\Info\\"
	-- for index=1, #KeyTable do
		-- local key = KeyTable[index][1]
		-- local file = KeyTable[index][2]
		-- local inFile = file or key
		-- local outFile = folder..inFile.."_VideoUrl.txt"
		-- inFile = folder..inFile..".txt"
		-- xpcall(function() __FindVideo(inFile, outFile) end, err)
	-- end
	
	
	local memCount = collectgarbage("count")
	MsgBox("mem cost: "..tostring(memCount).."KB  functions count: "..tostring(count).."\n"..info)
	return 1
	--return SetGlobalObject(a, b) + 230
end


--<title>[Beautyleg]2010.11.22 No.470 Penny[59P/102M] - Powered by phpwind</title>
--密码: w9p4