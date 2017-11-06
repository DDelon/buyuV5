-----------------------------------------------------------狂暴锁定--------------------------------------------
cc.exports.LockViolnetUI = class("LockViolnetUI", require("Game/Skill/NormalSkill/SkillUI/SkillUIBase"))
function LockViolnetUI.create(layer)
    local obj = LockViolnetUI.new();
    obj:init(layer);
    return obj;
end

function LockViolnetUI:init(layer)
    self.super.init(self, layer)
end

function LockViolnetUI:initLockSpr(layer)
    layer.lockNode = cc.Node:create()
    layer:addChild(layer.lockNode,FishCD.ORDER_GAME_lock)

    --锁定目标的环
    layer.lockLoop = cc.Sprite:create("battle/effect/effet_violent_lock_0.png")
    layer.lockNode:addChild(layer.lockLoop, 1)
    layer.lockLoop:setTag(101)

    --锁定的箭头
    layer.lockCenterPoint = cc.Sprite:create("battle/effect/effet_violent_lock_1.png")
    layer.lockNode:addChild(layer.lockCenterPoint, 1)
    layer.lockNode:setVisible(false)
    layer.lockCenterPoint:setTag(100)
    
    --锁链
    layer.chain = {}
    layer.chainCount = 8
    for i=layer.chainCount,1,-1 do
        layer.chain[i] = cc.Sprite:create("battle/effect/effet_violent_lock_2.png")
        layer:addChild(layer.chain[i],FishCD.ORDER_GAME_lock-1)
        layer.chain[i]:setVisible(false)
    end
end

--初始化锁链环运动
function LockViolnetUI:initLockAct(layer)
    --锁定目标的环运动
    layer.lockLoop:stopAllActions()
    layer.lockLoop:runAction(cc.RepeatForever:create(cc.RotateBy:create(30,720)))

    --锁定的箭头运动
    layer.lockCenterPoint:stopAllActions()
    --layer.lockCenterPoint:setOpacity(0)
    local seq = cc.Sequence:create(cc.FadeTo:create(0, 0), cc.FadeTo:create(0.5, 255),cc.FadeTo:create(1, 0), cc.DelayTime:create(1))
    layer.lockCenterPoint:runAction(cc.RepeatForever:create(seq))
end

--设置锁链转换目标效果 
function LockViolnetUI:playLockChangeAim(layer)
    --锁定目标的箭头运动
    local fadeAct = cc.Sequence:create(cc.ScaleTo:create(0.2, 1.3), cc.ScaleTo:create(0.05, 1))
    fadeAct:setTag(layer.lockCenterPoint:getTag())
    fadeAct:retain()
    layer.lockCenterPoint:runAction(fadeAct)
    layer.lockCenterPoint:pause()

    --锁定目标的环运动  
    local LoopSeq = cc.Sequence:create(cc.ScaleTo:create(0.2, 1.3), cc.ScaleTo:create(0.05, 1))
    LoopSeq:setTag(layer.lockLoop:getTag())
    LoopSeq:retain()
    layer.lockLoop:runAction(LoopSeq)
    layer.lockLoop:pause()

end

return LockViolnetUI