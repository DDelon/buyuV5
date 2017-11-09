local OnTheMarch = class("OnTheMarch", cc.load("mvc").ViewBase)
local MAX_ROOM_NUM = 4

OnTheMarch.AUTO_RESOLUTION   = false
OnTheMarch.RESOURCE_FILENAME = "ui/hall/friend/friendhelpopen/uionthemarch.lua"
OnTheMarch.RESOURCE_BINDING  = {
    ["panel"] = { ["varname"] = "panel" },
    ["img_bg"]     = { ["varname"] = "img_bg"  },
    ["line"] = { ["varname"] = "line"  },

    ["text_name_title"]     = { ["varname"] = "text_name_title"  },    
    ["text_roomid_title"]    = { ["varname"] = "text_roomid_title"} , 
    ["text_roomid"]     = { ["varname"] = "text_roomid"  },   
    ["text_duration"]     = { ["varname"] = "text_duration"  },
    ["text_openroom_time"]     = { ["varname"] = "text_openroom_time"  }, 

    ["btn_roomrule"]        = { ["varname"] = "btn_roomrule" ,         ["events"]={["event"]="click",["method"]="onClickRule"}},
    ["btn_dissolution"]        = { ["varname"] = "btn_dissolution" ,         ["events"]={["event"]="click",["method"]="onClickDissolution"}}, 
    ["btn_invitation"]        = { ["varname"] = "btn_invitation" ,         ["events"]={["event"]="click",["method"]="onClickInvitation"}}, 

    ["node_head1"] = { ["varname"] = "node_head1"  },
    ["node_head2"] = { ["varname"] = "node_head2"  },
    ["node_head3"] = { ["varname"] = "node_head3"  },
    ["node_head4"] = { ["varname"] = "node_head4"  },
}

function OnTheMarch:onCreate(...)
    self:productPlayerHead(MAX_ROOM_NUM);
end

function OnTheMarch:productPlayerHead(num)
    self.headTab = {};

    local mode = require("common/HeadStyleSimple")
    for key = 1, num do
        local node = self["node_head"..key]
        --self:runAction(self["node_head"..key].animation)
        node = mode.new(self, node)
        self["node_head"..key] = node
        node:switchNull();
        node:setId(key);
        table.insert(self.headTab, node);
    end
end

function OnTheMarch:getHeadByLocalId(localId)
    if self.headTab == nil then
        return nil;
    end

    for key, val in pairs(self.headTab) do
        local id = val:getId();
        if localId == id then
            return val;
        end
    end
end

function OnTheMarch:getHeadByPlayerId(playerId)
    if self.headTab == nil then
        return nil;
    end

    for key, val in pairs(self.headTab) do
        local id = val:getPlayerId();
        if playerId == id then
            return val;
        end
    end
end

function OnTheMarch:setRoomId(roomId)
    self.text_roomid:setString(roomId);
end

function OnTheMarch:getRoomId()
    return tonumber(self.text_roomid:getString());
end

function OnTheMarch:setRoomDuration(duration)
    self.text_duration:setString(duration);
end

function OnTheMarch:getRoomDuration()
    return self.text_duration:getString();
end

function OnTheMarch:setRoomOpenTime(openTime)
    self.text_openroom_time:setString(openTime);
end

function OnTheMarch:getRoomOpenTime()
    return self.text_openroom_time:getString();
end

function OnTheMarch:setRoomMaxPlayerNum(num)
    self.maxPlayerNum = num;

    for key = 1, MAX_ROOM_NUM do
        local head = self:getHeadByLocalId(key);
        if key <= self.maxPlayerNum then
            if head ~= nil then
                head:setVisible(true);
            end
        else
            if head ~= nil then
                head:setVisible(false);
            end
        end
    end
end

function OnTheMarch:getRoomMaxPlayerNum()
    return self.maxPlayerNum;
end

function OnTheMarch:setInfo(roomInfo)
    self.roomInfo = roomInfo
    self.friendGameId = roomInfo.friendGameId;
    local friendRoomNo = roomInfo.friendRoomNo;
    local duration = roomInfo.duration;
    local createTime = roomInfo.createTime;
    local playerCount = roomInfo.playerCount


    self:setRoomId(friendRoomNo);
    self:setRoomDuration(duration);
    self:setRoomOpenTime(createTime);
    self:setRoomMaxPlayerNum(playerCount)

end

function OnTheMarch:playerJoinRoom(info)
    local localId = info.chairId;
    local headPath = info.path;
    local playerId = info.playerId;
    local name = info.name;
    local isReady = info.isReady
    local score = info.score

    if localId > self.maxPlayerNum then
        return;
    end

    local head = self:getHeadByLocalId(localId);
    head:setPlayerId(playerId);
    head:setName(name);
    head:setHead(headPath);
    head:setIsReady(isReady);
    head:switchWithoutScoreMode();
end

function OnTheMarch:playerExitRoom(playerId)
    local head = self:getHeadByPlayerId(playerId);
    if head ~= nil then
        head:switchNull();
    end
end

function OnTheMarch:onClickRule(sender)
    local event = cc.EventCustom:new("openRuleTip")
    event._userdata = self.roomInfo
    local pos = cc.p(self:getPositionX(),self:getPositionY())
    pos = (self:getParent()):convertToWorldSpace(pos)
    event._userdata.pos = pos
    cc.Director:getInstance():getEventDispatcher():dispatchEvent(event)
end

function OnTheMarch:onClickDissolution(sender)
    if self.friendGameId ~= nil then
        FishGI.FriendRoomManage:sendFriendDissolveRoom(self.friendGameId)
    else
        print("----------friendGameId == nil------------")
    end
    
end

function OnTheMarch:onClickInvitation(sender)
end

return OnTheMarch;
