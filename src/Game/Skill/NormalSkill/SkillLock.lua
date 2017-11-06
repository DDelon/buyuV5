






---------------------------------------------------------------

local SkillBase = import("Game.Skill.NormalSkill.SkillBase")
local SkillLock = class("SkillLock",SkillBase)

SkillLock.touchDisTime  = 0.5  --手动切换目标间隔
SkillLock.chainCount  = 8  --锁链点个数
function SkillLock:ctor(...)
    self:registerListener()
    self:openTouchEventListener(false)
    self:initData();
    
end

--初始化监听器
function SkillLock:registerListener()
    local startLockListener=cc.EventListenerCustom:create("startMyLock",handler(self, self.startMyLock))
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(startLockListener, 1)

    local startOtherLockListener = cc.EventListenerCustom:create("startOtherLock",handler(self, self.startOtherLock))
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(startOtherLockListener, 1)

    local changeTargetListener=cc.EventListenerCustom:create("bulletTargetChange",handler(self, self.bulletTargetChange))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(changeTargetListener, 1)

    local sendChangeAimFishListener=cc.EventListenerCustom:create("sendChangeAimFish",handler(self, self.sendChangeAimFish))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(sendChangeAimFishListener, 1)


end

function SkillLock:initData()
    FishGI.isLock = false
    print("skill lock init data");
    
end

--初始化锁定
function SkillLock:initLock()
    self.aimFish = nil
    self.playerSelf = nil
    self.startTime = 0
    self.touchStartTime = 0
    LuaCppAdapter:getInstance():setLuaNode(1,self,{});
    self.lockUI = require("Game/Skill/NormalSkill/SkillUI/LockCommonUI").create(self);
end

--结束锁定
function SkillLock:endLock()
    print("over lock")
    --结束锁定标志量
    if self.lockFunc ~= nil then
        self.lockFunc:over(FishGI.gameScene.playerManager.selfIndex)
        self.lockFunc = nil
    else
        print("--------endLock------self.lockFunc = nil-----------")
    end
    self.touchStartTime = 0
    self.btn.parentClasss:setState(1)

end

function SkillLock:onTouchBegan(touch, event) 
    if FishGI.isLock == true then
        if self.touchStartTime ~= 0 and (os.time() -self.touchStartTime < self.touchDisTime) then
            return
        end
        self.touchStartTime = os.time()
        --判断点击的是否是其他的技能按钮 如果是就不执行换锁定目标的功能
        local isTouchBtn = self:getParent():getParent():isTouchBtn(touch)
        if isTouchBtn then
            return true
        end

        
        
        local curPos = touch:getLocation()
        if self.lockFunc ~= nil then
            self.lockFunc:changeLockTarget(curPos);
        end
        return true
    end

    return false

end

function SkillLock:onTouchCancelled(touch, event) 

end

--按键按下的处理
function SkillLock:clickCallBack( )
    local useType = self:judgeUseType()
    if useType == nil then
        return
    end

    --判断是不是正在使用狂暴
    local isUseViolent = (FishGI.gameScene.playerManager:getPlayerByPlayerId(FishGI.gameScene.playerManager.selfIndex):getEffectId()==FishCD.SKILL_TAG_VIOLENT)
    if isUseViolent then
        --FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000350),nil)
        FishGF.showSystemTip(nil,800000350,1);
        return;
    end

    if self.lockFunc ~= nil then
        self.lockFunc:over(FishGI.gameScene.playerManager.selfIndex)
        self.lockFunc = nil;
    end

    local lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
    local timelineId, fishArrayId = lockFunc:getLockFishByScore();
    if timelineId == nil then
        return;
    end
    self:pushDataToPool(useType)
    self.useType = useType

    
    lockFunc:sendStartLock(useType);
    lockFunc:over(FishGI.gameScene.playerManager.selfIndex)
    lockFunc = nil;

    self:runTimer()
    self.btn:setTouchEnabled(false)
	
	if FishGI.curGameRoomID == 1 then
		local dataTab = {}
		dataTab.funName = "showLockPointUI"
		LuaCppAdapter:getInstance():luaUseCppFun(dataTab);
	end
end

--开始我的锁定
function SkillLock:startMyLock(evt)
    local valTab = evt._userdata;
    self.scaleX_,self.scaleY_,self.scaleMin_  = FishGF.getCurScale()
    self.playerSelf = FishGI.gameScene.playerManager:getMyData()
    local myPlayerId = self.playerSelf.playerInfo.playerId
    local useType = valTab.useType
    local newCrystal = valTab.newCrystal
    local isSuccess = valTab.isSuccess
    local skillPlus = valTab.skillPlus

    --技能持续时间
    self.lock_userTime = self.super.getSkillData(self, 4,"duration")
    if skillPlus ~= nil then
        self.lock_userTime = self.lock_userTime*skillPlus/100
    end

    if isSuccess == false then
        --清除缓存
        self:clearDataFromPool(useType)
        --停止技能图标转圈
        self:stopTimer()
        --
        self.lockFunc:init();
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000087),nil)
        
    else
        self.lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
        self.lockUI:rebind(self);

        --记录锁定开始时间
        self.startTime = os.time()
        --播放声音
        FishGI.AudioControl:playEffect("sound/lock_01.mp3")
        --开启技能栏边缘光圈转动
        self.btn.parentClasss:setState(2)
        --清除缓存
        self:clearDataFromPool(useType,false)
        --开始倒计时转圈
        self:runTimer()
        --关闭自动发炮d的UI图标
        self.playerSelf.cannon.uiCannonChange:setAutoFire(false)
        --设置玩家锁定目标
        self.lockFunc:start(myPlayerId);
        --开始计时
        self:stopActionByTag(10006)
        local delayAct = cc.Sequence:create(cc.DelayTime:create(self.lock_userTime),cc.CallFunc:create(function ( ... )
            self:endLock();
        end))
        delayAct:setTag(10006)
        self:runAction(delayAct)

        if useType == 1 then
            --更新水晶
            FishGMF.upDataByPropId(myPlayerId,FishCD.PROP_TAG_02,newCrystal,false)
        elseif useType == 0 then
            --更新数量
            FishGMF.addTrueAndFlyProp(myPlayerId,FishCD.PROP_TAG_04,-1,false)
        end
        self:clearDataFromPool(useType)
    end
end

--进入前台刷新时间
function SkillLock:upDateUserTime(disTime )
    if FishGI.isLock == false then
        return
    end
    local curTime =  os.time()
    local curdisTime = curTime - self.startTime
    if curdisTime <=0 then
        return
    end

    if self.lock_userTime == nil then
        return;
    end

    self:upDateTimer()
    if curdisTime > self.lock_userTime then
        self:stopActionByTag(10006)
        self:endLock()
    else
        self:stopActionByTag(10006)
        local times = self.lock_userTime -curdisTime
        local delayAct = cc.Sequence:create(cc.DelayTime:create(times),cc.CallFunc:create(function ( ... )
            self:endLock();
        end))
        delayAct:setTag(10006)
        self:runAction(delayAct)
    end

end

function SkillLock:startOtherLock(evt)
    local data = evt._userdata
    local playerId = data.playerId
    local timelineId = data.timelineId
    local fishArrayId = data.fishArrayId

    local lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
    lockFunc:lockFish(playerId, timelineId, fishArrayId);
    lockFunc:over(playerId);
    lockFunc = nil;
    
    --更新水晶
    FishGMF.upDataByPropId(playerId,2,data.newCrystal)
end

function SkillLock:sendChangeAimFish(data)
    data = data._userdata
    if self.lockFunc ~= nil then
        self.lockFunc:sendChangeTarget(data.timelineId, data.fishArrayId)
    else
        local lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
        lockFunc:lockFish(data.playerId, data.timelineId, data.fishArrayId);
        lockFunc:over(data.playerId);
        lockFunc = nil;
    end
end

--收到玩家改变目标消息
function SkillLock:bulletTargetChange(data )
    data = data._userdata
    local selfId = FishGI.gameScene.playerManager.selfIndex;
    if data.playerId ~= selfId then
        if self.lockFunc ~= nil then
            self.lockFunc:lockFish(data.playerId,data.timelineId,data.fishArrayId)
        else
            local lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
            lockFunc:lockFish(data.playerId, data.timelineId, data.fishArrayId);
            lockFunc:over(data.playerId);
            lockFunc = nil;
        end
    end 
    
end

return SkillLock;