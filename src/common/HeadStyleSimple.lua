local HeadStyleSimple = class("HeadStyleSimple", cc.load("mvc").ViewBase)

HeadStyleSimple.AUTO_RESOLUTION   = false
HeadStyleSimple.RESOURCE_FILENAME = "ui/common/uiheadstylesimple"
HeadStyleSimple.RESOURCE_BINDING  = {
    ["panel"]     = { ["varname"] = "panel"  },
    
    ["spr_head"]     = { ["varname"] = "spr_head"  },

    ["text_id_title"] = { ["varname"] = "text_id_title"  },
    ["text_name_title"]    = { ["varname"] = "text_name_title"} , 
    ["text_name"]     = { ["varname"] = "text_name"  },    
    ["text_id"]    = { ["varname"] = "text_id"} , 
    ["text_score_title"]     = { ["varname"] = "text_score_title"  },   
    ["text_score"]     = { ["varname"] = "text_score"  }, 

    ["spr_ready"]     = { ["varname"] = "spr_ready"  }, 
    
    ["spr_loop"]     = { ["varname"] = "spr_loop"  }, 
    
}

function HeadStyleSimple:onCreate(...)
    self.withScoreHeadPosX = self.spr_head:getPositionX();
    self:initTitleByConfig();
    self:initContent();
    self.spr_ready:setVisible(false)
    self.spr_loop:setLocalZOrder(150)
    self.spr_ready:setLocalZOrder(200)
end

function HeadStyleSimple:initTitleByConfig()
    local scoreTitle = "积分:";
    local idTitle = "ID ：";
    local nameTitle = "昵称：";

    self.text_id_title:setString(idTitle);
    self.text_name_title:setString(nameTitle);
    self.text_score_title:setString(scoreTitle);
end

function HeadStyleSimple:initContent()
    self:setPlayerId("nil");
    self:setName("nil");
    self:setScore("nil");
end

function HeadStyleSimple:setId(id)
    self.id = id;
end

function HeadStyleSimple:getId()
    return self.id;
end

function HeadStyleSimple:setPlayerId(playerId)
    self.text_id:setString(playerId);
end

function HeadStyleSimple:getPlayerId()
    return tonumber(self.text_id:getString());
end

function HeadStyleSimple:setName(name)
    self.text_name:setString(name);
end

function HeadStyleSimple:getName()
    return self.text_name:getString();
end

function HeadStyleSimple:setScore(score)
    self.text_score:setString(score);
end

function HeadStyleSimple:getScore()
    return tonumber(self.text_score:getString());
end

function HeadStyleSimple:setAllTextVisible(visible)
    self.text_id_title:setVisible(visible);
    self.text_name_title:setVisible(visible);
    self.text_name:setVisible(visible);
    self.text_id:setVisible(visible);
    self.text_score_title:setVisible(visible);
    self.text_score:setVisible(visible);
end

--切换到带积分的头像类型
function HeadStyleSimple:switchWithScoreMode()
    self:setHeadX(self.withScoreHeadPosX);
    self:setAllTextVisible(true);
    self.text_score:setVisible(true);
    self.text_score_title:setVisible(true);
end

--切换到没有带积分的头像类型
function HeadStyleSimple:switchWithoutScoreMode()
    self:setHeadX(0);
    self:setAllTextVisible(true);
    self.text_score:setVisible(false);
    self.text_score_title:setVisible(false);
end

--切换到无人类型的头像
function HeadStyleSimple:switchNull()
    self:setHeadX(0);

    self.spr_head:setVisible(true);
    if self.headClip ~= nil then
        self.headClip:setVisible(false);
    end

    self:setAllTextVisible(false);
    self:initContent();
    self.spr_loop:setVisible(false)
end

function HeadStyleSimple:setHeadX(val)
    self.spr_head:setPositionX(val)
    if self.headClip ~= nil then
        self.headClip:setPositionX(val)
    end
    self.spr_loop:setPositionX(val)
    
end

function HeadStyleSimple:setHead(headPath)
    if headPath == nil or headPath == "" then
        return;
    end
    self.spr_head:setVisible(false);
    self.spr_loop:setVisible(true)
    if self.headClip ~= nil then
        self.headClip:setVisible(true);
        self:replaceHead(headPath);
        return;
    end

    
    self.headClip = cc.ClippingNode:create();

    local templatePath = self.spr_head:getResourceName();
    local cutedNode = cc.Sprite:create(headPath);
    cutedNode:setTag(10086)
    self.headClip:setAlphaThreshold(0.9);
    self.headClip:addChild(cutedNode);
    local stencil = cc.Sprite:create(templatePath);
    stencil:setTag(10010)
    self.headClip:setStencil(stencil)
    self.headClip:setInverted(false)
    self:setHeadX(self.spr_head:getPositionX());
    self.headClip.nodeType = "viewlist"
    self.panel:addChild(self.headClip, 100);
end

function HeadStyleSimple:replaceHead(headPath)
    if headPath == nil or headPath == "" or self.headClip == nil then
        return;
    end
    local cutedNode = cc.Sprite:create(headPath);
    cutedNode:setTag(10086)
    self.headClip:removeChildByTag(10086)
    self.headClip:addChild(cutedNode);
end

function HeadStyleSimple:setIsReady(isReady)
    self.spr_ready:setVisible(isReady)
end

return HeadStyleSimple;