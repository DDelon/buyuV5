--------------------------------------------------------------
-- This file was automatically generated by Cocos Studio.
-- Do not make changes to this file.
-- All changes will be lost.
--------------------------------------------------------------

local luaExtend = require "LuaExtend"

-- using for layout to decrease count of local variables
local layout = nil
local localLuaFile = nil
local innerCSD = nil
local innerProject = nil
local localFrame = nil

local Result = {}
------------------------------------------------------------
-- function call description
-- create function caller should provide a function to 
-- get a callback function in creating scene process.
-- the returned callback function will be registered to 
-- the callback event of the control.
-- the function provider is as below :
-- Callback callBackProvider(luaFileName, node, callbackName)
-- parameter description:
-- luaFileName  : a string, lua file name
-- node         : a Node, event source
-- callbackName : a string, callback function name
-- the return value is a callback function
------------------------------------------------------------
function Result.create(callBackProvider)

local result={}
setmetatable(result, luaExtend)

--Create Node
local Node=cc.Node:create()
Node:setName("Node")

--Create panel
local panel = ccui.ImageView:create()
panel:ignoreContentAdaptWithSize(false)
panel:loadTexture("hall/task/task_bar_bg.png",0)
panel:setScale9Enabled(true)
panel:setCapInsets({x = 21, y = 12, width = 1, height = 1})
panel:setLayoutComponentEnabled(true)
panel:setName("panel")
panel:setTag(170)
panel:setCascadeColorEnabled(true)
panel:setCascadeOpacityEnabled(true)
layout = ccui.LayoutComponent:bindLayoutComponent(panel)
layout:setSize({width = 723.0000, height = 24.0000})
layout:setLeftMargin(-361.5000)
layout:setRightMargin(-361.5000)
layout:setTopMargin(-12.0000)
layout:setBottomMargin(-12.0000)
Node:addChild(panel)

--Create loading_processbar
local loading_processbar = ccui.LoadingBar:create()
loading_processbar:loadTexture("hall/task/task_bar.png",0)
loading_processbar:ignoreContentAdaptWithSize(false)
loading_processbar:setLayoutComponentEnabled(true)
loading_processbar:setName("loading_processbar")
loading_processbar:setTag(43)
loading_processbar:setCascadeColorEnabled(true)
loading_processbar:setCascadeOpacityEnabled(true)
loading_processbar:setAnchorPoint(0.0000, 0.0000)
loading_processbar:setPosition(5.0005, 5.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(loading_processbar)
layout:setPositionPercentX(0.0069)
layout:setPositionPercentY(0.2083)
layout:setPercentWidth(0.9876)
layout:setPercentHeight(0.5833)
layout:setSize({width = 714.0000, height = 14.0000})
layout:setLeftMargin(5.0005)
layout:setRightMargin(3.9995)
layout:setTopMargin(5.0000)
layout:setBottomMargin(5.0000)
panel:addChild(loading_processbar)

--Create txt_active_count
local txt_active_count = ccui.Text:create()
txt_active_count:ignoreContentAdaptWithSize(true)
txt_active_count:setTextAreaSize({width = 0, height = 0})
txt_active_count:setFontSize(28)
txt_active_count:setString([[100]])
txt_active_count:setLayoutComponentEnabled(true)
txt_active_count:setName("txt_active_count")
txt_active_count:setTag(48)
txt_active_count:setCascadeColorEnabled(true)
txt_active_count:setCascadeOpacityEnabled(true)
txt_active_count:setAnchorPoint(0.0000, 0.5000)
txt_active_count:setPosition(40.0000, 68.0001)
txt_active_count:setTextColor({r = 192, g = 60, b = 2})
layout = ccui.LayoutComponent:bindLayoutComponent(txt_active_count)
layout:setPositionPercentX(0.0553)
layout:setPositionPercentY(2.8333)
layout:setPercentWidth(0.0581)
layout:setPercentHeight(1.1667)
layout:setSize({width = 42.0000, height = 28.0000})
layout:setLeftMargin(40.0000)
layout:setRightMargin(641.0000)
layout:setTopMargin(-58.0001)
layout:setBottomMargin(54.0001)
panel:addChild(txt_active_count)

--Create task_icon_jf_1
local task_icon_jf_1 = cc.Sprite:create("hall/task/task_icon_jf.png")
task_icon_jf_1:setName("task_icon_jf_1")
task_icon_jf_1:setTag(35)
task_icon_jf_1:setCascadeColorEnabled(true)
task_icon_jf_1:setCascadeOpacityEnabled(true)
task_icon_jf_1:setPosition(15.0000, 67.9999)
layout = ccui.LayoutComponent:bindLayoutComponent(task_icon_jf_1)
layout:setPositionPercentX(0.0207)
layout:setPositionPercentY(2.8333)
layout:setPercentWidth(0.0609)
layout:setPercentHeight(1.8333)
layout:setSize({width = 44.0000, height = 44.0000})
layout:setLeftMargin(-7.0000)
layout:setRightMargin(686.0000)
layout:setTopMargin(-65.9999)
layout:setBottomMargin(45.9999)
task_icon_jf_1:setBlendFunc({src = 1, dst = 771})
panel:addChild(task_icon_jf_1)

--Create txt_process
local txt_process = ccui.Text:create()
txt_process:ignoreContentAdaptWithSize(true)
txt_process:setTextAreaSize({width = 0, height = 0})
txt_process:setFontSize(28)
txt_process:setString([[当前活跃]])
txt_process:setLayoutComponentEnabled(true)
txt_process:setName("txt_process")
txt_process:setTag(36)
txt_process:setCascadeColorEnabled(true)
txt_process:setCascadeOpacityEnabled(true)
txt_process:setPosition(-70.0000, 68.0001)
txt_process:setTextColor({r = 72, g = 79, b = 89})
layout = ccui.LayoutComponent:bindLayoutComponent(txt_process)
layout:setPositionPercentX(-0.0968)
layout:setPositionPercentY(2.8333)
layout:setPercentWidth(0.1549)
layout:setPercentHeight(1.1667)
layout:setSize({width = 112.0000, height = 28.0000})
layout:setLeftMargin(-126.0000)
layout:setRightMargin(737.0000)
layout:setTopMargin(-58.0001)
layout:setBottomMargin(54.0001)
panel:addChild(txt_process)

--Create txt_award
local txt_award = ccui.Text:create()
txt_award:ignoreContentAdaptWithSize(true)
txt_award:setTextAreaSize({width = 0, height = 0})
txt_award:setFontSize(28)
txt_award:setString([[宝箱奖励]])
txt_award:setLayoutComponentEnabled(true)
txt_award:setName("txt_award")
txt_award:setTag(37)
txt_award:setCascadeColorEnabled(true)
txt_award:setCascadeOpacityEnabled(true)
txt_award:setPosition(-70.0000, 12.7084)
txt_award:setTextColor({r = 72, g = 79, b = 89})
layout = ccui.LayoutComponent:bindLayoutComponent(txt_award)
layout:setPositionPercentX(-0.0968)
layout:setPositionPercentY(0.5295)
layout:setPercentWidth(0.1549)
layout:setPercentHeight(1.1667)
layout:setSize({width = 112.0000, height = 28.0000})
layout:setLeftMargin(-126.0000)
layout:setRightMargin(737.0000)
layout:setTopMargin(-2.7084)
layout:setBottomMargin(-1.2916)
panel:addChild(txt_award)

--Create Animation
result['animation'] = ccs.ActionTimeline:create()
  
result['animation']:setDuration(0)
result['animation']:setTimeSpeed(1.0000)
--Create Animation List

result['root'] = Node
return result;
end

return Result
