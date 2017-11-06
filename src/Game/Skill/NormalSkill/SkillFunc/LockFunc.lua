---------------------------------------------------------------锁定功能
local LockFunc = class ("LockFunc")

function LockFunc.create()
    local obj = LockFunc.new();
    obj:init();
    FishGI.lockCount = FishGI.lockCount+1;
    return obj;
end

function LockFunc:init()
    self.timelineId = 0;
    self.fishArrayId = 0;
end

function LockFunc:start(playerId)
    --开启锁定标志量
    FishGI.isLock = true
    local player = FishGI.gameScene.playerManager:getMyData();
    local ret = FishGMF.setLockData(playerId,1,self.timelineId,self.fishArrayId);
    if ret.x ~= nil then
        if player ~= nil then
            if ret.x > 0 and  ret.y > 0 then
                player:shoot(cc.p(ret.x,ret.y));
            end
        end
    else
        if player ~= nil then
            player:shootByDegree(90);
        end
    end
end

function LockFunc:lockFish(playerId, timelineId, fishArrayId)
    FishGMF.setLockData(playerId,3,timelineId,fishArrayId)
end

function LockFunc:over(playerId)
    
    FishGI.lockCount = FishGI.lockCount-1;
    if FishGI.lockCount > 0 or playerId ~= FishGI.gameScene.playerManager.selfIndex then
        return false;
    end
    FishGMF.setLockData(playerId,2,0,0)
    FishGI.isLock = false;
    local player = FishGI.gameScene.playerManager:getMyData();
    if player ~= nil then
        player:endShoot();
    end 
    return true;
end

function LockFunc:changeLockTarget(pos)
    local player = FishGI.gameScene.playerManager:getMyData()
    local playerId = FishGI.gameScene.playerManager.selfIndex
    local timelineId,fishArrayId = self:getLockFishByPos(pos)

    if timelineId ~= nil and (timelineId ~= 0 or fishArrayId ~= 0)  then
        --锁定目标变换
        self:sendChangeTarget(timelineId,fishArrayId)
        --
        FishGMF.setLockData(playerId,3,timelineId,fishArrayId)
    end
end

--得到锁定子弹
function LockFunc:getLockBullet(playerId)
    local bullerCount = 0
    local bulletsTab = {}
    local dataTab = {}
    dataTab.funName = "getLockBullet"
    dataTab.playerId = playerId
    local bullets = LuaCppAdapter:getInstance():luaUseCppFun(dataTab)
    for key,val in pairs(bullets) do
        bullerCount = bullerCount +1
        table.insert(bulletsTab,val)
    end
    return bullerCount,bulletsTab
end

function LockFunc:getLockFishByScore()
    local dataTab = {}
    dataTab.funName = "getLockFishByScore"
    local data = LuaCppAdapter:getInstance():luaUseCppFun(dataTab);
    local timelineId = data["timelineId"]
    local fishArrayId = data["fishArrayId"]
    if timelineId == nil then
        return 
    end

    return timelineId,fishArrayId
end

function LockFunc:getLockFishByPos(curPos)
    local locationInNode = FishGI.gameScene:convertToNodeSpace(curPos)
    local dataTab = {}
    dataTab.funName = "getFishByPos"
    dataTab.posX = locationInNode.x
    dataTab.posY = locationInNode.y
    local aimFish = LuaCppAdapter:getInstance():luaUseCppFun(dataTab);
    if aimFish["timelineId"] == 0 and  aimFish["fishArrayId"] == 0  then
        return 
    end
    local timelineId = aimFish["timelineId"]
    local fishArrayId = aimFish["fishArrayId"]
    return timelineId,fishArrayId
end

----------------------------------------------------net message-------------------------------
function LockFunc:sendStartLock(useType)
    local timelineId,fishArrayId = self:getLockFishByScore()
    self.timelineId = timelineId;
    self.fishArrayId = fishArrayId;
    FishGI.gameScene.net:sendlockFish(timelineId,fishArrayId,useType)
end

function LockFunc:sendChangeTarget(timelineId, fishArrayId)
    if timelineId == 0 then
        print("-------timelineId <=0----------")
        return
    end
    local playerId = FishGI.gameScene.playerManager.selfIndex
    local bulletCount,bullets = self:getLockBullet(playerId)
    local data = {}
    data.timelineId = timelineId
    data.fishArrayId = fishArrayId
    data.bullets = bullets
    FishGI.gameScene.net:sendBulletTargetChange(data)
end
return LockFunc