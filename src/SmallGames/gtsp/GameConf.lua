local GameConf = class("GtspGameConf", nil)

GameConf.LOGIN_CONFIG = {
    baidu = {
        typeName = "baidu",
    },
    huawei = {
        typeName = "huawei",
    },
    jinli = {
        typeName = "jinli",
    },
    lenovo = {
        typeName = "lenovo",
    },
    mi = {
        typeName = "xiaomi",
    },
    mz = {
        typeName = "mz",
    },
    oppo = {
        typeName = "oppo",
    },
    qihu = {
        typeName = "login360",
    },
    samsung = {
        typeName = "samsung",
    },
    vivo = {
        typeName = "vivo",
    },
    yyb = {
        typeName = "ysdk",
    },
}

GameConf.PRODUCT_CONFIG = {
    {
        id="830000001",
        recharge="600",
        recharge_num="30000",
        recharge_res="dog_coins_1.png",
        recharge_name="30000鱼币",
        recharge_type=1,
    },
    {
        id="830000002",
        recharge="1200",
        recharge_num="60000",
        recharge_res="dog_coins_2.png",
        recharge_name="60000鱼币",
        recharge_type=1,
    },
    {
        id="830000003",
        recharge="3000",
        recharge_num="165000",
        recharge_res="dog_coins_3.png",
        recharge_name="150000鱼币",
        recharge_type=1,
    },
    {
        id="830000004",
        recharge="5000",
        recharge_num="287500",
        recharge_res="dog_coins_4.png",
        recharge_name="250000鱼币",
        recharge_type=1,
    },
    {
        id="830000005",
        recharge="10800",
        recharge_num="648000",
        recharge_res="dog_coins_5.png",
        recharge_name="540000鱼币",
        recharge_type=1,
    },
    {
        id="830000006",
        recharge="32800",
        recharge_num="2050000",
        recharge_res="dog_coins_6.png",
        recharge_name="1640000鱼币",
        recharge_type=1,
    },
    {
        id="830000007",
        recharge="64800",
        recharge_num="4212000",
        recharge_res="dog_coins_7.png",
        recharge_name="3240000鱼币",
        recharge_type=1,
    },
}

GameConf.PAY_CONFIG = {
    wechat = {
        typeName = "wechat",
        payData = {},
    },
    alipay = {
        typeName = "alipay_client",
        payData = {
            partner="",
            email=" ",
            rsa_private="MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANY3GTv3SaqX/zNMhf4at61rJg1Cm9GpdHujJ3N5LnEtjXL6BwtGUlPe1A+XOSXoqZB1BeOi+aiE6yHfNs643nIPCLsSAmKZSulrssCDjnPOG/MuL8lcOT8c+rbezeWYmS2sm6ynE2lXxNHEA5u1bP+okR/zEdJIB01YgZUFnTJ1AgMBAAECgYEAqhRBIs1qXdoks1Q0ptYLs9L4+VpDYSoL5AZcUmCKsS2buwgtA5Sn1RN8h4xnwWODDcD8FgrV8ijmj5QsbeF2KuCElg+p4g4Moo1A/xznXfDm0ATY+8IzChuQkBtGoBNW1E5PWD0+BkaEf0+FhJDKVzGRAE+JLEyFbTDS9uZ0XwECQQD51oV+utG3cfNqBnedcrO4z6hROUs0tn+y+PuKPZUlVCH09YEkGyDX1Z9fBIRaUCFNiApl/VDIEChpNkbskqphAkEA23+npV2ABGu/ODJ1Fuu/fdJYIPXSGdvu9OalBkP2EpcKulFAH3gklQRfbkp5EBSX7GCFQkBm021hOLKdGA0IlQJBAND1ycXLP2itWCfPrO/1ZbgnhuIYh3xZP8lTUh+3ji0ghx440ICAaCHdvGRehMx8xL3yELBpBM2wJfyJtxxbN0ECQDssmwGVx2Fpus9nqvFW9PTytBeOremSxUT4uRyLTdeNKLM6HFNfjF0wJJoTMbgIFT0AeGx3+ECfiEpEvN0zBlECQGZ9JA9y91mKJS8ZlJrDc2HTB/wphQf5w5Mp+JuKmAWCk5I+k0TYm/8eAD9zTCTDTfrFG/kn0E3L87sEJ3KKGEY="
        },
    },
    unionpay = {
        typeName = "unionpay_client",
        payData = {mode="00"},
    },
    apple = {
        typeName = "appstore",
        payData = {
            [6] = "com.weile.buyu.money1",
            [12] = "com.weile.buyu.money2",
            [30] = "com.weile.buyu.money3",
            [50] = "com.weile.buyu.money4",
            [108] = "com.weile.buyu.money5",
            [328] = "com.weile.buyu.money6",
            [648] = "com.weile.buyu.money7"
        },
    },
    baidu = {
        typeName = "baidu",
    },
    huawei = {
        typeName = "huawei",
    },
    jinli = {
        typeName = "jinli",
    },
    lenovo = {
        typeName = "lenovo",
    },
    mi = {
        typeName = "mi",
    },
    mz = {
        typeName = "meizu",
    },
    oppo = {
        typeName = "oppo",
    },
    qihu = {
        typeName = "360",
    },
    samsung = {
        typeName = "samsung",
    },
    vivo = {
        typeName = "vivo",
    },
    yyb = {
        typeName = "midas",
    },
}

-- 充值结果：SUCCESS成功 FAILED失败
-- 注：不可改动
GameConf.PAY_RESULT_STATUS = {
    SUCCESS = 0,
    FAILED = 1,
}

-- 微信分享模式：SESSION朋友 TIMELINE朋友圈 FAVORITE收藏
-- 注：不可改动
GameConf.WECHAT_SHARE_MODEL = {
    SESSION = 0,
    TIMELINE = 1,
    FAVORITE = 2,
}

-- 分享类型：Image图片
-- 注：不可改动
GameConf.SHARE_TYPE = {
    Image = 1,
}

-- 分享结果：SUCCESS成功 FAILED失败
-- 注：不可改动
GameConf.SHARE_RESULT_STATUS = {
    SUCCESS = 0,
    FAILED = 1,
}

function GameConf.create( ... )
    local gameConf = GameConf.new()
    gameConf:init()
    return gameConf
end

function GameConf:init()
    -- 充值类型：wechat alipay unipay apple baidu huawei jinli lenovo mi oppo qihu vivo yyb
    self.pay_type = "wechat"
    -- 是否是第三方充值渠道
    self.is_thirdpay_sdk = false
    -- 分享类型：wechat
    self.share_type = "wechat"
    -- 扩展数据
    self.extend_data = {
        res_path = "SmallGames/gtsp",
        money_prop_res = "prop_001.png",
        recharge_product_config = GameConf.PRODUCT_CONFIG,
        recharge_unit = "鱼币",
        recharge_point_of_money = 1,
    }
end

function GameConf:getPayData()
    return {
        common = {
            android = {
                className = "com.weile.pay.PayApi",
                methodName = "doPay",
                methodSig = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                callbackResult = "addScriptListener",
            },
            ios = {
                className = "AppController",
                methodName = "doPay",
            },
        },
        wechat = {},
        apliay = {},
        unionpay = {},
        apple = {},
        gcsdk = {
            android = {
                className = "com.weile.gcsdk.GameCenterBase",
                methodName = "doPayForProduct",
                methodSig = "(Ljava/lang/String;I)I",
                callbackResult = nil,
            },
            ios = {
                className = "AppController",
                methodName = "doPay",
            },
        },
        baidu = {},
        mi = {},
        oppo = {},
        qihu = {},
        vivo = {},
        huawei = {},
        jinli = {},
        lenovo = {},
        yyb = {},
        samsung = {},
        mz = {},
    }
end

function GameConf:getLoginTypeName(loginType)
    if loginType == nil then
        loginType = self.login_type
    end
    return GameConf.LOGIN_CONFIG[loginType].typeName
end

function GameConf:getPayTypeName(payType)
    if payType == nil then
        payType = self.pay_type
    end
    return GameConf.PAY_CONFIG[payType].typeName
end

function GameConf:makeOrderData( payData )
    local payConfig = GameConf.PAY_CONFIG[self.pay_type]
    local orderData = {}
    orderData.id = payData.id
    orderData.goods = payData.id
    orderData.name = payData.name
    orderData.body = payData.unit.." "..payData.id.." x1"
    orderData.money = payData.money
    orderData.price = payData.money/100
    orderData.type = payConfig.typeName
    orderData.subject = payData.unit
    orderData.autobuy = 1
    orderData.ingame = 1
    orderData.roomid = 0
    orderData.count = 1
    orderData.debug = 0
    orderData.udid = Helper.GetDeviceCode()
    if self.pay_type == "apple" then
        orderData.productType = orderData.type
        local cfgTable = checktable(payConfig.payData)
        orderData.goods = cfgTable[payInfo.price]
    end
    return orderData
end

function GameConf:makeOrderResultData( data, orderData )
    local payInfo = checktable(data)
    table.merge(payInfo, orderData)
    if data.ext then
        table.merge(payInfo, data.ext)
        return payInfo
    end
    return payInfo
end

function GameConf:makeAndroidPayParams( payInfo, extendData )
    local javaParams = {}
    if not self.is_thirdpay_sdk then
        payInfo.virtual=checkint( payInfo.virtual) 
        local jsonArgs = json.encode(payInfo)
        local jsonCfg = json.encode(GameConf.PAY_CONFIG[self.pay_type].payData)

        javaParams = {
            payInfo.type,
            jsonArgs,
            jsonCfg
        }
    else
        local onThirdpaySdkFun = {
            baidu = GameConf.onThirdpayBaidu,
            huawei = GameConf.onThirdpayHuawei,
            jinli = GameConf.onThirdpayJinli,
            lenovo = GameConf.onThirdpayLenovo,
            mi = GameConf.onThirdpayMi,
            mz = GameConf.onThirdpayMz,
            oppo = GameConf.onThirdpayOppo,
            qihu = GameConf.onThirdpayQihu,
            samsung = GameConf.onThirdpaySamsung,
            vivo = GameConf.onThirdpayVivo,
            yyb = GameConf.onThirdpayYyb,
        }
        if onThirdpaySdkFun[self.pay_type] then
            payInfo = onThirdpaySdkFun[self.pay_type](self, payInfo, extendData)
            local jsonStr = json.encode(payInfo, extendData)
            javaParams = {
                jsonStr,
                extendData.handlerCallback
            }
        end
    end
    return javaParams
end

function GameConf:makeIosPayParams( payInfo, extendData )
    payInfo.listener = extendData.handlerCallback
	payInfo.money = tonumber(payInfo.money)/100
	local cfgTable = checktable(GameConf.PAY_CONFIG[self.pay_type].payData)

	if self.pay_type == "apple" then
		local productids = table.values(cfgTable)
		payInfo.productidarray = json.encode(productids)
		payInfo.productid = cfgTable[payInfo.money]
	else
		table.merge(payInfo, cfgTable)
	end
    return payInfo
end

function GameConf:makeAndroidPayResultData( data )
    local resultInfo = nil
    local statusCode = 0
    if not self.is_thirdpay_sdk then
        if type(data) == "string" then
            local ok = false
            ok,resultInfo = pcall(function()
                return loadstring(data)()
            end)
        else
            resultInfo = data
        end
        statusCode = resultInfo.status
    else
        resultInfo = json.decode(data)
        statusCode = resultInfo.resultCode
    end

    if statusCode == 0 then
        return GameConf.PAY_RESULT_STATUS.SUCCESS
    else
        return GameConf.PAY_RESULT_STATUS.FAILED
    end
end

function GameConf:makeIosPayResultData( status, paytype, msg )
    if paytype == "appstore" and status == 0 then
		self:verifyIosReceipt_(msg, paytype)
        local ok, args = pcall(function()
            return loadstring(luastr)()
        end)
        if ok then
            print("-----------------ok verify")

            FishGF.waitNetManager(true,FishGF.getChByIndex(800000186),"verifyIosReceipt_")
            SmallGamesGI.Dapi:VerifyIosReceipt(args, function(msg)
                FishGF.waitNetManager(false,FishGF.getChByIndex(800000186),"verifyIosReceipt_")
                if msg.status == 0 then
                    local retTab = { status = msg.status, paytype = paytype, msg = "支付成功！" }
                    self:makeAndroidPayResultData(retTab)
                else
                    print("iap order verify failure")
                end
    
            end)
        else
            printf("解析ios 参数失败 %s", luastr)
        end
	else
		local retTab = {
			status = status,
			paytype = paytype,
			msg = msg,
		}
        self:makeAndroidPayResultData(retTab)
	end
end

function GameConf:getShareData()
    return {
        wechat = {
            android = {
                className = "com.weile.api.WXShareHelper",
                methodName = "doShareToWX",
                methodSig = "(Ljava/lang/String;)V",
                callbackResult = "addScriptListener",
            },
            ios = {
                className = "AppController",
                methodName = "doShareTo",
            },
        },
    }
end

local function makeWechatShareInfo( extendData )
    local shareInfo = {}
    shareInfo.appid = extendData.appid
    shareInfo.wxscene = extendData.wxscene
    shareInfo.imgpath = extendData.imgpath
    if extendData.sharetype == GameConf.SHARE_TYPE.Image then
        shareInfo.sharetype="image"
    end
    if extendData.handlerCallback then
        shareInfo.listener = extendData.handlerCallback
    end
    return shareInfo
end

function GameConf:makeAndroidShareParams( extendData )
    local javaParams = {}
    if self.share_type == "wechat" then
        local shareInfo = makeWechatShareInfo(extendData)
        local jsonArgs = json.encode(shareInfo)
        javaParams = {
            jsonArgs
        }
    end
    return javaParams
end

function GameConf:makeIosShareParams( extendData )
    local shareInfo = {}
    if self.share_type == "wechat" then
        shareInfo = makeWechatShareInfo(extendData)
    end
    return shareInfo
end

local function makeWechatShareResult( data )
    local resultStatus = GameConf.SHARE_RESULT_STATUS.FAILED
    local ok,argtable = pcall(function()
		return loadstring(data)()
	end)
	if ok then
        if argtable.status == 0 then
            resultStatus = GameConf.SHARE_RESULT_STATUS.SUCCESS
        end
    else
        if data == 0 then
            --ios平台
            resultStatus = GameConf.SHARE_RESULT_STATUS.SUCCESS
        end
    end
    return resultStatus
end

function GameConf:makeAndroidShareResultData( data )
    local resultStatus = GameConf.SHARE_RESULT_STATUS.FAILED
    if self.share_type == "wechat" then
        resultStatus = makeWechatShareResult(extendData)
    end
    return resultStatus
end

function GameConf:makeIosShareResultData( data )
    local resultStatus = GameConf.SHARE_RESULT_STATUS.FAILED
    if self.share_type == "wechat" then
        resultStatus = makeWechatShareResult(extendData)
    end
    return resultStatus
end

--------------------------------------------------------------------------------
-------------------------------自定义接口----------------------------------------
--------------------------------------------------------------------------------

function GameConf:getThirdpayCallBackUrl(url)
    return string.format(url, GameConf.PAY_CONFIG[self.pay_type].typeName)
end

function GameConf:onThirdpayBaidu(payArgs, extendData)
	local pointArr = {
        ["830000001"] = "28035",
        ["830000002"] = "28036",
        ["830000003"] = "28037",
        ["830000004"] = "28038",
        ["830000005"] = "28039",
        ["830000006"] = "28040",
        ["830000007"] = "28041",
        ["830000008"] = "28042",
        ["830000009"] = "28043",
        ["830000010"] = "28044",
        ["830000011"] = "28045",
        ["830000012"] = "28046",
        ["830000013"] = "28047",
        ["830000014"] = "28048",
        ["830000015"] = "28049",
    }
	local payInfo = {}
    payInfo.order = payArgs.orderid
    payInfo.mPropsId = pointArr[tostring(payArgs.id)]
    payInfo.productName = payArgs.name
    payInfo.price = payArgs.money/100
	return payInfo
end

function GameConf:onThirdpayHuawei(payArgs, extendData)
	local payInfo = {}
    payInfo.orderid = payArgs.orderid
    payInfo.money = payArgs.money
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
    payInfo.productname = payArgs.subject
    payInfo.productid   = payArgs.goods
    payInfo.username    = SmallGamesGI.hallNet.userinfo.nick
    payInfo.userid      = SmallGamesGI.hallNet.userinfo.id
	return payInfo
end

function GameConf:onThirdpayJinli(payArgs, extendData)
	local payInfo = {}
    payInfo.orderid     = payArgs.orderid
    --payInfo.money       = payTab.money/100
    payInfo.money       = tonumber(payArgs.money)/100
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
    payInfo.productname = payArgs.subject
    payInfo.productid   = payArgs.goods
    payInfo.username    = SmallGamesGI.hallNet.userinfo.nick
    payInfo.userid      = SmallGamesGI.hallNet.userinfo.id
	return payInfo
end

function GameConf:onThirdpayLenovo(payArgs, extendData)
	local tWaresidList = {}
    tWaresidList["830000001"] = 148920
    tWaresidList["830000002"] = 148198
    tWaresidList["830000003"] = 148199
    tWaresidList["830000004"] = 148200
    tWaresidList["830000005"] = 148201
    tWaresidList["830000006"] = 148202
    tWaresidList["830000007"] = 148203
    tWaresidList["830000008"] = 148204
    tWaresidList["830000009"] = 148205
    tWaresidList["830000010"] = 148206
    tWaresidList["830000011"] = 148207
    tWaresidList["830000012"] = 148208
    tWaresidList["830000013"] = 148209
    tWaresidList["830000014"] = 148210
    tWaresidList["830000015"] = 148211
    local payInfo = {}
    payInfo.waresid = tWaresidList[tostring(payArgs.goods)]
    payInfo.exorderno = payArgs.orderid
    payInfo.price = payArgs.money/100
    payInfo.notifyurl = ""
	return payInfo
end

function GameConf:onThirdpayMi(payArgs, extendData)
	 local payInfo = {}
    payInfo.order = payArgs.orderid
    payInfo.amount = payArgs.money;
    payInfo.productName = payArgs.subject
    payInfo.productDesc = payArgs.body
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
	return payInfo
end

function GameConf:onThirdpayMz(payArgs, extendData)
    local payInfo = {}
    payInfo.order = payArgs.orderid
    payInfo.money = payArgs.money/100
    payInfo.subject = payArgs.subject
    payInfo.goods = payArgs.goods
    return payInfo
end

function GameConf:onThirdpayOppo(payArgs, extendData)
	local payInfo = {}
    payInfo.order = payArgs.orderid
    payInfo.amount = payArgs.money
    payInfo.productName = payArgs.subject
    payInfo.productDesc = payArgs.body
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
	return payInfo
end

function GameConf:onThirdpayQihu(payArgs, extendData)
	local payInfo = {}
    payInfo.orderid = payArgs.orderid
    payInfo.money = payArgs.money
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
    payInfo.productname = payArgs.subject
    payInfo.productid   = payArgs.goods
	payInfo.username    = SmallGamesGI.hallNet.userinfo.nick
    payInfo.userid      = SmallGamesGI.hallNet.userinfo.id
	return payInfo
end

function GameConf:onThirdpaySamsung(payArgs, extendData)
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
    payInfo.userid  = SmallGamesGI.hallNet.userinfo.id
    payInfo.order   = payArgs.orderid
    payInfo.waresid = tWaresidList[tostring(payArgs.goods)]
    payInfo.price   = payArgs.money
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
	return payInfo
end

function GameConf:onThirdpayVivo(payArgs, extendData)
	local payInfo = {}
    payInfo.order = payArgs.ext.vivoOrder
    payInfo.amount = payArgs.ext.orderAmount
    payInfo.signature = payArgs.ext.vivoSignature
    payInfo.productName = payArgs.subject
    payInfo.productDesc = payArgs.body
	return payInfo
end

function GameConf:onThirdpayYyb(payArgs, extendData)
	local payInfo = {}
    payInfo.orderid = payArgs.orderid
    payInfo.money = payArgs.money
    payInfo.name = payArgs.subject
    payInfo.productDesc = payArgs.body
    payInfo.goods = payArgs.goods
    payInfo.callbackurl = self:getThirdpayCallBackUrl(extendData.thirdpayCallbackUrl)
	return payInfo
end

return GameConf