local Timer = class("Timer")

function Timer:ctor()
    self.scheduler_ = cc.Director:getInstance():getScheduler()
    self.timers_ = {}
end

--[[
启动定时器
@param callback 回调方法
@param interval 间隔
@param runCount 运行次数
@param data ...数据
@return timerId
]]
function Timer:start(callback, interval, runCount, ...)
    local timerId
    local data = { ... }
    local onTick = function(dt)
        if callback then
            callback(dt, data, timerId)
        end
        if runCount ~= nil and type(runCount) == "number" then
            runCount = runCount - 1;
            if runCount <= 0 then -- 达到指定运行次数,杀掉
                self:kill(timerId)
            end
        end
    end
    timerId = self.scheduler_:scheduleScriptFunc(onTick, interval or 0, false)
    self.timers_[timerId] = 1;
    return timerId
end

-- 倒计时函数
-- -   callback: 计时器执行回调函数
-- -   countdown: 倒计时（秒）
-- -   interval（可选）: 检查倒计时的时间间隔,最小为 1 秒,如果未指定则默认为 1 秒
function Timer:addCountdown(callback, countdown, interval, ...)
    local timerId
    interval = interval or 1
    assert(callback," Countdown callback params is nil ")
    assert(type(countdown) == "number", "invalid countdown ")
    assert(type(interval) == "number", "invalid interval ")
    local cusor = countdown or 30
    local function shch_func_(dt, data, timerId)
        cusor = cusor - dt
        if cusor <= 0 then
            cusor = 0
            self:kill(timerId)
        end      
        callback(checknumber(cusor), dt, data, timerId)         
    end
    timerId = self:start(shch_func_, interval, nil, ...);
    return timerId
end


--[[
启动一个只执行一次的定时器
@param callback 回调方法
@param data 数据
]]
function Timer:runOnce(callback, interval, data, ...)
   return self:start(callback, interval, 1, data, ...)
end

--[[
杀掉指定定时器
@param timerId 定时器ID
]]
function Timer:kill(timerId)
    self.scheduler_:unscheduleScriptEntry(timerId)
    self.timers_[timerId] = nil;
end

--[[
杀掉所有定时器
]]
function Timer:killAll()
    for timerId, flag in pairs(self.timers_) do
        self:kill(timerId)
    end
end

return Timer
