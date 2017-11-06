local FriendGameData = class("FriendGameData", cc.load("mvc").ViewBase)

FriendGameData.AUTO_RESOLUTION   = true
FriendGameData.RESOURCE_FILENAME = "ui/battle/friend/uifriendgamedata"
FriendGameData.RESOURCE_BINDING  = {
    ["img_rank_my_bg"]      = { ["varname"] = "img_rank_my_bg" },
    ["node_rank_1"]         = { ["varname"] = "node_rank_1" },
    ["node_rank_2"]         = { ["varname"] = "node_rank_2" },
    ["node_rank_3"]         = { ["varname"] = "node_rank_3" },
    ["node_rank_4"]         = { ["varname"] = "node_rank_4" },
}

function FriendGameData:onCreate( ... )
    self:init()
    self:initView()
end

function FriendGameData:init()
    self:openTouchEventListener()
    self.node_rank_item = {}
    self.tListInfo = {}
    self.tSortList = {}
    self.iCount = 0
    self.bTimeoutAni = false
    self.lastFrameIndex = -1
    self.curFrameIndex = -1
end

function FriendGameData:onTouchBegan(touch, event)
    return false
end

function FriendGameData:initView()
    for i = 1, 4 do
        self.node_rank_item[i] = require("Game/Friend/FriendRankItem").new(self, self["node_rank_"..i])
        self.node_rank_item[i]:setRankIndex(i)
        self.node_rank_item[i]:setVisible(false)
    end
    self.img_rank_my_bg:setVisible(false)
end

function FriendGameData:setOwnerIndex(iOwnerIndex)
    self.iOwnerIndex = iOwnerIndex
end

function FriendGameData:setPlayerScore( iScore, iChairId )
    local bNew = false
    if self.tListInfo[iChairId] == nil then 
        self.iCount = self.iCount + 1
        self.node_rank_item[self.iCount]:setVisible(true)
        self.tListInfo[iChairId] = iScore
        table.insert( self.tSortList, iChairId )
        bNew = true
    else
        if self.tListInfo[iChairId] ~= iScore then 
            self.tListInfo[iChairId] = iScore
        else 
            return
        end
    end 
    -- 根据积分排序
    local iSortIndex = table.getn(self.tSortList)
    if not bNew then 
        for i = 1, table.getn(self.tSortList) do 
            if self.tSortList[i] == iChairId then 
                iSortIndex = i
                break
            end 
        end
    end 
    local iFind = table.getn(self.tSortList)
    for i, v in pairs(self.tSortList) do
        if iChairId == v then 
            --自己跳过
        elseif iScore > self.tListInfo[v] then 
            if iSortIndex > i then
                iFind = i
            else
                iFind = i-1
            end
            break
        end 
    end
    if iFind ~= iSortIndex then 
        if iFind < iSortIndex then 
            for i = iSortIndex, iFind+1, -1 do 
                self.tSortList[i] = self.tSortList[i-1]
            end 
        else
            for i = iSortIndex, iFind-1, 1 do 
                self.tSortList[i] = self.tSortList[i+1]
            end 
        end 
        self.tSortList[iFind] = iChairId
    end

    for i, v in pairs(self.tSortList) do
        self.node_rank_item[i]:setScore(self.tListInfo[v])
        if v == self.iOwnerIndex then 
            self.img_rank_my_bg:setPositionY(self.node_rank_item[i]:getPositionY())
            self.img_rank_my_bg:setVisible(true)
            self.node_rank_item[i]:setOwner(true)
        else
            self.node_rank_item[i]:setOwner(false)
        end
    end
end

return FriendGameData