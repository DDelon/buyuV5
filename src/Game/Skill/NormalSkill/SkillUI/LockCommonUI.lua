----------------------------------------------------普通场锁定--------------------------------------------------------
local LockCommonUI = class("LockCommonUI", require("Game/Skill/NormalSkill/SkillUI/SkillUIBase"))

function LockCommonUI.create(layer)
    local obj = LockCommonUI.new();
    obj:init(layer);
    return obj;
end

function LockCommonUI:init(layer)
    self.super.init(self, layer);
end

--初始化锁定精灵
function LockCommonUI:initLockSpr(layer)

    layer.lockNode = cc.Node:create()
    layer:addChild(layer.lockNode,1)
    layer.lockNode:setVisible(false)
    --锁定目标的环
    layer.lockLoop = cc.Sprite:create("battle/effect/effect_lock_0.png")
    layer.lockNode:addChild(layer.lockLoop)
    layer.lockLoop:setTag(101)

    --锁定的箭头
    layer.lockArrow = cc.Sprite:create("battle/effect/effect_lock_1.png")
    layer.lockNode:addChild(layer.lockArrow)
    layer.lockArrow:setTag(100)
    
    --锁链
    layer.chain = {}
    layer.chainCount = 8;
    for i=layer.chainCount,1,-1 do
        layer.chain[i] = cc.Sprite:create("battle/effect/effect_lock_2.png")
        layer:addChild(layer.chain[i],0)
        layer.chain[i]:setVisible(false)
    end


end

--初始化锁链环运动
function LockCommonUI:initLockAct(layer)
    --锁定目标的环运动
    layer.lockLoop:stopAllActions()
    layer.lockLoop:runAction(cc.RepeatForever:create(cc.RotateBy:create(4,360)))

    --锁定的箭头运动
    layer.lockArrow:stopAllActions()
    local seq = cc.Sequence:create(cc.ScaleTo:create(0.13,0.8),cc.ScaleTo:create(0.87,1))
    layer.lockArrow:runAction(cc.RepeatForever:create(seq))
end

--设置锁链转换目标效果 
function LockCommonUI:playLockChangeAim( layer )
    --锁定目标的箭头运动
    local scaleAct1 = cc.ScaleTo:create(0.13,0.9)
    local OpacityAct1 = cc.FadeTo:create(0.13,255)
    local spawnAct1 = cc.Spawn:create(scaleAct1,OpacityAct1)

    local act2 = cc.ScaleTo:create(0.03,1)
    local rotate = cc.RotateBy:create(0.16,80)
  
    local seq1 = cc.Sequence:create(cc.ScaleTo:create(0.01,1.8),spawnAct1,act2)
    local spawnAct2 = cc.Spawn:create(seq1,rotate)

    spawnAct2:setTag(layer.lockArrow:getTag())
    spawnAct2:retain()
    layer.lockArrow:runAction(spawnAct2)
    layer.lockArrow:pause()

    --锁定目标的环运动  
    local LoopSeq = cc.Sequence:create(cc.ScaleTo:create(0,1.2),cc.ScaleTo:create(0.13,0.8),cc.ScaleTo:create(0.03,1))
    LoopSeq:setTag(layer.lockLoop:getTag())
    LoopSeq:retain()
    layer.lockLoop:runAction(LoopSeq)
    layer.lockLoop:pause()

end

return LockCommonUI