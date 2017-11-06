local TIME_VAL = 9999999999
local SkillBase = import("Game.Skill.NormalSkill.SkillBase")
local SkillViolnet = class("SkillViolnet",SkillBase)

function SkillViolnet:ctor(...)
    self.duration = tonumber(FishGI.GameConfig:getConfigData("skill","960000006","duration"))
    self:initListener()
    self:initBg()
    self:openTouchEventListener(false)
    self.isChose = false
    self.endTime = TIME_VAL
    
    --self.lockUI =  require("Game/Skill/NormalSkill/SkillUI/LockViolentUI").create(self);

    local dataStr = tostring(FishGI.GameConfig:getConfigData("config",tostring(990000101), "data"));
    local list = string.split(dataStr,";")
    self.rateDataList = {}
    self.needNoticeRate = {}
    for k,v in pairs(list) do
        local valTab = string.split(v,",")
        local data = {}
        data.rate = tonumber(valTab[1])
        data.propCount = tonumber(valTab[2])
        table.insert( self.rateDataList, data )

        local index = FishGI.GameTableData:getCannonIndex(data.rate)
        local priorRate = FishGI.GameTableData:getCannonTable(index - 1).times
        self.needNoticeRate[priorRate] = 1
    end

end

--初始化监听器
function SkillViolnet:initListener()
    --FishGI.eventDispatcher:registerCustomListener("UseViolentResult", self, function(valTab) self:useViolentResult(valTab) end);
    --FishGI.eventDispatcher:registerCustomListener("ViolentTimeOut", self, function(valTab) self:ViolentTimeOut(valTab) end);
    --FishGI.eventDispatcher:registerCustomListener("bulletTargetChange", self, function(valTab) self:ViolentTimeOut(valTab) end);
    local useViolentResultListener=cc.EventListenerCustom:create("UseViolentResult",handler(self, self.useViolentResult))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(useViolentResultListener, 2)
    
    local violentTimeOutListener=cc.EventListenerCustom:create("ViolentTimeOut",handler(self, self.violentTimeOut))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(violentTimeOutListener, 2)
    
    local changeTargetListener=cc.EventListenerCustom:create("bulletTargetChange",handler(self, self.bulletTargetChange))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(changeTargetListener, 2)

    local sendChangeAimFishListener=cc.EventListenerCustom:create("sendChangeAimFish",handler(self, self.sendChangeAimFish))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(sendChangeAimFishListener, 2)
        
end

--初始化精灵
function SkillViolnet:initBg()

end

function SkillViolnet:initLock()
    --LuaCppAdapter:getInstance():setLuaNode(1,self,{});
    self.lockUI = require("Game/Skill/NormalSkill/SkillUI/LockViolentUI").create(self);
end

function SkillViolnet:setPropId(propId)
    self.propId = propId
    
end


function SkillViolnet:onTouchBegan(touch, event) 
    --判断点击的是否是其他的技能按钮 如果是就不执行换锁定目标的功能
    local isTouchBtn = self:getParent():getParent():isTouchBtn(touch)
    if isTouchBtn then
        return true
    end

    local curPos = touch:getLocation()
    if self.lockFunc ~= nil then
        self.lockFunc:changeLockTarget(curPos);
        --FishGF.print("lock func send message change target")
    else
        --print("lock func is nil")
    end
    return true

end

--按键按下的处理
function SkillViolnet:clickCallBack( )
    local isCanUse = self:isCanUse()
    if isCanUse ~= true then
        return
    end

    local useType = self:judgeUseType()
    if useType == nil then
        return
    end
    self:pushDataToPool(useType)
    self.useType = useType
    local data = {}
    data.useType = useType
    self:sendNetMessage(data)
    self.btn:setTouchEnabled(false)
    
    self.endTime = os.time()+self.duration
    --[[if self.lockFunc == nil then
        self.lockFunc =  require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
        self.lockUI:rebind(self);
        FishGF.print("violent lock func created");
    end]]--
    --self.lockFunc:start(FishGI.gameScene.playerManager.selfIndex);
end

function SkillViolnet:isCanUse()
    if self.btn.parentClasss:getFntCount() > 0 then
        return true
    end
    self.playerSelf = FishGI.gameScene.playerManager:getMyData()
    local playerInfo = self.playerSelf.playerInfo;
    --提示炮倍不足
    local maxGunRate = playerInfo.maxGunRate;
    if maxGunRate < 200 then
        local function callback(sender)
            local tag = sender:getTag()
            if tag == 2 then
                FishGI.gameScene.uiUnlockCannon:showLayer()
            end
        end
        local str = FishGF.getChByIndex(800000348)
        if maxGunRate < 100 then
            str = FishGF.getChByIndex(800000347)
        end
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_CLOSE,str,callback);
        return false
    end

    return true
end

function SkillViolnet:checkIsEnd()
    
    local curTime = os.time();
    
    if curTime >= self.endTime then
        local playerId = FishGI.gameScene.playerManager.selfIndex;
        local player = FishGI.gameScene.playerManager:getPlayerByPlayerId(playerId)
        player:endEffectId();
        self.endTime = TIME_VAL;
        self.btn.parentClasss:setState(1)
        if self.lockFunc ~= nil then
            FishGF.print("violent lock func over on checkIsEnd")
            self.lockFunc:over(playerId);
            self.lockFunc = nil;
        end
    end
end

function SkillViolnet:sendChangeAimFish(data)
    data = data._userdata
    if self.lockFunc ~= nil then
        self.lockFunc:sendChangeTarget(data.timelineId, data.fishArrayId)
    end
end

function SkillViolnet:useViolentResult(evt)
    local data = evt._userdata;
    local playerId = data.playerID;
    local useType = data.useType;
    local newCrystal = data.newCrystal;
    if data.isSuccess then
        
        self.playerSelf = FishGI.gameScene.playerManager:getMyData()
        local myPlayerId = self.playerSelf.playerInfo.playerId

        local isShow = nil
        if myPlayerId == playerId then
            if self.lockFunc == nil then
                self.lockFunc =  require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
                self.lockUI:rebind(self);
                FishGF.print("violent lock func created");
            end

            self.endTime = os.time()+self.duration
            self:runTimer()
            isShow = false
            self.btn.parentClasss:setState(2)
        end

        if useType == 1 then
            --更新水晶
            FishGMF.upDataByPropId(playerId,FishCD.PROP_TAG_02,newCrystal,isShow)
        elseif useType == 0 then
            FishGMF.addTrueAndFlyProp(playerId,FishCD.PROP_TAG_20,-1,isShow)
        end

        if myPlayerId == playerId then
            self:clearDataFromPool(useType)
        end


        local player = FishGI.gameScene.playerManager:getPlayerByPlayerId(playerId)
        player:startEffectId(self.propId);
        if myPlayerId == playerId then
            if self.lockFunc ~= nil then
                self.lockFunc:start(playerId);
                FishGF.print("violent lock func start")
            end
        else
            local lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
            lockFunc:lockFish(playerId, timelineId, fishArrayId);
            lockFunc:over(playerId);
            lockFunc = nil;
        end
    end
end

function SkillViolnet:violentTimeOut(evt)
    local data = evt._userdata;
    local playerId = data.playerId;
    local player = FishGI.gameScene.playerManager:getPlayerByPlayerId(playerId)
    player:endEffectId();
    
    if playerId == FishGI.gameScene.playerManager.selfIndex then
        if self.lockFunc ~= nil then
            FishGF.print("violent lock func over on violentTimeOut")
            self.lockFunc:over(playerId);
            self.lockFunc = nil;
        end
        self.endTime = TIME_VAL;
        self.btn.parentClasss:setState(1);
    end
    
end

function SkillViolnet:bulletTargetChange(evt)
    local data = evt._userdata;
    local selfId = FishGI.gameScene.playerManager.selfIndex;
    if data.playerId ~= selfId then
        local lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
        lockFunc:lockFish(data.playerId, data.timelineId, data.fishArrayId);
        lockFunc:over(data.playerId);
        lockFunc = nil;
    else
        if self.lockFunc ~= nil then
            self.lockFunc:lockFish(data.playerId,data.timelineId,data.fishArrayId)
        end
    end 
end

--进入前台刷新时间
function SkillViolnet:upDateUserTime(disTime )
    self:upDateTimer()
end

--判断是否能正常解锁
function SkillViolnet:ifNoticeRate( )
    self.playerSelf = FishGI.gameScene.playerManager:getMyData()
    local playerInfo = self.playerSelf.playerInfo;
    --提示炮倍不足
    local maxGunRate = playerInfo.maxGunRate;
    if self.needNoticeRate[maxGunRate] == 1 then
        local count = FishGMF.getPlayerPropData(playerInfo.playerId,17,nil).realCount
        --local count = self.btn.parentClasss:getFntCount()
        if count > 0  then
            return true
        end
    end

    return false    
end

function SkillViolnet:setMaxRate( maxGunRate )
    self.maxGunRate = maxGunRate
    self:updateBtnIfCanUsed()
end

function SkillViolnet:GunUpgrade( maxGunRate )
    self:setMaxRate(maxGunRate)
    if maxGunRate == 800 then
        self.isUpgrade = false
        self.upDate = nil
        self.btn.parentClasss:playBtnUpAct(3)
    end
    for k,v in pairs(self.rateDataList) do
        if maxGunRate == v.rate then
            self.isUpgrade = true
            self.upDate = v
        end
    end


end

function SkillViolnet:updateBtnIfCanUsed( )
    if self.maxGunRate == nil then
        self.maxGunRate = 0
    end
    local maxGunRate = self.maxGunRate
    local propCount = self.btn.parentClasss:getFntCount()
    if propCount <= 0 and maxGunRate < 800 then
        self.btn.parentClasss:setBtnIfCanUsed(false)
    else
        self.btn.parentClasss:setBtnIfCanUsed(true)
    end
end

function SkillViolnet:setSkillByTag( val )
    self.super.setSkillByTag(self,val)
    print("------------------SkillViolnet:setSkillByTag---------------------")
    if self.isUpgrade == true and self.upDate ~= nil then
        if val.propCount == self.upDate.propCount then
            self.isUpgrade = false
            self.btn.parentClasss:playBtnUpAct(2)
        end
    end
    self:updateBtnIfCanUsed()
end

return SkillViolnet;