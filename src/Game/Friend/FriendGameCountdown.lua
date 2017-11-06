local FriendGameCountdown = class("FriendGameCountdown", cc.load("mvc").ViewBase)

FriendGameCountdown.AUTO_RESOLUTION   = true
FriendGameCountdown.RESOURCE_FILENAME = "ui/battle/friend/uifriendgamecountdown"
FriendGameCountdown.RESOURCE_BINDING  = {
    ["spr_bswks"]           = { ["varname"] = "spr_bswks" },
    ["node_game_timeout"]   = { ["varname"] = "node_game_timeout" },
    ["fnt_time_minu"]       = { ["varname"] = "fnt_time_minu" },
    ["fnt_time_sec"]        = { ["varname"] = "fnt_time_sec" },
}

function FriendGameCountdown:onCreate( ... )
    self:init()
    self:initView()
end

function FriendGameCountdown:init()
    self:openTouchEventListener()
    self.eachTime = tonumber(FishGI.GameConfig:getConfigData("config", "990000071", "data"))
    self.iGameTimeout = self.eachTime
    self.bTimeoutAni = false
    self.leftTime = 0
    self.lastFrameTime = 0
    self.curFrameTime = 0
end

function FriendGameCountdown:setGameTimeout(iGameTimeout)
    self.iGameTimeout = iGameTimeout
end

function FriendGameCountdown:onTouchBegan(touch, event)
    return false
end

function FriendGameCountdown:initView()
    self:runAction(self.resourceNode_["animation"])
    self.resourceNode_["animation"]:play("init", false)
end

function FriendGameCountdown:initData( durationType, leftTime )
    self.durationType = durationType
    if durationType == 1 then
        self.iGameTimeout = self.eachTime*3
    else
        self.iGameTimeout = self.eachTime
    end
    if leftTime > 0 then
        self.leftTime = leftTime/1000
    end
end

function FriendGameCountdown:updateGameStatus(bStart)
    self.bStart = bStart or false
    self.spr_bswks:setVisible(not self.bStart)
    self.node_game_timeout:setVisible(self.bStart)
    self:unscheduleTimes()
    if self.bStart then 
        self.fnt_time_minu:setString(string.format( "%02d",math.floor( self.iGameTimeout / 60 ) ))
        self.fnt_time_sec:setString(string.format( "%02d",self.iGameTimeout % 60 ))
        if self.leftTime >= self.eachTime*2 then
            self.lastFrameTime = self.eachTime*2
        elseif self.leftTime >= self.eachTime then
            self.lastFrameTime = self.eachTime
        end
        local function updateTimeout()
            local curFrameTime = math.floor(LuaCppAdapter:getInstance():getCurFrame()*0.05)
            if self.curFrameTime > curFrameTime then
                self.lastFrameTime = self.lastFrameTime+self.curFrameTime
            end
            self.curFrameTime = curFrameTime
            local iCountDown = self.iGameTimeout-(self.lastFrameTime+self.curFrameTime)
            if iCountDown > 0 then 
                local iMinu = math.floor( iCountDown / 60 )
                local iSec = iCountDown % 60
                self.fnt_time_minu:setString(string.format( "%02d",iMinu ))
                self.fnt_time_sec:setString(string.format( "%02d",iSec ))
                if iMinu == 0 then
                    if not self.bTimeoutAni then
                        self.bTimeoutAni = true
                        self.resourceNode_["animation"]:play("effect_light", true)
                    end
                end
            else 
                self.fnt_time_minu:setString("00")
                self.fnt_time_sec:setString("00")
                self:unscheduleTimes()
                self.resourceNode_["animation"]:play("init", false)
                if self.funTimeoutCallBack then 
                    self.funTimeoutCallBack()
                end 
            end 
        end
        self.getCurFrameScheduleId = cc.Director:getInstance():getScheduler():scheduleScriptFunc(updateTimeout, 0.1, false)
    end 
end

function FriendGameCountdown:unscheduleTimes()
    if self.getCurFrameScheduleId ~= nil then
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.getCurFrameScheduleId)
        self.getCurFrameScheduleId = nil
    end
end

--设置回调
function FriendGameCountdown:setTimeoutCallBack( callback )
    self.funTimeoutCallBack = callback
end

return FriendGameCountdown