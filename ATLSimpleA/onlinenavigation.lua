function GetSubTable()
	local t = {}
	local coreVersion
	if XMP and XMP.XmpPre.self then
		coreVersion = XMP.XmpPre.self:Call("GetCoreBuildNum")
	end
	local count = 0
	local recreationpluginVersion = XMP.XmpPre.self:Call("GetPluginVersion","recreationplugin")
	local webbrowserpluginVersion = XMP.XmpPre.self:Call("GetPluginVersion","webbrowserplugin")
	XMP.LOG("onlineNavigation.lua: recreationpluginVersion "..recreationpluginVersion.." webbrowserpluginVersion "..webbrowserpluginVersion)
	if coreVersion and ( ( (coreVersion == 3378 or coreVersion == 3415 or  coreVersion == 3417 or coreVersion == 3464 or coreVersion == 3471 or coreVersion == 3474 or coreVersion == 3480 or coreVersion == 3487 )
	and recreationpluginVersion > 1 and webbrowserpluginVersion > 1) or coreVersion > 3500 ) then
		local MyGame2 = {
			["sname"]     = "游戏",
			["sid"]       = "mygame2",
			["surl"]  	  = "http://pianku5.xmp.kankan.com/jumpurl/jumpto.html?cmd=mygame2",
			["srealurl"]  = "http://jump.niu.xunlei.com:8080/iEjqYn",
			["bkeepopen"] = false,
		}
		table.insert(t, MyGame2)
		local mygame2Config = XMP.Plugins.Recreation:GetConfigByID(MyGame2["sid"])
		if mygame2Config and 0 == mygame2Config["state"] then
		else
			count = count + 1
			--发统计
			XMP.XmpPre.Utility:SendConvHttpStat("XMP-navigation",MyGame2["sid"])
		end
	end
	if coreVersion and coreVersion >= 3511 then
		local GirlVideo = {
			["sname"]	  = "美女视频",
			["sid"]       = "girlvideo",
			["surl"]  	  = "http://pianku5.xmp.kankan.com/jumpurl/jumpto.html?cmd=girlvideo",
			["srealurl"]  = "http://show.v.xunlei.com/?ref=tab",
			["bkeepopen"] = false,
		}
		table.insert(t, GirlVideo)
		local girlVideoConfig = XMP.Plugins.Recreation:GetConfigByID(GirlVideo["sid"])
		if girlVideoConfig and 0 == girlVideoConfig["state"] then
		else
			count = count + 1
			--发统计
			XMP.XmpPre.Utility:SendConvHttpStat("XMP-navigation",GirlVideo["sid"])
		end
	end
	
	local vip_saleConfig = XMP.Plugins.Recreation:GetConfigByID("vip_sale")
	local bhasshow = vip_saleConfig and true or false
	local VipSale = {
			["sname"]	  = "会员特卖",
			["sid"]       = "vip_sale",
			["surl"]  	  = "http://pianku5.xmp.kankan.com/jumpurl/jumpto.html?cmd=vip_sale",
			["srealurl"]  = "http://act.vip.xunlei.com/vip/2015/zqb/?from=XMP",
			["bkeepopen"] = false,
		}
	XMP.LOG("bhasshow: ", bhasshow, " count: ", count, " coreVersion: ", coreVersion)
	if not bhasshow and count < 2 and coreVersion and coreVersion >= 3511 then
		table.insert(t, VipSale)
		
		--尚未显示过”会员特卖“
		vip_saleConfig = {}
		vip_saleConfig["bhasshow_vip_sale"] = true
		XMP.Plugins.Recreation:SetConfigByID("vip_sale", vip_saleConfig)
		--发统计
		XMP.XmpPre.Utility:SendConvHttpStat("XMP-navigation",VipSale["sid"])
	elseif bhasshow and coreVersion and coreVersion >= 3511 then
		table.insert(t, VipSale)
		if vip_saleConfig and 0 == vip_saleConfig["state"] then
		else
			--发统计
			XMP.XmpPre.Utility:SendConvHttpStat("XMP-navigation",VipSale["sid"])
		end
	end
	
	return t
end