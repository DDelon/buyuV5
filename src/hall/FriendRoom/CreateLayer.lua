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


    ["btn_help_1"]     = { ["varname"] = "btn_help_1" ,         ["events"]={["event"]="click_color",["method"]="onClickhelp_1"}},
    ["btn_help_2"]     = { ["varname"] = "btn_help_2" ,         ["events"]={["event"]="click_color",["method"]="onClickhelp_2"}},

    ["btn_cradtype_1"]     = { ["varname"] = "btn_cradtype_1" ,         ["events"]={["event"]="click_color",["method"]="onClickcradtype_1"}},
    ["btn_cradtype_2"]     = { ["varname"] = "btn_cradtype_2" ,         ["events"]={["event"]="click_color",["method"]="onClickcradtype_2"}},

    ["fnt_count"]      = { ["varname"] = "fnt_count" }, 
}

CreateLayer.ch_list  = {    
    ["text_1"]         = { ["str"] = FishGF.getChByIndex(800000322) }, 
    ["text_2"]         = { ["str"] = FishGF.getChByIndex(800000323)  }, 
    ["text_3"]         = { ["str"] = FishGF.getChByIndex(800000324) }, 
    ["text_4"]         = { ["str"] = FishGF.getChByIndex(800000325) }, 
    ["text_5"]         = { ["str"] = FishGF.getChByIndex(800000326) }, 
    ["text_6"]         = { ["str"] = FishGF.getChByIndex(800000327)  }, 
    ["text_7"]         = { ["str"] = FishGF.getChByIndex(800000328)  }, 

    ["text_8"]         = { ["str"] = FishGF.getChByIndex(800000355)  }, 
    ["text_9"]         = { ["str"] = FishGF.getChByIndex(800000356)  }, 
    ["text_10"]         = { ["str"] = FishGF.getChByIndex(800000355)  }, 
    ["text_11"]         = { ["str"] = FishGF.getChByIndex(800000356)  }, 

    ["text_word_count"]         = { ["str"] = FishGF.getChByIndex(800000329)  }, 
    ["text_cell"]         = { ["str"] = FishGF.getChByIndex(800000330)  },     
}

function CreateLayer:onCreate( ... )
    self:openTouchEventListener()

    for k,v in pairs(self.ch_list) do
        local node = self:child(k)
        if node ~= nil then
            node:setString(v.str)
        end
    end

    self.propList = {}
    self.createData = {}
    self:showType()

    self:setIsCanHelpOpen(true)
end

function CreateLayer:showLayer(roomPropType,roomPeopleCountType,roomDurationType,agent,roomCardType)
    self.super.showLayer(self)
    self:showType(roomPropType,roomPeopleCountType,roomDurationType,agent,roomCardType)
end

function CreateLayer:showType(roomPropType,roomPeopleCountType,roomDurationType,agent,roomCardType)
    if roomPropType == nil then --默认道具类型,0:不带怼人道具，1:带怼人道具
        roomPropType = 1
    end
    if roomPeopleCountType == nil then --默认人数类型，0:2人，1:3人，2:4人
        roomPeopleCountType = 0
    end
    if roomDurationType == nil then --默认时长类型,0:8分钟，1,24分钟
        roomDurationType = 0
    end
    if agent == nil then --默是否代开，如果代开true，roomCardType必须=1，即使用平台房卡
        agent = false
    end
    if roomCardType == nil then --默认房卡类型 0,游戏内房卡，1，平台房卡
        roomCardType = 0
    end

    self:buttonClicked("roomPropType",roomPropType)
    self:buttonClicked("roomPeopleCountType",roomPeopleCountType)
    self:buttonClicked("roomDurationType",roomDurationType)
    self:buttonClicked("agent",agent)
    self:buttonClicked("roomCardType",roomCardType)

end

function CreateLayer:setIsCanHelpOpen( isCanHelpOpen)

    local dis = 60
    local showCount = 5
    if not isCanHelpOpen then
        dis = 80
        showCount = 3
    end

    local firstPosY = self:child("node_1"):getPositionY()
    for i=1,5 do
        local node = self:child("node_"..i)
        if i <= showCount then
            node:setPositionY(firstPosY - (i - 1)*dis)
            node:setVisible(true)
        else
            node:setVisible(false)  
        end
    end

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
    self:buttonClicked("roomPropType",1)
end
function CreateLayer:onClickprop_2( sender )
    self:buttonClicked("roomPropType",0)
end

function CreateLayer:onClickcount_1( sender )
    self:buttonClicked("roomPeopleCountType",0)
end
function CreateLayer:onClickcount_2( sender )
    self:buttonClicked("roomPeopleCountType",1)
end
function CreateLayer:onClickcount_3( sender )
    self:buttonClicked("roomPeopleCountType",2)
end

function CreateLayer:onClicktime_1( sender )
    self:buttonClicked("roomDurationType",0)
end
function CreateLayer:onClicktime_2( sender )
    self:buttonClicked("roomDurationType",1)
end

function CreateLayer:onClickhelp_1( sender )
    self:buttonClicked("agent",true)
end
function CreateLayer:onClickhelp_2( sender )
    self:buttonClicked("agent",false)
end

function CreateLayer:onClickcradtype_1( sender )
    self:buttonClicked("roomCardType",1)
end
function CreateLayer:onClickcradtype_2( sender )
    self:buttonClicked("roomCardType",0)
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
    if viewTag == "roomPropType" then 
        self:setBtnState(self.btn_prop_1,false)
        self:setBtnState(self.btn_prop_2,false)
        if btnTag == 1 then
            self:setBtnState(self.btn_prop_1,true)
        elseif btnTag == 0 then 
            self:setBtnState(self.btn_prop_2,true)
        end
    elseif viewTag == "roomPeopleCountType" then 
        self:setBtnState(self.btn_count_1,false)
        self:setBtnState(self.btn_count_2,false)
        self:setBtnState(self.btn_count_3,false)
        if btnTag == 0 then
            self:setBtnState(self.btn_count_1,true)
        elseif btnTag == 1 then 
            self:setBtnState(self.btn_count_2,true)
        elseif btnTag == 2 then 
            self:setBtnState(self.btn_count_3,true)
        end
    elseif viewTag == "roomDurationType" then 
        self:setBtnState(self.btn_time_1,false)
        self:setBtnState(self.btn_time_2,false)
        if btnTag == 0 then
            self:setBtnState(self.btn_time_1,true)
        elseif btnTag == 1 then 
            self:setBtnState(self.btn_time_2,true)
        end
    elseif viewTag == "agent" then 
        self:setBtnState(self.btn_help_1,false)
        self:setBtnState(self.btn_help_2,false)
        if btnTag == true then
            self:setBtnState(self.btn_help_1,true)
        elseif btnTag == false then 
            self:setBtnState(self.btn_help_2,true)
        end
    elseif viewTag == "roomCardType" then 
        self:setBtnState(self.btn_cradtype_1,false)
        self:setBtnState(self.btn_cradtype_2,false)
        if btnTag == 1 then
            self:setBtnState(self.btn_cradtype_1,true)
        elseif btnTag == 0 then 
            self:setBtnState(self.btn_cradtype_2,true)
        end
    end
    
end

function CreateLayer:buttonClicked(viewTag, btnTag)
    if viewTag == nil then
        print("buttonClicked---viewTag== nil --")
        return 
    end
    if btnTag ~= nil then
        print("buttonClicked----viewTag = ["..viewTag.."]-----btnTag = "..tostring(btnTag))        
    end
    
    if viewTag == "sure" then 
        local newData = FishGF.changeRoomData("roomDurationType",self.createData.roomDurationType)
        FishGI.hallScene.uiFriendRoom:setChooseType(1,newData.cardCount)
        self:hideLayer()
    elseif viewTag == "roomPropType" then 
        self.createData[viewTag] = btnTag
    elseif viewTag == "roomPeopleCountType" then 
        self.createData[viewTag] = btnTag
    elseif viewTag == "roomDurationType" then 
        self.createData[viewTag] = btnTag
    elseif viewTag == "agent" then 
        self.createData[viewTag] = btnTag
        self:buttonClicked("roomCardType", 1)
    elseif viewTag == "roomCardType" then --房卡类型 roomCardType:0,游戏内房卡，1，平台房卡
        if self.createData.agent == true and btnTag == 0 then
            FishGF.showSystemTip(nil, 800000181, 3)
            return 
        end
        self.createData[viewTag] = btnTag
    end

    self:setBtnChoose(viewTag, btnTag)

end

return CreateLayer;