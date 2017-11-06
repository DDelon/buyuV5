local CommonLayer = class("CommonLayer",nil)

function CommonLayer.create()
    local data = CommonLayer.new();
    data:init();
    return data;
end

--0.登录   1.大厅   2.普通场   3.朋友场
CommonLayer.layerList  = {  
    { ["layerName"] = "uiShopLayer",    ["order"] = FishCD.ORDER_LAYER_TRUE,        ["isAutoHide"] = 0,["noShowScene"] = {0},["isAddToScene"] = 0}, 
    { ["layerName"] = "uiVipRight",     ["order"] = FishCD.ORDER_LAYER_TRUE,        ["isAutoHide"] = 0,["noShowScene"] = {0},["isAddToScene"] = 0}, 
    { ["layerName"] = "uiMonthcard",    ["order"] = FishCD.ORDER_LAYER_TRUE,        ["isAutoHide"] = 0,["noShowScene"] = {0},["isAddToScene"] = 0}, 
    { ["layerName"] = "uiNoticeLayer",  ["order"] = FishCD.ORDER_SYSTEM_MESSAGE,    ["isAutoHide"] = 0,["noShowScene"] = {},["isAddToScene"] = 1}, 
    { ["layerName"] = "uiExitNotice",   ["order"] = FishCD.ORDER_SYSTEM_MESSAGE+1,  ["isAutoHide"] = 1,["noShowScene"] = {},["isAddToScene"] = 1}, 
}

function CommonLayer:init()
    self.scaleX_,self.scaleY_,self.scaleMin_  = FishGF.getCurScale()
end

function CommonLayer:initComLayer()
    
    --商店
    if self.uiShopLayer == nil then
        self.uiShopLayer = require("Shop/Shop").create()
        self.uiShopLayer:setPosition(cc.p(cc.Director:getInstance():getWinSize().width/2,cc.Director:getInstance():getWinSize().height/2))
        self.uiShopLayer:retain()
    end

    --VIP特权
    if self.uiVipRight == nil then
        self.uiVipRight = require("VipRight/VipRight").create()
        self.uiVipRight:setPosition(cc.p(cc.Director:getInstance():getWinSize().width/2,cc.Director:getInstance():getWinSize().height/2))
        self.uiVipRight:retain()
    end

    --月卡
    if self.uiMonthcard == nil then
        self.uiMonthcard = require("hall/Monthcard/Monthcard").create()
        self.uiMonthcard:setPosition(cc.p(cc.Director:getInstance():getWinSize().width/2,330*self.scaleY_))
        self.uiMonthcard:retain()
    end


    self:initNotice()
    for k,v in pairs(self.layerList) do
        self[v.layerName]:setScale(self.scaleMin_)
        self[v.layerName]:hideLayer(false)
    end 

end

function CommonLayer:initNotice()
    --提示框
    if self.uiNoticeLayer == nil then
        self.uiNoticeLayer = require("Message/MessageDialog").create()
        self.uiNoticeLayer:setPosition(cc.p(cc.Director:getInstance():getWinSize().width/2,cc.Director:getInstance():getWinSize().height/2))  
        self.uiNoticeLayer:setName("uiNoticeLayer")
        self.uiNoticeLayer:retain()
        self.uiNoticeLayer:setScale(self.scaleMin_)
        self.uiNoticeLayer:hideLayer(false)
    end

    --退出提示框
    if self.uiExitNotice == nil then
        self.uiExitNotice = require("Message/MessageDialog").create()
        self.uiExitNotice:setPosition(cc.p(cc.Director:getInstance():getWinSize().width/2,cc.Director:getInstance():getWinSize().height/2))
        self.uiExitNotice:setName("uiExitNotice")
        self.uiExitNotice:retain()
        self.uiExitNotice:setScale(self.scaleMin_)
        self.uiExitNotice:hideLayer(false)
    end

end

function CommonLayer:getComLayer(layerName)
    if self[layerName] == nil then
        self:initComLayer()
    end
    if self[layerName] == nil then
        print("----------layer is No exit----------------")
    end    
    return self[layerName]
end

function CommonLayer:getAllComLayer()
    local result = {}
    for k,v in pairs(self.layerList) do
        if self[v] == nil then
            self:initComLayer()
        end
        result[v] = self[v]
    end
    return result
end

function CommonLayer:addOneLayerToParent(layerName,parent,parent2,order,isAdd,isAutoHide,isShow,isAddToScene)
    print("CommonLayer:addOneLayerToParent----------------------------------------------------------------")
    if isAdd == false then
        return
    end
    if self[layerName] == nil then
        return
    end
    if self[layerName]:getParent() ~= nil then
        self[layerName]:removeFromParent()
    end        
    parent[layerName] = self[layerName]
    if parent2 ~= nil then
        parent2[layerName] = self[layerName]
    end
    if isAddToScene == 0 then
        parent:addChild(self[layerName],order)
    else
        parent2:addChild(self[layerName],order)
    end
    
    if (not isShow) or isAutoHide == 1 then
        self[layerName]:hideLayer(false)
    end
    
end

function CommonLayer:addLayerToParent(parent,parent2)
    print("CommonLayer:initComLayer----------------------------------------------------------------")

    local curScene = cc.Director:getInstance():getRunningScene();
    if parent2 == nil then
        parent2 = curScene
    end
    local sceneName = curScene.sceneName
    local scenetype = 0
    if sceneName == "login" or sceneName == "UpDateScene" then
        scenetype = 0
    elseif sceneName == "hall" then
        scenetype = 1
    elseif sceneName == "game" then
        scenetype = 2
    end
    if scenetype == 0 then
        self:initNotice()
    else
        self:initComLayer()
    end
    for k,v in pairs(self.layerList) do
        local isAdd = true
        for k2,v2 in pairs(v.noShowScene) do
            if v2 == scenetype then
                isAdd = false
            end
        end
        local isShow = true
        if scenetype == 0 then
            isShow = false
        end
        self:addOneLayerToParent(v.layerName,parent,parent2,v.order,isAdd,v.isAutoHide,isShow,v.isAddToScene)
    end 
end

return CommonLayer