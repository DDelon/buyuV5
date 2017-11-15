local RoomRuleTips = class("RoomRuleTips", cc.load("mvc").ViewBase)

RoomRuleTips.AUTO_RESOLUTION   = false
RoomRuleTips.RESOURCE_FILENAME = "ui/hall/friend/friendhelpopen/uiroomrule"
RoomRuleTips.RESOURCE_BINDING  = {    
    ["panel"]         = { ["varname"] = "panel" }, 
    ["img_bg"]        = { ["varname"] = "img_bg" }, 
    
--    ["text_roomno"]   = { ["varname"] = "text_roomno" }, 
    ["text_duration"] = { ["varname"] = "text_duration" }, 
    ["text_prop"]     = { ["varname"] = "text_prop" }, 
    ["text_count"]    = { ["varname"] = "text_count" }, 
}

RoomRuleTips.ch_list  = {    
    --["text_word_roomno"]   = { ["str"] = FishGF.getChByIndex(800000314) }, 
    ["text_word_duration"] = { ["str"] = FishGF.getChByIndex(800000358) },
    ["text_word_prop"]     = { ["str"] = FishGF.getChByIndex(800000359) },
    ["text_word_count"]    = { ["str"] = FishGF.getChByIndex(800000360) },   
}
RoomRuleTips.topDis = 6

function RoomRuleTips:onCreate( ... )
    self:openTouchEventListener()
    self:initCh()

    self:initListener()
end

--初始化中文
function RoomRuleTips:initCh()
    for k,v in pairs(self.ch_list) do
        local node = self:child(k)
        if node ~= nil then
            node:setString(v.str)
        end
    end
end

--初始化监听器
function RoomRuleTips:initListener()

    --正在进行的代开列表
    local eventDispatcher = self:getEventDispatcher()
    local openRuleTip = cc.EventListenerCustom:create("openRuleTip", handler(self, self.openRuleTip))
    eventDispatcher:addEventListenerWithSceneGraphPriority(openRuleTip, self)

    local hideRuleTip = cc.EventListenerCustom:create("hideRuleTip", handler(self, self.hideRuleTip))
    eventDispatcher:addEventListenerWithSceneGraphPriority(hideRuleTip, self)

end

function RoomRuleTips:onTouchBegan(touch, event)
    if not self:isVisible() then
        return false
    end
    local curPos = touch:getLocation()  
    local s = self.img_bg:getContentSize()
    local locationInNode = self.img_bg:convertToNodeSpace(curPos)
    local rect = cc.rect(0,0,s.width,s.height)
    if cc.rectContainsPoint(rect,locationInNode) then
        return true
    end
    return false
end

function RoomRuleTips:setRoomId(roomId)
    --self.text_roomno:setString(roomId);
end

function RoomRuleTips:setRoomDuration(duration)
    self.text_duration:setString(duration);
end

function RoomRuleTips:setRoomMaxPlayerNum(numStr)
    self.text_count:setString(numStr..FishGF.getChByIndex(800000357))
end

function RoomRuleTips:setPropType(roomPropType)
    local str = ""
    if roomPropType == 0 then
        str = FishGF.getChByIndex(800000356)
    else
        str = FishGF.getChByIndex(800000355)
    end
    self.text_prop:setString(str)
end

--
function RoomRuleTips:openRuleTip(data)
    print("--------openRuleTip-----------")
    data = data._userdata
    local pos = data.pos

    self:setPositionY(self:changePos(pos).y)
    self:setRoomId(data.friendRoomNo)
    self:setRoomDuration(data.duration)
    self:setPropType(data.roomPropType)
    self:setRoomMaxPlayerNum(data.playerCount)

    self:hideRuleTip({_userdata = {}})
    self:setVisible(true)
    local seq = cc.Sequence:create(cc.DelayTime:create(2),cc.CallFunc:create(function ( ... )
        self:hideRuleTip({_userdata = {}})
    end))
    self:runAction(seq)
end

function RoomRuleTips:hideRuleTip(data)
    --print("--------hideRuleTip-----------")
    --data = data._userdata
    if not self:isVisible() then
        return 
    end
    self:stopAllActions()
    self:setVisible(false)
end

function RoomRuleTips:changePos(pos)
    pos = (self:getParent()):convertToNodeSpace(pos)
    return pos
end

return RoomRuleTips;