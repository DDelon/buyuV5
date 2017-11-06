
local RealName = class("RealName", cc.load("mvc").ViewBase)

RealName.AUTO_RESOLUTION   = false
RealName.RESOURCE_FILENAME = "ui/hall/realname/uirealname"
RealName.RESOURCE_BINDING  = {  
    ["panel"]       = { ["varname"] = "panel" },
    ["btn_close"]   = { ["varname"] = "btn_close" , ["events"]={["event"]="click",["method"]="onClickClose"}},   
    ["bmf_num"]     = { ["varname"] = "bmf_num" },
    ["tf_name"]     = { ["varname"] = "tf_name" },
    ["tf_idcard"]   = { ["varname"] = "tf_idcard" },
    ["btn_ok"]      = { ["varname"] = "btn_ok", ["events"]={["event"]="click",["method"]="onClickOk"} }, 
    ["text_tip"]    = { ["varname"] = "text_tip" },
}

function RealName:onCreate( ... )
    --初始化
    self:init()
    -- 初始化View
    self:initView()
end

function RealName:init()
end

function RealName:initEditBoxStr(str)
    self.tf_name:setString(str);
    self.tf_idcard:setString(str);
end

function RealName:initView()
    self:openTouchEventListener()
    self:initWinEditBox("tf_name")
    self:initWinEditBox("tf_idcard")

    self.tf_name:setNewPlaceHolder(FishGF.getChByIndex(800000340))
    self.tf_idcard:setNewPlaceHolder(FishGF.getChByIndex(800000341))
    self.text_tip:setString(FishGF.getChByIndex(800000342))

    local dataStr = FishGI.GameConfig:getConfigData("config",tostring(990000102), "data")
    self.presentCount = tonumber(string.split(dataStr,",")[2])
    self.bmf_num:setString(tostring(self.presentCount))
end

function RealName:onTouchBegan(touch, event)
    if not self:isVisible() then
         return false
    end
    return true
end

function RealName:onClickClose( sender )
    self:hideLayer()
end

function RealName:onClickOk( sender )
    self:onClickCommit()
end

function RealName:onClickCommit()
    local strName = self.tf_name:getString()
    local strIdcard = self.tf_idcard:getString()
    if #strName == 0 then
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000343))
        return
    end
    if #strIdcard == 0 then
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000344))
        return
    end
    local function callback(data)
        local strMsg = data.msg
        if data.status == 0 then
            --更新数据
            FishGMF.addTrueAndFlyProp(FishGI.myData.playerId,FishCD.PROP_TAG_02,self.presentCount,false)
            FishGMF.setAddFlyProp(FishGI.myData.playerId,FishCD.PROP_TAG_02,self.presentCount,false)
            local propTab = {}
            propTab.playerId = FishGI.myData.playerId
            propTab.propId = FishCD.PROP_TAG_02
            propTab.propCount = self.presentCount
            propTab.isRefreshData = true
            propTab.isJump = false
            propTab.firstPos = cc.p(display.width/2, display.height/2)
            propTab.dropType = "normal"
            propTab.isShowCount = false
            FishGI.GameEffect:playDropProp(propTab)
            self:hideLayer()
            if self.funCallback then
                self.funCallback()
            end
            strMsg = FishGF.getChByIndex(800000354)
        end
        FishGF.waitNetManager(false, nil, "RealNameVerify")
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,strMsg)
    end
    FishGF.waitNetManager(true, nil, "RealNameVerify")
    FishGI.Dapi:RealNameVerify(strName, strIdcard, callback)
end

function RealName:setCallback( funCallback )
    self.funCallback = funCallback
end

return RealName