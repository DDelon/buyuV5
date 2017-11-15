
local Feedback = class("Feedback", cc.load("mvc").ViewBase)

Feedback.AUTO_RESOLUTION   = false
Feedback.RESOURCE_FILENAME = "ui/hall/feedback/uifeedback"
Feedback.RESOURCE_BINDING  = {  
    ["panel"]               = { ["varname"] = "panel" },
    ["btn_close"]           = { ["varname"] = "btn_close", ["events"]={["event"]="click",["method"]="onClickclose"} }, 

    ["tf_content"]          = { ["varname"] = "tf_content" },
    
    ["btn_get_pic"]         = { ["varname"] = "btn_get_pic", ["events"]={["event"]="click",["method"]="onClickGetPic"} }, 
    ["img_pic_1"]           = { ["varname"] = "img_pic_1" }, 
    ["img_pic_2"]           = { ["varname"] = "img_pic_2" }, 
    ["btn_delete_pic_1"]    = { ["varname"] = "btn_delete_pic_1", ["events"]={["event"]="click",["method"]="onClickPicDelete"} },
    ["btn_delete_pic_2"]    = { ["varname"] = "btn_delete_pic_2", ["events"]={["event"]="click",["method"]="onClickPicDelete"} },

    ["tf_phonenum"]         = { ["varname"] = "tf_phonenum" }, 
    
    ["btn_commit"]          = { ["varname"] = "btn_commit", ["events"]={["event"]="click",["method"]="onClickCommit"} }, 
}

local picLimitCount = 2        -- 玩家上传图片限制数量

function Feedback:onCreate( ... )
    --初始化
    self:init()
    self:initView()
end

function Feedback:init()
    self.panel:setSwallowTouches(false)
    self:openTouchEventListener()
    self.addPhotoCount = 0           -- 添加的图片数量
    self.scuccesCount = 0            -- 图片上传成功次数
    self.vecPicFullPath = {}
    self.vecPhotoUrl = {}
end

function Feedback:initView()
    self:initWinEditBox("tf_phonenum")
    self.tf_content:setPlaceHolder(FishGF.getChByIndex(800000368))
    self.img_pic_1:setVisible(false)
    self.img_pic_2:setVisible(false)
    self.btn_delete_pic_1:setTag(1)
    self.btn_delete_pic_2:setTag(2)
end

function Feedback:onTouchBegan(touch, event)
    if self:isVisible() then
         return true  
    end

    return false
end

function Feedback:onClickclose( sender )
    self:hideLayer() 
end

function Feedback:onClickGetPic( sender )
    local function funPicPicked( picfullpath )
        if self.addPhotoCount < picLimitCount then
            self.addPhotoCount = self.addPhotoCount + 1
            self.vecPicFullPath[self.addPhotoCount] = picfullpath
            self["img_pic_"..self.addPhotoCount]:loadTexture(self.vecPicFullPath[self.addPhotoCount])
            self["img_pic_"..self.addPhotoCount]:setVisible(true)
            if self.addPhotoCount < picLimitCount then
                self.btn_get_pic:setVisible(true)
                self.btn_get_pic:setPosition(cc.p(self["img_pic_"..self.addPhotoCount+1]:getPosition()))
            else
                self.btn_get_pic:setVisible(false)
            end
            FishGF.waitNetManager(true,nil,"UploadCloudimg")
            -- 调用上传图片接口
            FishGI.Dapi:UploadCloudimg(picfullpath,function (result)
                FishGF.waitNetManager(false,nil,"UploadCloudimg")
                -- 上传图片回调
                if result and result.status == 0 then
                    -- 图片上传反馈的URL
                     local photoUrl = result.url
                     self.scuccesCount = self.scuccesCount +1
                     self.vecPhotoUrl[self.scuccesCount] = photoUrl
                else
                    --GameApp:dispatchEvent(gg.Event.SHOW_TOAST,"上传失败,请重新添加！")
                    self:onClickPicDelete(self["btn_delete_pic_"..self.addPhotoCount])
                end
            end)
        end
    end
    -- 设备上传图片接口
    device.callImagePicker("library", funPicPicked, false)
end

function Feedback:onClickPicDelete( sender )
    local iBtnTag = sender:getTag()
    if self.addPhotoCount > 0 then
        self["img_pic_"..self.addPhotoCount]:setVisible(false)
        for i=iBtnTag,self.addPhotoCount do
            if i < self.addPhotoCount then
                self.vecPicFullPath[i] = self.vecPicFullPath[i+1]
                self["img_pic_"..i]:loadTexture(self.vecPicFullPath[i])
            else
                self.vecPicFullPath[i] = nil
            end
        end
        self.btn_get_pic:setVisible(true)
        self.btn_get_pic:setPosition(cc.p(self["img_pic_"..self.addPhotoCount]:getPosition()))
        self.addPhotoCount = self.addPhotoCount - 1
    end
    if self.scuccesCount > 0 then
        for i=iBtnTag,self.scuccesCount do
            if i < self.scuccesCount then
                self.vecPhotoUrl[i] = self.vecPhotoUrl[i+1]
            else
                self.vecPhotoUrl[i] = nil
            end
        end
        self.scuccesCount = self.scuccesCount - 1
    end
end

function Feedback:onClickCommit( sender )
    local strFeedback = self.tf_content:getString()
    if #strFeedback == 0 and self.scuccesCount == 0 then
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000365))
        return
    end
    local strPhoneNum = self.tf_phonenum:getString()
    -- 电话号可填可不填,填写的话判断电话号码格式
    if #strPhoneNum > 0 and #strPhoneNum ~= 11 then
        FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,FishGF.getChByIndex(800000366))
        return
    end
    FishGF.waitNetManager(true,nil,"FeedBack")
    local strPhoneUrls = ""
    for i=1,self.scuccesCount do
        strPhoneUrls = strPhoneUrls.."||"..self.vecPhotoUrl[i]
    end 
    FishGI.Dapi:FeedBack(Helper.GetDeviceCode(), GAME_ID, FishGI.hallScene.net.roomId, strFeedback, strPhoneUrls, strPhoneNum, 
        function( result )
            FishGF.waitNetManager(false,nil,"FeedBack")
            local strMsg = result.msg
            if result and result.status == 0 then
                strMsg = FishGF.getChByIndex(800000367)
            end
            FishGF.showMessageLayer(FishCD.MODE_MIDDLE_OK_ONLY,strMsg)
        end)
end

return Feedback