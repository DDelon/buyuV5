local HelpOpenLayer = class("HelpOpenLayer", cc.load("mvc").ViewBase)

HelpOpenLayer.AUTO_RESOLUTION   = false
HelpOpenLayer.RESOURCE_FILENAME = "ui/hall/friend/friendhelpopen/uihelpopenroom"
HelpOpenLayer.RESOURCE_BINDING  = {    
    ["panel"]         = { ["varname"] = "panel" }, 
    ["btn_close"]       = { ["varname"] = "btn_close" ,        ["events"]={["event"]="click",["method"]="onClickclose"}}, 

    ["btn_running"]        = { ["varname"] = "btn_running" ,         ["events"]={["event"]="click_color",["method"]="onClickrunning"}},   
    ["btn_record"]        = { ["varname"] = "btn_record" ,         ["events"]={["event"]="click_color",["method"]="onClickrecord"}},   

    ["btn_create"]        = { ["varname"] = "btn_create" ,         ["events"]={["event"]="click",["method"]="onClickcreate"}},   
    ["btn_refresh"]        = { ["varname"] = "btn_refresh" ,         ["events"]={["event"]="click",["method"]="onClickrefresh"}},   
        

    ["scroll_onthemarch"]        = { ["varname"] = "scroll_onthemarch",        ["nodeType"]="viewlist"   },
    ["scroll_record"]        = { ["varname"] = "scroll_record",        ["nodeType"]="viewlist"   },

    ["node_roomrule"]         = { ["varname"] = "node_roomrule" }, 
}

HelpOpenLayer.ch_list  = {    
    ["text_notice"]         = { ["str"] = FishGF.getChByIndex(800000322) }, 
}
HelpOpenLayer.topDis = 6

function HelpOpenLayer:onCreate( ... )
    self:openTouchEventListener()
    self:initCh()

    self.listData = {}
    self.playerChangeNetData = {}
    self.startload = false

    self.node_roomrule:setLocalZOrder(300)
    self:initViewData("running",self.scroll_onthemarch)
    self:initViewData("record",self.scroll_record)
    
    self:initListener()

    local mode = require("hall/FriendRoom/FriendHelpOpen/RoomRuleTips")
    local node = self.node_roomrule
    node = mode.new(self, node)
    self.node_roomrule = node
    self.node_roomrule:hideRuleTip({})
end

--初始化中文
function HelpOpenLayer:initCh()
    for k,v in pairs(self.ch_list) do
        local node = self:child(k)
        if node ~= nil then
            node:setString(v.str)
        end
    end
end

--初始化监听器
function HelpOpenLayer:initListener()

    --正在进行的代开列表
    local eventDispatcher = self:getEventDispatcher()
    local FriendRefreshRunningList = cc.EventListenerCustom:create("FriendRefreshRunningList", handler(self, self.FriendRefreshRunningList))
    eventDispatcher:addEventListenerWithSceneGraphPriority(FriendRefreshRunningList, self)

    local FriendRefreshHelpOpenList = cc.EventListenerCustom:create("FriendRefreshHelpOpenList", handler(self, self.FriendRefreshHelpOpenList))
    eventDispatcher:addEventListenerWithSceneGraphPriority(FriendRefreshHelpOpenList, self)

    local FriendRunningStart = cc.EventListenerCustom:create("FriendRunningStart", handler(self, self.FriendRunningStart))
    eventDispatcher:addEventListenerWithSceneGraphPriority(FriendRunningStart, self)

    local FriendPlayerStateChange = cc.EventListenerCustom:create("FriendPlayerStateChange", handler(self, self.FriendPlayerStateChange))
    eventDispatcher:addEventListenerWithSceneGraphPriority(FriendPlayerStateChange, self)

    local FriendDissolveRoom = cc.EventListenerCustom:create("FriendDissolveRoom", handler(self, self.FriendDissolveRoom))
    eventDispatcher:addEventListenerWithSceneGraphPriority(FriendDissolveRoom, self)

end

function HelpOpenLayer:showLayer(type)
    self.super.showLayer(self)
    if type == nil then
        type = "running"
    end
    self:setCurType(type)
    --发送监听关注消息
    self:sendNetData("StartMonitor")
    self.node_roomrule:hideRuleTip({})
end

function HelpOpenLayer:hideLayer()
    self.super.hideLayer(self)
    self:sendNetData("CancelMonitor")
    self.curType = nil
    self.playerChangeNetData = {}
end

function HelpOpenLayer:setCurType(type)
    self.curType = type
    local img_down_2 = self:child("img_down_2")
    local img_down_1 = self:child("img_down_1")
    if type == "running" then  --进行中
        -- body
        self.scroll_onthemarch:setVisible(true)
        self.scroll_record:setVisible(false)
        self:setBtnType("running")
        img_down_1:setVisible(true)
        img_down_2:setVisible(false)
    elseif type == "record" then  --历史记录
        self.scroll_record:setVisible(true)
        self.scroll_onthemarch:setVisible(false)
        self:setBtnType("record")
        img_down_1:setVisible(false)
        img_down_2:setVisible(true)
    end
end

function HelpOpenLayer:getCurType()
    return self.curType
end

function HelpOpenLayer:setBtnType(type)
    if type == "running" then
        self:setBtnIsEnable(self.btn_running,false)
        self:setBtnIsEnable(self.btn_record,true)
    elseif type == "record" then
        self:setBtnIsEnable(self.btn_running,true)
        self:setBtnIsEnable(self.btn_record,false)
    end
end

function HelpOpenLayer:setBtnIsEnable(btn,isEnable)
    btn:setEnabled(isEnable)
    local node_1 = btn:getChildByName("node_state_1")
    local node_2 = btn:getChildByName("node_state_2")
    node_1:setVisible(not isEnable)
    node_2:setVisible(isEnable)
    if isEnable == true then
        btn:setLocalZOrder(9)
    else
        btn:setLocalZOrder(10)
    end
end

function HelpOpenLayer:onTouchBegan(touch, event)
    if not self:isVisible() then
         return false
    end
    return true
end

function HelpOpenLayer:onClickclose( sender )
    print("onClickclose")
    self:buttonClicked("close")
end

function HelpOpenLayer:onClickrunning( sender )
    self:buttonClicked("running")
end

function HelpOpenLayer:onClickrecord( sender )
    self:buttonClicked("record")
end

function HelpOpenLayer:onClickcreate( sender )
    self:buttonClicked("create")
end

function HelpOpenLayer:onClickrefresh( sender )
    self:buttonClicked("refresh")
end

function HelpOpenLayer:buttonClicked(viewTag, btnTag)
    if viewTag == nil then
        print("HelpOpenLayer - buttonClicked---viewTag== nil --")
        return 
    end
    print("HelpOpenLayer -- buttonClicked---viewTag = "..viewTag)
    if viewTag == "close" then 
        self:hideLayer()
    elseif viewTag == "running" then 
        self:setCurType("running")
        self:sendNetData("running")
        self.node_roomrule:hideRuleTip({})
    elseif viewTag == "record" then 
        self:setCurType("record")
        self:sendNetData("record")
        self.node_roomrule:hideRuleTip({})
    elseif viewTag == "create" then 
        FishGI.hallScene.uiCreateLayer:showLayer(nil,nil,nil,true,1) 
    elseif viewTag == "refresh" then 
        self:sendNetData(self.curType)
    end

end

--初始化列表视图数据
function HelpOpenLayer:initViewData(mType,scroll)
    scroll:setLocalZOrder(100)
    local Val = {}
    Val.mType = mType
    Val.scroll = scroll
    local cellCountSize = scroll:getContentSize()
    -- 计算出每个格子的宽高
    Val.cellW = cellCountSize.width
    local RecordItem = self:createRecordItem(mType,nil)
    Val.cellH = RecordItem.img_bg:getContentSize().height+8
    if mType == "running" then
        Val.showCount = 3
    elseif mType == "record" then
        Val.showCount = 3
    end
    self.listData[mType] = Val
    self.listData[mType].ItemList = {}

    local img_top = self:child("img_top")
    local img_down_1 = self:child("img_down_1")
    local img_down_2 = self:child("img_down_2")
    img_top:setLocalZOrder(200)
    img_down_1:setLocalZOrder(200)
    img_down_2:setLocalZOrder(200)
    scroll.img_top = img_top
    scroll.img_down = img_down_2
    if mType == "running" then
        scroll.img_down = img_down_1
    end

    local function scrollviewEvent(sender,eventType)
        if eventType==ccui.ScrollviewEventType.scrollToBottom then
           --print("滚动到底部噢")
           sender.img_top:setVisible(true)
           sender.img_down:setVisible(false)
        elseif eventType==ccui.ScrollviewEventType.scrollToTop then
            --print("滚动到顶部噢")
            sender.img_top:setVisible(false)
            sender.img_down:setVisible(true)
        elseif eventType== ccui.ScrollviewEventType.scrolling then
            --print("滚动中噢")
            sender.img_top:setVisible(true)
            sender.img_down:setVisible(true)
            
            local event = cc.EventCustom:new("hideRuleTip")
            event._userdata = self.roomInfo
            cc.Director:getInstance():getEventDispatcher():dispatchEvent(event)
        
        end
    end
    scroll:addEventListener(scrollviewEvent)

end

--创建列表item
function HelpOpenLayer:createRecordItem(type,data)
    print("-------------HelpOpenLayer:createRecordItem------------------")
    local RecordItem = nil
    if type == "running" then
        RecordItem = require("hall/FriendRoom/FriendHelpOpen/OnTheMarch").create()
    elseif type == "record" then
        RecordItem = require("hall/FriendRoom/FriendHelpOpen/HelpOpenRecord").create()
    end

    if data ~= nil then
        RecordItem.friendGameId = data.friendGameId
        RecordItem.createTime = data.createTime
    end
    RecordItem:setVisible(false)
    self:setItemData(RecordItem,data)
    return RecordItem
end

--创建列表item
function HelpOpenLayer:setItemData(item,data)
    if item == nil or data == nil or next(data) == nil then
        return 
    end

    local roomInfo = {}
    roomInfo.friendGameId = data.friendGameId
    roomInfo.createTime = data.createTime
    roomInfo.duration = FishGF.changeRoomData("roomDurationType",data.roomDurationType).timeStr
    roomInfo.friendRoomNo = data.friendRoomNo
    roomInfo.playerCount = FishGF.changeRoomData("roomPeopleCountType",data.roomPeopleCountType).count
    roomInfo.roomPropType = data.roomPropType
    item:setInfo(roomInfo)

    local playerList = data.playerList
    for k,val in pairs(playerList) do
        self:setItemPlayerData(item,val)
    end
end

--设置item中的玩家数据
function HelpOpenLayer:setItemPlayerData(item,data)
    local state = data.state
    if state == 2 then
        item:playerExitRoom(data.playerId)
    else
        local info = {}
        info.chairId = data.chairId
        info.path = data.headPath
        info.playerId = data.playerId
        info.name = data.nickName
        info.score = data.score
        info.isReady =false
        if state == 1 then
            info.isReady = true
        end
        item:playerJoinRoom(info)
    end 
end

--设置列表网络数据
function HelpOpenLayer:setListData(type, data )
    print("----HelpOpenLayer---setListData------")
    if self.listData[type] == nil then
        self.listData[type] = {}
    end
    self.listData[type].data = data
end

--得到列表
function HelpOpenLayer:getDataList( type )
    local RecordList = self.listData[type].data
    local tempRecord = {}
    for i,val in ipairs(RecordList) do
        if val ~= nil then
            table.insert( tempRecord,val )
        end
    end
    return tempRecord
end

--设置列表中玩家数据
function HelpOpenLayer:setListPlayerData(type, data )
    print("----HelpOpenLayer---setListPlayerData------")
    if self.listData[type] == nil then
        self.listData[type] = {}
    end

    for k,v in pairs(self.listData[type].data) do
        if v.playerId == data.playerId then
            v = data
        end
    end

end

--改变列表item的状态  State:0 待定  1 已用  2 废弃
function HelpOpenLayer:changeItemState( type ,oldState,newState)
    local ListView = self.listData[type].ItemList
    if ListView == nil then
        return 
    end
    for k,v in pairs(ListView) do
        if v.mState == oldState then
            v.mState = newState
        end
    end
end

--得到列表item
function HelpOpenLayer:getItemById( type ,friendGameId)
    local ListView = self.listData[type].ItemList
    if ListView == nil then
        return nil
    end
    for k,v in pairs(ListView) do
        if v.friendGameId == friendGameId then
            return v
        end
    end

    for k,v in pairs(ListView) do
        if v.mState == 2 then
            return v
        end
    end

    return nil
end

--开始加载列表
function HelpOpenLayer:loadList( type )
    print("-------loadUnreadRecord------")
    local RecordList = self:getDataList(type)
    self:changeItemState(type,1,0)

    FishGF.waitNetManager(true,nil,"loadList"..type)
    local index = 1
    self.startload = true
    local function loadscheduler()
        local val  = RecordList[index]
        if val == nil then
            if index >= #RecordList then
                self:endScheduler(type)
                return
            end
            index = index + 1
            return
        end
        local friendGameId = val.friendGameId
        local RecordItem = self:getItemById(type, friendGameId )
        if RecordItem == nil then
            -- 创建新的
            RecordItem = self:createRecordItem(type,val)
            self.listData[type].scroll:addChild( RecordItem)
            -- 加入视图列表
            table.insert(self.listData[type].ItemList,RecordItem)
        else
            -- 有更新数据
            self:setItemData(RecordItem,val)
        end
        RecordItem.mState = 1

        if index >= #RecordList then
            self:endScheduler(type)
            return
        end
        index = index +1
    end
    self.listData[type].schedulerID = cc.Director:getInstance():getScheduler():scheduleScriptFunc(loadscheduler, 0.02, false);

end

--隐藏多余的item
function HelpOpenLayer:clearMoreItem( type )
    self:changeItemState(type,0,2)
end

--移除单个item
function HelpOpenLayer:removeItemByGameId(type, friendGameId )
    local RecordList = self.listData[type].data
    local tempList = {}
    for i,v in ipairs(RecordList) do
        if v.friendGameId ~= friendGameId then
            table.insert( tempList, v )
        end
    end
    self.listData[type].data = tempList

    local RecordItem = self:getItemById(type, friendGameId )
    RecordItem.mState = 2
    self:updateListPos(type)
end

--结束定时器
function HelpOpenLayer:endScheduler( type )
    self:clearMoreItem(type)
    if not self:isVisible() then
        self:showLayer(type)
    end
    self:closeAllSchedule(type)

    if next(self.playerChangeNetData) ~= nil then
        for i,v in ipairs(self.playerChangeNetData) do
            self:PlayerStateChange(v)
        end
    end

    self:updateListPos(type)
end

function HelpOpenLayer:closeAllSchedule(type)
    if type ~= nil then
        FishGF.waitNetManager(false,nil,"loadList"..type)
    end
    self.startload = false
    for k,v in pairs(self.listData) do
        local schedulerID = v.schedulerID
        if schedulerID ~= nil then
            cc.Director:getInstance():getScheduler():unscheduleScriptEntry(schedulerID )
            self.listData[k].schedulerID = nil
        end
    end

end

--更新列表
function HelpOpenLayer:updateListPos( type )
    if self.listData[type] == nil then
        return 
    end

    local ItemList = self.listData[type].ItemList
    local tempList = {}
    for k,v in pairs(ItemList) do
        if v.mState == 1 then
            v:setVisible(true)
            table.insert( tempList, v )
        else
            v:setVisible(false)
        end
    end

    FishGF.sortByKey(tempList,"createTime",0)

    local RecordCount = #tempList
    local cellW = self.listData[type].cellW
    local cellH = self.listData[type].cellH
    local scroll = self.listData[type].scroll
    local showCount = self.listData[type].showCount

    local sizwHeight = RecordCount*cellH + self.topDis*2
    scroll:setInnerContainerSize(cc.size(cellW, sizwHeight))
    local size = scroll:getContentSize()
    local topPos = 0
    if RecordCount >= showCount then
        topPos = sizwHeight - self.topDis - cellH/2
    else
        topPos = size.height - self.topDis - cellH/2
    end

    for i=1,#tempList do
        local item = tempList[i]
        item:setPositionX(cellW/2)
        item:setPositionY(topPos - (i - 1)*cellH)
    end

end

--判断是否更新数据
function HelpOpenLayer:isUpdateNetData()
    if self.curType == nil or (not self:isVisible()) then
        return 
    end
    print("---------send update -----------curType="..self.curType)
    self:sendNetData(self.curType)

end

--更新数据
function HelpOpenLayer:sendNetData(type)
    if type == nil then
        return 
    end

    if type == "running" then
        FishGI.FriendRoomManage:sendFriendRefreshRunningList()
    elseif type == "record" then
        FishGI.FriendRoomManage:sendFriendRefreshHelpOpenList()
    elseif type == "StartMonitor" then
        FishGI.FriendRoomManage:sendStartMonitorRunningList()
    elseif type == "CancelMonitor" then
        FishGI.FriendRoomManage:sendCancelMonitorRunningList()
    end

    --self:netDat(type)
end

--正在进行的代开列表
function HelpOpenLayer:FriendRefreshRunningList(data)
    data = data._userdata
    self:setListData("running",data.list)
    self:loadList("running")
end

--历史代开列表
function HelpOpenLayer:FriendRefreshHelpOpenList(data)
    data = data._userdata
    self:setListData("record",data.list)
    self:loadList("record")
end

--玩家状态改变
function HelpOpenLayer:FriendPlayerStateChange(data)
    data = data._userdata
    data.changeType = 0
    if self.startload then
        table.insert( self.playerChangeNetData,data)
        return 
    end
    self:PlayerStateChange(data)
end

--代开游戏开始
function HelpOpenLayer:FriendRunningStart(data)
    data = data._userdata
    data.changeType = 1
    if self.startload then
        table.insert( self.playerChangeNetData,data)
        return 
    end
    self:PlayerStateChange(data)
end

--确认是否解散
function HelpOpenLayer:FriendDissolveRoom(data)
    data = data._userdata
    data.changeType = 2
    if self.startload then
        table.insert( self.playerChangeNetData,data)
        return 
    end
    self:PlayerStateChange(data)

end


--玩家状态改变
function HelpOpenLayer:PlayerStateChange(data)
    if data == nil or data.changeType == nil then
        return 
    end
    local changeType = data.changeType  --0.玩家状态改变  1.游戏开始  2.游戏解散
    local friendGameId = data.friendGameId
    --先修改数据
    if changeType == 1 then
        self:removeItemByGameId("running",friendGameId)
    elseif changeType == 2 then
        local errorCode = data.errorCode
        local str = nil
        if errorCode == 1 then
            str = FishGF.getChByIndex(800000361)    --"房间不存在"
        elseif errorCode == 2 then
            str = FishGF.getChByIndex(800000302)    --游戏已经开始
        elseif errorCode == 3 then
            str = FishGF.getChByIndex(800000362)    --"您不是房主，不能解散房间"            
        end
        if str ~= nil then
            FishGF.showToast(str)
        end
        if errorCode ~= 3 then
            self:removeItemByGameId("running",friendGameId)
        end
        
    elseif changeType == 0 then
        local item = self:getItemById("running",friendGameId)
        if item == nil or item.mState == 2 then
            return 
        end
        local playerData = data.playerData
        self:setListPlayerData("running", playerData )
        self:setItemPlayerData(item,playerData)
    end
    
end






--测试接口
function HelpOpenLayer:netDat(type)
    local key = ""
    if type == "running" then
        key = "MSGS2CFriendRefreshRunningList"
    elseif type == "record" then
        key = "MSGS2CFriendRefreshHelpOpenList"
    elseif type == "leave" then
        key = "MSGS2CFriendPlayerStateChange"
    elseif type == "CancelMonitor" then
    end

    if FishGI.count == nil then
        FishGI.count = 50
    end
    local data = {}
    if type == "StartMonitor" or type == "CancelMonitor" then
        return 
    elseif type == "running" then
        data.list = {}
        for i=1,FishGI.count do
            local val = {}
            val.friendGameId = i
            val.friendRoomNo = i+100
            local time = string.format( "%02d",i)
            val.createTime = "2017-10-25 09:"..time

            val.roomPropType = (i%2)
            val.roomPeopleCountType = ((i+ 1)%3)
            val.roomDurationType = ((i+ 2)%2)
            val.roomCardType = (i%2)
            val.playerList = {}

            for j=2,4 do
                local palyerData = {}
                palyerData.playerId = j
                palyerData.nickName = (i*100+j).."-"..j
                palyerData.headPath = "common/com_pic_photo_1.png"
                palyerData.state = 1
                palyerData.chairId = (j)
                table.insert( val.playerList, palyerData)
            end
            table.insert( data.list, val )
        end
    elseif type == "record" then
        data.list = {}
        for i=1,FishGI.count do
            local val = {}
            val.friendGameId = i
            val.friendRoomNo = i+100
            local time = string.format( "%02d",i)
            val.createTime = "2017-10-25 09:"..time

            val.roomPropType = (i%2)
            val.roomPeopleCountType = ((i+ 1)%3)
            val.roomDurationType = ((i+ 2)%2)
            val.roomCardType = (i%2)
            val.playerList = {}

            for j=2,4 do
                local palyerData = {}
                palyerData.playerId = i*100+j
                palyerData.nickName = (i*100+j).."-"..j
                palyerData.headPath = "common/com_pic_photo_1.png"
                palyerData.state = 1
                palyerData.score = 10000+i*100+j
                palyerData.chairId = (j)
                table.insert( val.playerList, palyerData)
            end
            table.insert( data.list, val )
        end
    elseif type == "leave" then
        data.friendGameId = FishGI.count
        local palyerData = {}
        palyerData.playerId = 2
        palyerData.nickName = "efsfsff"
        palyerData.headPath = ""
        palyerData.state = 2
        palyerData.score = 0
        palyerData.chairId = 2
        data.playerData = palyerData
    elseif type == "CancelMonitor" then

    end


    --FishGI.count = FishGI.count + 1
    FishGI.FriendRoomManage:OnJMsg(key,data)
end

return HelpOpenLayer;