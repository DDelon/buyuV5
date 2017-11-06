local SKillUIBase = class("SKillUIBase")

function SKillUIBase:init(layer)
    self:initLockSpr(layer)
    self:initLockAct(layer)
    self:playLockChangeAim(layer)
    self:setLuaNode(layer)
end

function SKillUIBase:rebind(layer)
    LuaCppAdapter:getInstance():setLuaNode(1,layer,{});
    self:initLockAct(layer)
    self:playLockChangeAim(layer)
    self:setLuaNode(layer)
end

--将锁定精灵绑定到c++
function SKillUIBase:setLuaNode(layer)
    local data = {}
    data.bindType= "lockNode"
    LuaCppAdapter:getInstance():setLuaNode(2,layer.lockNode,data);

    for i=layer.chainCount,1,-1 do
        local data = {}
        data.bindType= "chain"
        data.index= i
        LuaCppAdapter:getInstance():setLuaNode(2,layer.chain[i],data);
    end
end

return SKillUIBase