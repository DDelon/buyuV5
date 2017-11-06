local CreateLayer = class("CreateLayer", cc.load("mvc").ViewBase)

CreateLayer.AUTO_RESOLUTION   = false
CreateLayer.RESOURCE_FILENAME = "ui/hall/friend/uicreatelayer"
CreateLayer.RESOURCE_BINDING  = {    
    ["panel"]         = { ["varname"] = "panel" }, 
    ["btn_sure"]        = { ["varname"] = "btn_sure" ,         ["events"]={["event"]="click",["method"]="onClicksure"}},   
    ["btn_close"]       = { ["varname"] = "btn_close" ,        ["events"]={["event"]="click",["method"]="onClickclose"}}, 

    ["btn_prop_1"]     = { ["varname"] = "btn_prop_1" ,         ["events"]={["event"]="click_color",["method"]="onClickprop_1"}},
    ["btn_prop_2"]     = { ["varname"] = "btn_prop_2" ,         ["events"]={["event"]="click_color",["method"]="onClickprop_2"}},

    ["btn_count_1"]     = { ["varname"] = "btn_count_1" ,         ["events"]={["event"]="click_color",["method"]="onClickcount_1"}},
    ["btn_count_2"]     = { ["varname"] = "btn_count_2" ,         ["events"]={["event"]="click_color",["method"]="onClickcount_2"}},
    ["btn_count_3"]     = { ["varname"] = "btn_count_3" ,         ["events"]={["event"]="click_color",["method"]="onClickcount_3"}},

    ["btn_time_1"]     = { ["varname"] = "btn_time_1" ,         ["events"]={["event"]="click_color",["method"]="onClicktime_1"}},
    ["btn_time_2"]     = { ["varname"] = "btn_time_2" ,         ["events"]={["event"]="click_color",["method"]="onClicktime_2"}},

    ["text_1"]         = { ["varname"] = "text_1" }, 
    ["text_2"]         = { ["varname"] = "text_2" }, 
    ["text_3"]         = { ["varname"] = "text_3" }, 
    ["text_4"]         = { ["varname"] = "text_4" }, 
    ["text_5"]         = { ["varname"] = "text_5" }, 
    ["text_6"]         = { ["varname"] = "text_6" }, 
    ["text_7"]         = { ["varname"] = "text_7" }, 
    ["text_8"]         = { ["varname"] = "text_8" }, 
    ["text_cell"]         = { ["varname"] = "text_cell" }, 
    ["fnt_count"]         = { ["varname"] = "fnt_count" }, 
}

function CreateLayer:onCreate( ... )
    self:openTouchEventListener()

    for i=1,8 do
        self["text_"..i]:setString(FishGF.getChByIndex(800000321 + i))
    end
    self.text_cell:setString(FishGF.getChByIndex(800000330))

    self.propList = {}
    self.createData = {}
    self:onClickprop_1(self)
    self:onClickcount_1(self)
    self:onClicktime_1(self)
end

function CreateLayer:showLayer()
    self.super.showLayer(self)
    self:onClickprop_1(self)
    self:onClickcount_3(self)
    self:onClicktime_1(self)    
end

function CreateLayer:onTouchBegan(touch, event)
    if not self:isVisible() then
         return false
    end
    return true
end

--更新道具数量
function CreateLayer:updatePropData(propId,showCount)
    self.propList[propId] = showCount
    
    local allCount = 0
    for k,v in pairs(self.propList) do
        allCount = allCount + v
    end
    
    self.fnt_count:setString(allCount)

end

function CreateLayer:onClickclose( sender )
    print("onClickclose")
    self:hideLayer()
end

function CreateLayer:onClicksure( sender )
    self:buttonClicked("sure")
end

function CreateLayer:onClickprop_1( sender )
    self:buttonClicked("prop",1)
end
function CreateLayer:onClickprop_2( sender )
    self:buttonClicked("prop",2)
end

function CreateLayer:onClickcount_1( sender )
    self:buttonClicked("count",1)
end
function CreateLayer:onClickcount_2( sender )
    self:buttonClicked("count",2)
end
function CreateLayer:onClickcount_3( sender )
    self:buttonClicked("count",3)
end

function CreateLayer:onClicktime_1( sender )
    self:buttonClicked("time",1)
end
function CreateLayer:onClicktime_2( sender )
    self:buttonClicked("time",2)
end

--按键状态设置
function CreateLayer:setBtnState( btn,isChoose )
    local dot = btn:getChildByName("spr_dot")
    if dot ~= nil and isChoose ~= nil then
        dot:setVisible(isChoose)
    end
    btn:setTouchEnabled(not isChoose)
end

--设置按键的选择
function CreateLayer:setBtnChoose(viewTag, btnTag)
    if viewTag == "prop" then 
        self:setBtnState(self.btn_prop_1,false)
        self:setBtnState(self.btn_prop_2,false)
        if btnTag == 1 then
            self:setBtnState(self.btn_prop_1,true)
        elseif btnTag == 2 then 
            self:setBtnState(self.btn_prop_2,true)
        end
    elseif viewTag == "count" then 
        self:setBtnState(self.btn_count_1,false)
        self:setBtnState(self.btn_count_2,false)
        self:setBtnState(self.btn_count_3,false)
        if btnTag == 1 then
            self:setBtnState(self.btn_count_1,true)
        elseif btnTag == 2 then 
            self:setBtnState(self.btn_count_2,true)
        elseif btnTag == 3 then 
            self:setBtnState(self.btn_count_3,true)
        end
    elseif viewTag == "time" then 
        self:setBtnState(self.btn_time_1,false)
        self:setBtnState(self.btn_time_2,false)
        if btnTag == 1 then
            self:setBtnState(self.btn_time_1,true)
        elseif btnTag == 2 then 
            self:setBtnState(self.btn_time_2,true)
        end

    end

end

function CreateLayer:buttonClicked(viewTag, btnTag)
    print("buttonClicked---viewTag="..viewTag)
    if btnTag ~= nil then
        print("buttonClicked---btnTag="..btnTag)
    end

    self:setBtnChoose(viewTag, btnTag)

    if viewTag == "sure" then 
        local newData = FishGF.changeRoomData("roomDurationType",self.createData.roomDurationType)
        FishGI.hallScene.uiFriendRoom:setChooseType(1,newData.cardCount)
        self:hideLayer()
    elseif viewTag == "prop" then 
        if btnTag == 1 then
            self.createData.roomPropType = 1
        elseif btnTag == 2 then 
            self.createData.roomPropType = 0
        end
    elseif viewTag == "count" then 
        if btnTag == 1 then
            self.createData.roomPeopleCountType = 0
        elseif btnTag == 2 then 
            self.createData.roomPeopleCountType = 1
        elseif btnTag == 3 then 
            self.createData.roomPeopleCountType = 2
        end
    elseif viewTag == "time" then 
        if btnTag == 1 then
            self.createData.roomDurationType = 0
        elseif btnTag == 2 then 
            self.createData.roomDurationType = 1
        end

    end

end

return CreateLayer;