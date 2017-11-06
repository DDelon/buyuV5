local BuffAim = class("BuffAim", require("Game/Skill/Buff/BuffTime"))

function BuffAim.create(targetPlayerId, propId, buffNum)
	local obj = BuffAim.new();
	obj:init(targetPlayerId, propId, buffNum);
	return obj;
end

function BuffAim:init(targetPlayerId, propId, buffNum)
	BuffAim.super.init(self, targetPlayerId, propId, buffNum);
    self:registerListener();
    if targetPlayerId == FishGI.gameScene.playerManager.selfIndex then
        FishGI.gameScene.lockUI:rebind(FishGI.gameScene.uiMainLayer);
        self.lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
    end
    self:createTouchLayer();
end

function BuffAim:registerListener()
    local changeTargetListener=cc.EventListenerCustom:create("bulletTargetChange",function(data) self:onBulletTargetChange(data) end)  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(changeTargetListener, 1)

    local sendChangeAimFishListener=cc.EventListenerCustom:create("sendChangeAimFish",handler(self, self.sendChangeAimFish))  
    cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(sendChangeAimFishListener, 1)
end

function BuffAim:removeListener()
    cc.Director:getInstance():getEventDispatcher():removeCustomEventListeners("bulletTargetChange");
    cc.Director:getInstance():getEventDispatcher():removeCustomEventListeners("sendChangeAimFish");
end

function BuffAim:createTouchLayer()
    local function callback(pSender, eventName)
        self:playerSelectFish(pSender, eventName);
    end

    if self.targetPlayerId == FishGI.gameScene.playerManager.selfIndex then

        self.touchLayer = ccui.Button:createInstance();
        self.touchLayer:setAnchorPoint(cc.p(0.5, 0.5));
        self.touchLayer:setPosition(cc.p(cc.Director:getInstance():getWinSize().width/2, cc.Director:getInstance():getWinSize().height/2))
        self.touchLayer:setScale9Enabled(true);
        self.touchLayer:setContentSize(cc.Director:getInstance():getWinSize());
        self.touchLayer:addTouchEventListener(callback);
        FishGI.gameScene.uiMainLayer:addChild(self.touchLayer, FishCD.ORDER_GAME_lock);
    end
end

function BuffAim:removeTouchLayer()
    if self.targetPlayerId == FishGI.gameScene.playerManager.selfIndex then
        self.touchLayer:removeFromParent();
    end
end

function BuffAim:executeBuff(skillManager)
    print("buff aim execute buff")
	BuffAim.super.executeBuff(self, skillManager)
    

    if self.targetPlayerId ~= FishGI.gameScene.playerManager.selfIndex then
        return;
    end
	local player = FishGI.gameScene.playerManager:getPlayerByPlayerId(FishGI.gameScene.playerManager.selfIndex);
	self.cannonPos = player:getCannonPos();
    if self.lockFunc ~= nil then
	    self.lockFunc:start(self.targetPlayerId);
    end
    
end

function BuffAim:stopBuff()
	BuffAim.super.stopBuff(self);
    self:removeListener();
    self:removeTouchLayer();
    if self.lockFunc ~= nil then
        self.lockFunc:over(self.targetPlayerId);
    end
    cc.Director:getInstance():getEventDispatcher():removeCustomEventListeners("bulletTargetChange");
end

function BuffAim:playerSelectFish(pSender, eventName)
    if eventName == ccui.TouchEventType.ended then
        local curPos = pSender:getTouchEndPosition();

        self.lockFunc:changeLockTarget(curPos);
    end
end

function BuffAim:onBulletTargetChange(evt)
    print("aim target change");
    local data = evt._userdata;
    local selfId = FishGI.gameScene.playerManager.selfIndex;
    if data.playerId ~= selfId then
        if self.lockFunc ~= nil then
            self.lockFunc:lockFish(data.playerId, data.timelineId, data.fishArrayId);
        else
            self.lockFunc = require("Game/Skill/NormalSkill/SkillFunc/LockFunc").create();
            self.lockFunc:lockFish(data.playerId, data.timelineId, data.fishArrayId);
            self.lockFunc:over(data.playerId);
            self.lockFunc = nil;
        end
        
        
    end 
end

function BuffAim:sendChangeAimFish(data)
    data = data._userdata
    if self.lockFunc ~= nil then
        self.lockFunc:sendChangeTarget(data.timelineId, data.fishArrayId)
    end
end



return BuffAim;