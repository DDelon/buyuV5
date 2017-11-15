local PayBase = class("PayBase")


local JAVA_CLASS_NAME = "com.weile.pay.PayApi"

function PayBase.create()
	local obj = PayBase.new();
	obj:init();
	return obj;
end

function PayBase:init()
	self.callfunc = nil;
end

function PayBase:saveData(payInfo)
	self.money = payInfo.money;
	self.detail = payInfo.detail;
end

function PayBase:doPay(payInfo)
	--发起http请求订单号
	local function callfunc(sdkCallInfo)
		--调起sdk
		if device.platform == "ios" then
			print("发起苹果支付")
			self.info = sdkCallInfo
			self:doPayIOS(sdkCallInfo);
		elseif device.platform == "android" then
			print("发起安卓支付")
			self.info = sdkCallInfo;
			self:doPayAndroid(sdkCallInfo);
		else
			print("暂未开放此平台的支付功能")
		end
	end
	self:saveData(payInfo);
	self:orderRequest(payInfo, callfunc)
end

function PayBase:orderRequest(payInfo, callfunc)
    FishGF.waitNetManager(true,nil,"RequestOrder")
	local function ordercallback_(data)
        FishGF.waitNetManager(false,nil,"RequestOrder")
        if data and data.status == 0 then
            local payArgs = checktable(data)
            table.merge(payArgs, payInfo)
            local ext = data.ext;
            if  ext ~= nil then table.merge(payArgs, ext) end
            callfunc(payArgs)
        else
            --弹出提示框是否重试
        end
    end
	FishGI.Dapi:OrderNew(payInfo, ordercallback_)
end

--苹果数据审查
function PayBase:applePayDataVerify(luastr, paytype)
    local ok, args = pcall(function()
        return loadstring(luastr)();
    end)
    if ok then
		--保存审查数据
		cc.UserDefault:getInstance():setStringForKey("applePayData", luastr);
		--保存旧的超时时间 修改转圈超时时间
		local oldVal = FishCD.OVER_TIME;
		FishCD.OVER_TIME = 9999;
		--出现屏蔽触摸层
		FishGF.waitNetManager(true,nil,"VerifyApplePayData");
		--用来屏蔽多次web请求之后收到多次回来的数据
		local isReceived = false;

		--发起web请求审核支付数据
		local function verify(args)
			local function webCallFunc(msg)
				if isReceived then
					return;
				end
				local curScene = cc.Director:getInstance():getRunningScene();
				curScene:stopActionByTag(6513);
				--删除屏蔽触摸层
				FishGF.waitNetManager(false,nil,"VerifyApplePayData")
				--清除审查数据
				cc.UserDefault:getInstance():setStringForKey("applePayData", "");
				--还原超时时间
				FishCD.OVER_TIME = oldVal;
				isReceived = true;
				if msg.status == 0 then
	            	local ret_tab = { status = msg.status, paytype = paytype, msg = "支付成功！" }
				else
					print("apple verify data fail");
				end
			end
			FishGI.Dapi:VerifyIosReceipt(args, webCallFunc)
		end
		verify(args);
		FishGF.delayExcute(5, verify, args, true, 6513);
        
    else
        printf("解析ios 参数失败 %s", luastr)
    end
end

function PayBase:doPayIOS(payInfo)
	payInfo.listener = handler(self, self.onIosPayResult);
	payInfo.money = tonumber(payInfo.money)/100;
	local cfgTable = checktable(PAY_CONFIG[payInfo.type][payInfo.productType]);

	if payInfo.type == "appstore" then
		local productids = table.values(cfgTable);
		payInfo.productidarray = json.encode(productids);
		payInfo.productid = cfgTable[payInfo.money];
		if not payInfo.productid then
			print("don't support item price check ios product config");
			return;
		end
	else
		table.merge(payInfo, cfgTable);
	end
	
	local iosClassName = "AppController";
	local methodName = "doPay";
    local luaoc = require("cocos.cocos2d.luaoc");
	local ok, ret = luaoc.callStaticMethod(iosClassName, "doPay", payInfo)
	if not ok then
		print("call oc class:"..iosClassName.." method:"..methodName.." failure");
	end
end

function PayBase:doPaySDK(payInfo)
	
    FishGI.GameCenterSdk:trySDKPay(payInfo, handle(self, self.onSdkPayResult));
end

function PayBase:doPayAndroid(args)
    self:addListener()
    local javaMethodName = "doPay"
	dump(args)
    local jsonArgs = json.encode(args)
    local cfgTable = PAY_CONFIG[args.type]
    local jsonCfg = json.encode(cfgTable)


    local javaParams = {
        args.type,
        jsonArgs,
        jsonCfg,
    }
	local luaBridge = require("cocos.cocos2d.luaj")
    local javaMethodSig = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"
    luaBridge.callStaticMethod(JAVA_CLASS_NAME, javaMethodName, javaParams, javaMethodSig)
end

function PayBase:addListener()
    local luaBridge = require("cocos.cocos2d.luaj")
    luaBridge.callStaticMethod(JAVA_CLASS_NAME, "addScriptListener", { handler(self, self.onCallback_) })
    return self 
end

function PayBase:removeListener()
    local luaBridge = require("cocos.cocos2d.luaj")
    luaBridge.callStaticMethod(JAVA_CLASS_NAME, "removeScriptListener")
end

function PayBase:closePanel()
		--关闭商店面板
	local uiShopLayer = FishGF.getLayerByName("uiShopLayer");
	if uiShopLayer ~= nil then
		uiShopLayer:hideLayer(false);
	end
	--关闭支付方式面板
	cc.Director:getInstance():getRunningScene():removeChildByTag(FishCD.TAG.PAY_VIEW_TAG);
end

function PayBase:onRechargeSuccess(resultInfo)
	--游客提示
	if not FishGI.WebUserData:isActivited() then	--"游客"
		FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000175),nil, nil, nil, 0.5)
	else
		--充值数据必须在拉起支付的时候记录下
		--显示玩家充值完成提示
		FishGF.delayExcute(0.5, function() FishGF.showSystemTip(FishGF.getChByIndex(800000351)..resultInfo.detail..FishGF.getChByIndex(800000353), nil, 3) end)
	end

	if FishGI.GAME_STATE == 2 then
		--在大厅充值
		FishGF.waitNetManager(true,nil,"doPaySDK")
		FishGI.IS_RECHARGE = 5
		FishCD.hallScene:doAutoLogin(2);
	elseif FishGI.GAME_STATE == 3 then
		--在游戏里面充值
		--通知服务器刷新数据
		FishGI.gameScene.net:sendReChargeSucceed();
		FishGI.WebUserData:initWithUserId(FishGI.WebUserData:GetUserId());
		
	end

	FishGI.eventDispatcher:dispatch("BuySuccessCall", resultInfo);

end

function PayBase:onAndroidPayResult(resultStr)
	local ok, resultInfo = pcall(function()         
		return loadstring(resultStr)();
	end)

	--如果在游戏里面充值要通知服务器充值回来
	if FishGI.GAME_STATE == 3 then
		FishGI.gameScene.net:sendBackFromCharge();
	end

	if ok then
		if resultInfo.status == 0 then
			self:onRechargeSuccess(resultInfo);
		else
            FishGF.showSystemTip(nil,800000169,1);
		end
	else
		print("android pay return string parser fail");
	end

	self:closePanel();
end

function PayBase:onIosPayResult(status, paytype, msg)
	if paytype == "appstore" and status == 0 then
		self:applePayDataVerify(msg, paytype);
	else
		local retTab = {
			status = status,
			paytype = paytype,
			msg = msg,
		}

		--如果在游戏里面充值要通知服务器充值回来
		if FishGI.GAME_STATE == 3 then
			FishGI.gameScene.net:sendBackFromCharge();
		end
		if retTab.status == 0 then
			self:onRechargeSuccess(retTab);
		else
			FishGF.showSystemTip(nil,800000169,1);
		end
	end

	self:closePanel();
end

function PayBase:onSdkPayResult(resultInfo)
	local resultTab = json.decode(resultInfo)
		
        if resultTab.resultCode == 0 then
            --成功
            self:onRechargeSuccess(resultTab);
        else
            FishGF.showSystemTip(nil,800000169,1);
        end

	if FishGI.GAME_STATE == 3 then
		FishGI.gameScene.net:sendBackFromCharge()
	end		

	self:closePanel();
end

return PayBase;