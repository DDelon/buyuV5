
local UnlockCannonItem = class("UnlockCannonItem", cc.load("mvc").ViewBase)

UnlockCannonItem.AUTO_RESOLUTION   = 0
UnlockCannonItem.RESOURCE_FILENAME = "ui/battle/unlockcannon/uiunlockcannonitem"
UnlockCannonItem.RESOURCE_BINDING  = {  
    ["panel"]               = { ["varname"] = "panel" },
    
    ["bmf_cannon_num"]      = { ["varname"] = "bmf_cannon_num" },
    
    ["spr_base"]            = { ["varname"] = "spr_base" },
    ["spr_gun"]             = { ["varname"] = "spr_gun" },

    ["layer_lock_bg"]       = { ["varname"] = "layer_lock_bg" },
    ["spr_lock"]            = { ["varname"] = "spr_lock" },

    ["image_word_bg"]       = { ["varname"] = "image_word_bg" },
    ["spr_preseng_wh_num"]  = { ["varname"] = "spr_preseng_wh_num" },
    ["bmf_present_num"]     = { ["varname"] = "bmf_present_num" },
    
    ["btn_lock"]            = { ["varname"] = "btn_lock" ,      ["events"]={["event"]="click",["method"]="onClickLock"}},
    ["spr_jscg"]            = { ["varname"] = "spr_jscg" },
    ["node_unlock"]         = { ["varname"] = "node_unlock" },
    ["spr_wh_num"]          = { ["varname"] = "spr_wh_num" },
    ["bmf_num"]             = { ["varname"] = "bmf_num" },
}

function UnlockCannonItem:onCreate(...)
    --初始化
    self:init()
    -- 初始化View
    self:initView()
end

function UnlockCannonItem:init()
end

function UnlockCannonItem:initView()
end

function UnlockCannonItem:onClickLock( sender )
    if self.isCurLockRate then
        self.parent_:buttonClicked("UnlockCannonItem", self:getTag())
    end
end

function UnlockCannonItem:setVip(vip)
    self.playerVIP = vip
    local gunFile = "battle/cannon/"..FishGI.GameTableData:getGunOutlookTableByVip(self.playerVIP).cannon_img
    self.spr_gun:initWithFile(gunFile)
    local baseFile = "battle/cannon/"..FishGI.GameTableData:getGunOutlookTableByVip(self.playerVIP).base_img
    self.spr_base:initWithFile(baseFile)
end

function UnlockCannonItem:setMaxGunRate(gunRate)
    self.maxGunRate = gunRate
end

function UnlockCannonItem:getAimCrystal()
    return self.gemCount
end

function UnlockCannonItem:setItemData(gunRate, gemCount, awardCount, isCurLockRate)
    if isCurLockRate == nil then
        isCurLockRate = false
    end
    self.gunRate = gunRate
    self.gemCount = gemCount
    self.awardCount = awardCount
    self.isCurLockRate = isCurLockRate
    self.bmf_cannon_num:setString(tonumber(gunRate))
    self.bmf_present_num:setString(tonumber(awardCount))
    self.bmf_num:setString(tonumber(gemCount))
    if gunRate > self.maxGunRate then
        self.layer_lock_bg:setVisible(true)
        self.spr_lock:setVisible(true)
        self.node_unlock:setVisible(true)
        self.spr_jscg:setVisible(false)
        if isCurLockRate then
            self.bmf_present_num:setVisible(true)
            self.spr_preseng_wh_num:setVisible(false)
            self.bmf_num:setVisible(true)
            self.spr_wh_num:setVisible(false)
        else
            self.bmf_present_num:setVisible(false)
            self.spr_preseng_wh_num:setVisible(true)
            self.bmf_num:setVisible(false)
            self.spr_wh_num:setVisible(true)
        end
    else
        self.layer_lock_bg:setVisible(false)
        self.spr_lock:setVisible(false)
        self.bmf_present_num:setVisible(true)
        self.spr_preseng_wh_num:setVisible(false)
        self.node_unlock:setVisible(false)
        self.spr_jscg:setVisible(true)
    end
end

return UnlockCannonItem