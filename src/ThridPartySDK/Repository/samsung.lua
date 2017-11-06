local SamSungSdkInterface = class("SamSungSdkInterface", FishGI.GameCenterSdkBase)

--[[

Java 方法原型:
public static void doPay(final int payAmount, int cbId)

Java 方法原型:
public static void doLogin()

-- Java 类的名称
local className = "weile/buyu/game/AppActivity"
-- 调用 Java 方法
luaj.callStaticMethod(className, "doBilling", args)

]]

function SamSungSdkInterface:trySDKLogin(info, loginCB)
    local function callBackDoLogin( strJson )
        print("--------callBackDoLogin", strJson)
        local resultTab = LIB_CJSON.decode(strJson)
        local resultMsg = LIB_CJSON.decode(resultTab.resultMsg)
		loginCB(resultMsg)
    end

    self:doLogin(info, callBackDoLogin)
    return true
end

function SamSungSdkInterface:doSDKLogout(info, logoutCB)
    self:doLogout(info, logoutCB)
    return true
end

function SamSungSdkInterface:trySDKGameExit(info, exitCallback)
    self:doGameExit(info, exitCallback)
    return true
end

function SamSungSdkInterface:trySDKPay(payArgs, payCB)
    local tWaresidList = {}
    tWaresidList["830000001"] = 1
    tWaresidList["830000002"] = 2
    tWaresidList["830000003"] = 3
    tWaresidList["830000004"] = 4
    tWaresidList["830000005"] = 5
    tWaresidList["830000006"] = 6
    tWaresidList["830000007"] = 7
    tWaresidList["830000008"] = 8
    tWaresidList["830000009"] = 9
    tWaresidList["830000010"] = 10
    tWaresidList["830000011"] = 11
    tWaresidList["830000012"] = 12
    tWaresidList["830000013"] = 13
    tWaresidList["830000014"] = 14
    tWaresidList["830000015"] = 15
    local payInfo = {}
    payInfo.userid  = FishGI.myData.playerId
    payInfo.order   = payArgs.orderid
    payInfo.waresid = tWaresidList[tostring(payArgs.goods)]
    payInfo.price   = payArgs.money
    payInfo.callbackurl = self:getPayCallBackUrl("samsung")
    --{"goods":830000001,"token":"f0c7f383eb261b4cb6ad16adf228ff17","msg":"done","ingame":1,"body":"鱼币 830000001 x1","roomid":0,"callbackurl":"https://payback.weile.com/callback/alipay/264/207/1.1.5/0/","status":0,"money":600,"udid":"C2D6AFB6F8CC68D6371D91C175AA3FBA4A9A9596","subject":"30000鱼币","type":"alipay_client","ext":{"partner":"2088221603340274","email":"jiaxianghudong@weile.com"},"debug":1,"orderid":"1704274949026739","virtual":0,"autobuy":1}
    self:doPay(payInfo, payCB)
    return true
end

function SamSungSdkInterface:doGetTokenAndSsoid(loginCB)
    self:doImpl("doGetTokenAndSsoid", {}, loginCB)
    return true
end

function SamSungSdkInterface:doGetUserInfo(loginCB)
    self:doImpl("doGetUserInfo", {}, loginCB)
    return true
end

function SamSungSdkInterface:doReportUserGameInfoData(loginCB)
    local tGameInfo = {}
    tGameInfo.gameId = tostring(GAME_ID)
    tGameInfo.service = ""
    tGameInfo.role = ""
    tGameInfo.grade = ""
    self:doImpl("doReportUserGameInfoData", tGameInfo, loginCB)
    return true
end

function SamSungSdkInterface:doGetVerifiedInfo(loginCB)
    self:doImpl("doGetVerifiedInfo", {}, loginCB)
    return true
end

--退出房间
function SamSungSdkInterface:doGameExit(argsTab, exitCallback)
    self:doImpl("doGameExit", argsTab, exitCallback)
end

return SamSungSdkInterface