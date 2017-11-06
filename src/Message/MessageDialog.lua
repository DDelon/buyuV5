
local MessageDialog = class("MessageDialog", cc.load("mvc").ViewBase)

MessageDialog.AUTO_RESOLUTION   = false
MessageDialog.RESOURCE_FILENAME = "ui/common/uiMessageDialog"
MessageDialog.RESOURCE_BINDING  = {  
    ["panel"]              = { ["varname"] = "panel" },
    
    ["node_middle"]        = { ["varname"] = "node_middle" },
    ["btn_middle_only_OK"] = { ["varname"] = "btn_middle_only_OK" , ["events"]={["event"]="click",["method"]="onClickCallback"}},  
    ["btn_middle_OK"]      = { ["varname"] = "btn_middle_OK" ,      ["events"]={["event"]="click",["method"]="onClickCallback"}},      
    ["btn_middle_CANCEL"]  = { ["varname"] = "btn_middle_CANCEL" ,  ["events"]={["event"]="click",["method"]="onClickCallback"}},  
    
    ["node_hook"]          = { ["varname"] = "node_hook" },
    ["btn_hook"]           = { ["varname"] = "btn_hook" ,           ["events"]={["event"]="click",["method"]="onClickCallback"}},  
    ["text_notice"]        = { ["varname"] = "text_notice" },  
    
    ["text_middle_data"]   = { ["varname"] = "text_middle_data" },    
    ["text_middle_desc"]   = { ["varname"] = "text_middle_desc" },    
       
    ["node_min"]           = { ["varname"] = "node_min" },
    ["btn_min_only_OK"]    = { ["varname"] = "btn_min_only_OK" ,    ["events"]={["event"]="click",["method"]="onClickCallback"}},  
    ["text_min_data"]      = { ["varname"] = "text_min_data" },    

}

function MessageDialog:onCreate(...)   
    self.panel:setSwallowTouches(false)
    self:openTouchEventListener()
    
    self.btn_hook:getChildByName("spr_hook"):setVisible(false)

    self.firstPos = {}
    self.firstPos.x = self.text_middle_data:getPositionX()
    self.firstPos.y = self.text_middle_data:getPositionY()
    self.textSize = self.text_middle_data:getContentSize()
    self.text_middle_data:ignoreContentAdaptWithSize(true)
    self.text_middle_data:setTextAreaSize({width = 0, height = 0})

    self.text_middle_desc:ignoreContentAdaptWithSize(true)
    self.text_middle_desc:setTextAreaSize({width = 0, height = 0})

end

function MessageDialog:initBtnShow()   
    self.node_middle:setVisible(false)
    self.node_min:setVisible(false)  

    self.btn_min_only_OK:setVisible(false)  
    self.btn_min_only_OK:setTag(0)

    self.btn_middle_only_OK:setVisible(false)  
    self.btn_middle_only_OK:setTag(1)

    self.btn_middle_OK:setVisible(false)  
    self.btn_middle_OK:setTag(2)
    self.btn_middle_CANCEL:setVisible(false)  
    self.btn_middle_CANCEL:setTag(3)

    self.node_hook:setVisible(false)  
    self.btn_hook:setTag(4)

end

function MessageDialog:setData(modeType ,strData, callBack,strHook) 
    self.modeType = modeType

    self:initBtnShow() 

    self.panel:setVisible(true)
    
    --tag值  0为小背景的确定，  1为中背景的确定，2为中背景的确定，3为中背景的取消, 4为勾选框
    if callBack ~= nil then
        self.btn_min_only_OK:addClickEventListener(callBack)
        self.btn_middle_only_OK:addClickEventListener(callBack)
        self.btn_middle_OK:addClickEventListener(callBack)
        self.btn_middle_CANCEL:addClickEventListener(callBack)
        self.btn_hook:addClickEventListener(callBack)
    end
   
    if modeType == 1 then     --FishCD.MODE_MIN_OK_ONLY
        self.node_min:setVisible(true) 
        self.btn_min_only_OK:setVisible(true)  
    elseif modeType == 2 then  --FishCD.MODE_MIDDLE_OK_ONLY
        self.node_middle:setVisible(true)
        self.btn_middle_only_OK:setVisible(true)     
    elseif modeType == 3 then --FishCD.MODE_MIDDLE_OK_CLOSE
        self.node_middle:setVisible(true)
        self.btn_middle_OK:setVisible(true)
        self.btn_middle_CANCEL:setVisible(true)
    elseif modeType == 4 then --FishCD.MODE_MIDDLE_OK_CLOSE_HOOK
        self.node_middle:setVisible(true)
        self.btn_middle_OK:setVisible(true)
        self.btn_middle_CANCEL:setVisible(true)
        self.node_hook:setVisible(true)
        self.btn_hook:getChildByName("spr_hook"):setVisible(false)
    end

    if strHook == nil then
        strHook = "本次登录不再提示！"
    end
    self.text_notice:setString(strHook) 
    self:setTextData(strData) 
end

function MessageDialog:onTouchBegan(touch, event)
    if not self:isVisible() then
         return false
    end
    return true
end

function MessageDialog:setTextData( str )
    local back = self:splitTextStr(str)
    self:updateText(self.text_middle_data,back[1],self.textSize.width)
    self:updateText(self.text_middle_desc,back[2],self.textSize.width)
    self.text_min_data:setString(back[1])   
    self:updateTextPos()
end

function MessageDialog:splitTextStr( str )
    local result = string.split(str,"$")
    return result
end

function MessageDialog:updateTextPos(  )
    local size1 = self.text_middle_data:getContentSize()
    local size2 = self.text_middle_desc:getContentSize()
    if size2.width > 0 then
        local newPosY1 = self.firstPos.y + self.textSize.height/2 - size1.height/2
        self.text_middle_data:setPositionY(newPosY1)
        local newPosY2 = self.firstPos.y + self.textSize.height/2 - size1.height - size2.height/2
        self.text_middle_desc:setPositionY(newPosY2)
    else
        self.text_middle_data:setPositionY(self.firstPos.y)
    end


end


--设置中提示界面的提示数据
function MessageDialog:updateText(text, str,sizeWidth )
    text:setTextAreaSize({width = 0, height = 0})
    text:setString(str)
    local size = text:getContentSize()
    if size.width > sizeWidth then
        text:setTextAreaSize({width = sizeWidth, height = 0})
        text:setTextHorizontalAlignment(0)
    else
        text:setTextHorizontalAlignment(1)
    end  
end

function MessageDialog:onClickCallback( sender )
    print("--MessageDialog:onClickCallback-")

    --tag值  0为小背景的确定，  1为中背景的确定，2为中背景的确定，3为中背景的取消
    local tag = sender:getTag()
    if tag == 0 or tag == 1 or tag == 2 or tag == 3 then
        self:hideLayer(true,false) 
    end

end

return MessageDialog;