local HelpOpenRecord = class("HelpOpenRecord", cc.load("mvc").ViewBase)
local MAX_ROOM_NUM = 4

HelpOpenRecord.AUTO_RESOLUTION   = false
HelpOpenRecord.RESOURCE_FILENAME = "ui/hall/friend/friendhelpopen/uihelpopenrecord"
HelpOpenRecord.RESOURCE_BINDING  = {
    ["panel"]              = { ["varname"] = "panel" },
    ["img_bg"]             = { ["varname"] = "img_bg"  },
    ["line"]               = { ["varname"] = "line"  },
    
    ["text_name_title"]    = { ["varname"] = "text_name_title"  },    
    ["text_roomid_title"]  = { ["varname"] = "text_roomid_title"} , 
    ["text_roomid"]        = { ["varname"] = "text_roomid"  },
    ["text_openroom_time"] = { ["varname"] = "text_openroom_time"  }, 
    
    ["node_head1"]         = { ["varname"] = "node_head1"  },
    ["node_head2"]         = { ["varname"] = "node_head2"  },
    ["node_head3"]         = { ["varname"] = "node_head3"  },
    ["node_head4"]         = { ["varname"] = "node_head4"  },
}

function HelpOpenRecord:onCreate(...)
    self:productPlayerHead(MAX_ROOM_NUM);
end

function HelpOpenRecord:productPlayerHead(num)
    self.headTab = {};

    local mode = require("common/HeadStyleSimple")
    for key = 1, num do
        local node = self["node_head"..key]
        node = mode.new(self, node)
        self["node_head"..key] = node
        node:switchNull();
        node:setId(key);
        table.insert(self.headTab, node);
    end
end

function HelpOpenRecord:getHeadByLocalId(localId)
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

function HelpOpenRecord:getHeadByPlayerId(playerId)
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

function HelpOpenRecord:setRoomId(roomId)
    self.text_roomid:setString(roomId);
end

function HelpOpenRecord:getRoomId()
    return tonumber(self.text_roomid:getString());
end

function HelpOpenRecord:setRoomOpenTime(openTime)
    self.text_openroom_time:setString(openTime);
end

function HelpOpenRecord:getRoomOpenTime()
    return self.text_openroom_time:getString();
end

function HelpOpenRecord:setRoomMaxPlayerNum(num)
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

function HelpOpenRecord:getRoomMaxPlayerNum()
    return self.maxPlayerNum;
end

function HelpOpenRecord:setInfo(roomInfo)
    self.roomInfo = roomInfo
    self.friendGameId = roomInfo.friendGameId;
    local friendRoomNo = roomInfo.friendRoomNo;
    local duration = roomInfo.duration;
    local createTime = roomInfo.createTime;
    local playerCount = roomInfo.playerCount


    self:setRoomId(friendRoomNo);
    --self:setRoomDuration(duration);
    self:setRoomOpenTime(createTime);
    self:setRoomMaxPlayerNum(playerCount)
end

function HelpOpenRecord:playerJoinRoom(info)
    local localId = info.chairId + 1
    local headPath = info.path;
    local playerId = info.playerId;
    local name = info.name;
    local score = info.score;

    if localId > self.maxPlayerNum then
        return;
    end

    local head = self:getHeadByLocalId(localId);
    if head == nil then
        return nil;
    end
    head:setPlayerId(playerId);
    head:setName(name);
    head:setHead(headPath);
    head:setScore(score);
    head:switchWithScoreMode();
end

function HelpOpenRecord:playerExitRoomByChairId(chairId)
    local head = self:getHeadByLocalId(chairId)
    if head ~= nil then
        head:switchNull();
    end
end

function HelpOpenRecord:playerExitRoom(playerId)
    local head = self:getHeadByPlayerId(playerId);
    if head ~= nil then
        head:switchNull();
    end
end

return HelpOpenRecord;