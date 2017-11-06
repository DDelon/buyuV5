
local UnlockCannon = class("UnlockCannon", cc.load("mvc").ViewBase)

UnlockCannon.AUTO_RESOLUTION   = false
UnlockCannon.RESOURCE_FILENAME = "ui/battle/unlockcannon/uiunlockcannon"
UnlockCannon.RESOURCE_BINDING  = {  
    ["panel"]                   = { ["varname"] = "panel" },
    ["btn_close"]               = { ["varname"] = "btn_close" , ["events"]={["event"]="click",["method"]="onClickClose"}},   
    ["spr_zgpb"]                = { ["varname"] = "spr_zgpb" },
    ["node_item"]               = { ["varname"] = "node_item" },
    ["node_item_1"]             = { ["varname"] = "node_item_1" },
    ["node_item_2"]             = { ["varname"] = "node_item_2" },
    ["node_item_3"]             = { ["varname"] = "node_item_3" },
    ["node_item_4"]             = { ["varname"] = "node_item_4" },
    ["node_item_5"]             = { ["varname"] = "node_item_5" },
    ["spr_effect_unlock"]       = { ["varname"] = "spr_effect_unlock" },
}

function UnlockCannon:onCreate( ... )
    --初始化
    self:init()
    -- 初始化View
    self:initView()
end

function UnlockCannon:init()
    self.node_item_list = {}
    for i=1,5 do
        self.node_item_list[i] = require("Game/UnlockCannon/UnlockCannonItem").new(self, self["node_item_"..i])
        self.node_item_list[i]:setTag(i)
    end
end

function UnlockCannon:initView()
    FishGI.eventDispatcher:registerCustomListener("myGunUpData", self, function(valTab) self:onGunUpData(valTab) end)
    FishGI.eventDispatcher:registerCustomListener("GunUpgrade", self, function(valTab) self:onGunUpgrade(valTab) end)

    self:openTouchEventListener()
    self.panel:setSwallowTouches(false)
    self:runAction(self.resourceNode_["animation"])
end

function UnlockCannon:onTouchBegan(touch, event)
    if self:isVisible() then
         return true  
    end

    return false
end

function UnlockCannon:onClickClose( sender )
    self:hideLayer()
end

function UnlockCannon:showLayer(isAct)
    self.super.showLayer(self,isAct)
    self.resourceNode_["animation"]:play("unlock_prompt", true)
    self:upDataList()
end

function UnlockCannon:hideLayer()
    self.super.hideLayer(self)
    self.resourceNode_["animation"]:pause()
end

function UnlockCannon:buttonClicked(viewTag, btnTag)
    if self.node_item_list[btnTag] then
        if self.curCrystal >= self.node_item_list[btnTag]:getAimCrystal() then
            FishGI.gameScene.net:sendUpgradeCannon()
        else
            FishGI.gameScene.uiShopLayer:showLayer()
            FishGI.gameScene.uiShopLayer:setShopType(2)
        end
    end
end

--数据变动
function UnlockCannon:onGunUpData(valTab)
    local newCrystal = valTab.newCrystal
    if newCrystal == nil or newCrystal < 0 then
        return
    end
    self:setCurCrystal(newCrystal)
end

--炮倍解锁
function UnlockCannon:onGunUpgrade(valTab)
    if valTab.nextRate <= 1000 then
        self:setCurCrystal(valTab.newCrystal)
        self.spr_zgpb:setVisible(false)
        self.node_item:setVisible(true)
        self:upDataList()
    else
        self.resourceNode_["animation"]:pause()
        self.spr_effect_unlock:setVisible(false)
        self.spr_zgpb:setVisible(true)
        self.node_item:setVisible(false)
    end
end

function UnlockCannon:setCurCrystal(curCrystal)
    self.curCrystal = tonumber(curCrystal)
end

function UnlockCannon:setCurGunType(playerVIP)
    self.playerVIP = playerVIP
    for k,v in pairs(self.node_item_list) do
        v:setVip(self.playerVIP)
    end
end

function UnlockCannon:upDataList()
    local maxGunRate = FishGI.gameScene.playerManager:getMyData().playerInfo.maxGunRate
    if maxGunRate >= 1000 then
        self.resourceNode_["animation"]:pause()
        self.spr_effect_unlock:setVisible(false)
        self.spr_zgpb:setVisible(true)
        self.node_item:setVisible(false)
        return
    end
    self.spr_zgpb:setVisible(false)
    self.node_item:setVisible(true)
    local gunsData = FishGI.GameTableData:getCannonRangeTable(maxGunRate, 5, 1000)
    local curGunRateIndex = 0
    for i=1,5 do
        self.node_item_list[i]:setMaxGunRate(maxGunRate)
        if gunsData[i].times == maxGunRate then
            curGunRateIndex = i
        end
    end
    if gunsData[curGunRateIndex-1] then
        self.node_item_list[1]:setItemData(gunsData[curGunRateIndex-1].times, gunsData[curGunRateIndex-1].unlock_gem, gunsData[curGunRateIndex-1].unlock_award, false)
        self.node_item_list[1]:setVisible(true)
    else
        self.node_item_list[1]:setVisible(false)
    end
    self.node_item_list[2]:setItemData(gunsData[curGunRateIndex].times, gunsData[curGunRateIndex].unlock_gem, gunsData[curGunRateIndex].unlock_award, false)
    if gunsData[curGunRateIndex+1] then
        self.node_item_list[3]:setItemData(gunsData[curGunRateIndex+1].times, gunsData[curGunRateIndex+1].unlock_gem, gunsData[curGunRateIndex+1].unlock_award, true)
        self.node_item_list[3]:setVisible(true)
    else
        self.node_item_list[3]:setVisible(false)
    end
    if gunsData[curGunRateIndex+2] then
        self.node_item_list[4]:setItemData(gunsData[curGunRateIndex+2].times, gunsData[curGunRateIndex+2].unlock_gem, gunsData[curGunRateIndex+2].unlock_award, false)
        self.node_item_list[4]:setVisible(true)
    else
        self.node_item_list[4]:setVisible(false)
    end
    if gunsData[curGunRateIndex+3] then
        self.node_item_list[5]:setItemData(gunsData[curGunRateIndex+3].times, gunsData[curGunRateIndex+3].unlock_gem, gunsData[curGunRateIndex+3].unlock_award, false)
        self.node_item_list[5]:setVisible(true)
    else
        self.node_item_list[5]:setVisible(false)
    end
end

return UnlockCannon