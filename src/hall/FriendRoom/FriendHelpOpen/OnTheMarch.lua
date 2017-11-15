local OnTheMarch = class("OnTheMarch", cc.load("mvc").ViewBase)
local MAX_ROOM_NUM = 4

OnTheMarch.AUTO_RESOLUTION   = false
OnTheMarch.RESOURCE_FILENAME = "ui/hall/friend/friendhelpopen/uionthemarch"
OnTheMarch.RESOURCE_BINDING  = {
    ["panel"]              = { ["varname"] = "panel" },
    ["img_bg"]             = { ["varname"] = "img_bg"  },
    ["line"]               = { ["varname"] = "line"  },
    
    ["text_name_title"]    = { ["varname"] = "text_name_title"  },    
    ["text_roomid_title"]  = { ["varname"] = "text_roomid_title"} , 
    ["text_roomid"]        = { ["varname"] = "text_roomid"  },   
    ["text_duration"]      = { ["varname"] = "text_duration"  },
    ["text_openroom_time"] = { ["varname"] = "text_openroom_time"  }, 
    
    ["btn_roomrule"]       = { ["varname"] = "btn_roomrule" ,         ["events"]={["event"]="click",["method"]="onClickRule"}},
    ["btn_dissolution"]    = { ["varname"] = "btn_dissolution" ,         ["events"]={["event"]="click",["method"]="onClickDissolution"}}, 
    ["btn_invitation"]     = { ["varname"] = "btn_invitation" ,         ["events"]={["event"]="click",["method"]="onClickInvitation"}}, 
    
    ["node_head1"]         = { ["varname"] = "node_head1"  },
    ["node_head2"]         = { ["varname"] = "node_head2"  },
    ["node_head3"]         = { ["varname"] = "node_head3"  },
    ["node_head4"]         = { ["varname"] = "node_head4"  },
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

    self:setRoomMaxPlayerNum(playerCount)
    self:setRoomId(friendRoomNo);
    self:setRoomDuration(duration);
    self:setRoomOpenTime(createTime);

end

function OnTheMarch:playerJoinRoom(info)
    local localId = info.chairId + 1;
    local headPath = info.path;
    local playerId = info.playerId;
    local name = info.name;
    local isReady = info.isReady
    local score = info.score

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
    head:setIsReady(isReady);
    head:switchWithoutScoreMode();
end

function OnTheMarch:playerExitRoomByChairId(chairId)
    local head = self:getHeadByLocalId(chairId)
    if head ~= nil then
        head:switchNull();
        head:setIsReady(false);
    end
end

function OnTheMarch:playerExitRoom(playerId)
    local head = self:getHeadByPlayerId(playerId);
    if head ~= nil then
        head:switchNull();
        head:setIsReady(false);
    end
end

function OnTheMarch:playerIsReady(playerId,isReady)
    local head = self:getHeadByPlayerId(playerId)
    if head ~= nil then
        head:setIsReady(isReady)
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
    local shareInfo = FishGI.WebUserData:GetShareDataTable();
    local url = shareInfo.url
    if url == nil then
        url = "https://client-fish.weile.com/share/fish/channel_id/"..CHANNEL_ID.."/from_app/"..APP_ID.."/from_region/0"
    end
    local wechatAppId = shareInfo.id
    if wechatAppId == nil then
        wechatAppId = WX_APP_ID_LOGIN
    end

    local title = FishGF.getChByIndex(800000336)..FishGF.getChByIndex(800000241)..FishGF.getChByIndex(800000218)..self.roomInfo.friendRoomNo

    local createData = FishGI.hallScene.uiCreateLayer.createData
    local roomPropType = FishGF.getChByIndex(800000333)..FishGF.getChByIndex(800000218)..(FishGF.changeRoomData("roomPropType",self.roomInfo.roomPropType).str)
    local roomPeopleCountType = FishGF.getChByIndex(800000334)..FishGF.getChByIndex(800000218)..(FishGF.changeRoomData("roomPeopleCountType",self.roomInfo.roomPeopleCountType).str)
    local roomDurationType = FishGF.getChByIndex(800000335)..FishGF.getChByIndex(800000218)..(FishGF.changeRoomData("roomDurationType",self.roomInfo.roomDurationType).str)
    local des = roomPropType.."\n"..roomPeopleCountType.."\n"..roomDurationType
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if (cc.PLATFORM_OS_WINDOWS ~= targetPlatform) then
        FishGI.ShareHelper:doShareAppWebType(title,des,url,0,wechatAppId)
    end

end

return OnTheMarch;
