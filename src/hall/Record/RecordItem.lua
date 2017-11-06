
local RecordItem = class("RecordItem", cc.load("mvc").ViewBase)

RecordItem.AUTO_RESOLUTION   = false
RecordItem.RESOURCE_FILENAME = "ui/hall/record/uirecorditem"
RecordItem.RESOURCE_BINDING  = {  
       ["panel"]       = { ["varname"] = "panel" },  
       ["img_bg"]      = { ["varname"] = "img_bg" },   
       
       ["text_time"]   = { ["varname"] = "text_time" },   
       ["text_roomno"] = { ["varname"] = "text_roomno" }, 
       ["text_word_room"]   = { ["varname"] = "text_word_room" },   
       ["text_word_time"] = { ["varname"] = "text_word_time" }, 

       ["node_1"] = { ["varname"] = "node_1" }, 
       ["node_2"]   = { ["varname"] = "node_2" },
       ["node_3"] = { ["varname"] = "node_3" }, 
       ["node_4"]   = { ["varname"] = "node_4" },

}

function RecordItem:onCreate(...)   
    self.text_word_room:setString(FishGF.getChByIndex(800000241)..FishGF.getChByIndex(800000218))
    self.text_word_time:setString(FishGF.getChByIndex(800000240)..FishGF.getChByIndex(800000218))

    for i=1,4 do
        local node = self["node_"..i]
        local text1 = node:getChildByName("text_word_1")
        local text2 = node:getChildByName("text_word_2")
        text1:setString(FishGF.getChByIndex(800000338)..FishGF.getChByIndex(800000218))
        text2:setString(FishGF.getChByIndex(800000339)..FishGF.getChByIndex(800000218))
    end

end

--设置数据
function RecordItem:setItemData(val)   
    if val == nil then
        return
    end
    self.creatorNickName = val.creatorNickName
    self.creatorPlayerId = val.creatorPlayerId
    self.recordData = val
    self.friendGameId = val.friendGameId
    self.text_roomno:setString(val.friendRoomNo)
    self.text_time:setString(val.time)

    local orderList = self.recordData.items
    if orderList == nil then
        return
    end
    print("_---------sortByKey---------------")
    FishGF.sortByKey(orderList,"order",1)

    for i=1,4 do
        local node = self["node_"..i]
        local text_score = node:getChildByName("text_score")
        local text_account = node:getChildByName("text_account")
        local spr_owner = node:getChildByName("spr_owner")
        local data = orderList[i]
        if data == nil then
            node:setVisible(false)
        else
            node:setVisible(true)
            text_score:setString(data.score)
            text_account:setString(data.nickName)
            if self.creatorPlayerId == data.playerId then
                spr_owner:setVisible(true)
            else
                spr_owner:setVisible(false)
            end
        end

    end

end

function RecordItem:getItemData(val)   
    return self.recordData
end

--设置一个玩家数据



return RecordItem;