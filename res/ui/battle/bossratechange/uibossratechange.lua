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
local panel = ccui.Layout:create()
panel:ignoreContentAdaptWithSize(false)
panel:setClippingEnabled(false)
panel:setBackGroundColorType(1)
panel:setBackGroundColor({r = 150, g = 200, b = 255})
panel:setBackGroundColorOpacity(0)
panel:setTouchEnabled(true);
panel:setLayoutComponentEnabled(true)
panel:setName("panel")
panel:setTag(3)
panel:setCascadeColorEnabled(true)
panel:setCascadeOpacityEnabled(true)
layout = ccui.LayoutComponent:bindLayoutComponent(panel)
layout:setPositionPercentXEnabled(true)
layout:setPositionPercentYEnabled(true)
Node:addChild(panel)

--Create rc_bg_1
local rc_bg_1 = cc.Sprite:create("battle/ratechange/rc_bg_1.png")
rc_bg_1:setName("rc_bg_1")
rc_bg_1:setTag(2)
rc_bg_1:setCascadeColorEnabled(true)
rc_bg_1:setCascadeOpacityEnabled(true)
layout = ccui.LayoutComponent:bindLayoutComponent(rc_bg_1)
layout:setPositionPercentXEnabled(true)
layout:setSize({width = 436.0000, height = 140.0000})
layout:setLeftMargin(-218.0000)
layout:setRightMargin(-218.0000)
layout:setTopMargin(-70.0000)
layout:setBottomMargin(-70.0000)
rc_bg_1:setBlendFunc({src = 1, dst = 771})
panel:addChild(rc_bg_1)

--Create rc_bg_2
local rc_bg_2 = cc.Sprite:create("battle/ratechange/rc_bg_2.png")
rc_bg_2:setName("rc_bg_2")
rc_bg_2:setTag(33)
rc_bg_2:setCascadeColorEnabled(true)
rc_bg_2:setCascadeOpacityEnabled(true)
rc_bg_2:setAnchorPoint(0.2185, 0.1025)
rc_bg_2:setPosition(404.3475, 69.4682)
rc_bg_2:setRotationSkewX(144.0000)
rc_bg_2:setRotationSkewY(144.0121)
layout = ccui.LayoutComponent:bindLayoutComponent(rc_bg_2)
layout:setPositionPercentX(0.9274)
layout:setPositionPercentY(0.4962)
layout:setPercentWidth(0.0550)
layout:setPercentHeight(0.3714)
layout:setSize({width = 24.0000, height = 52.0000})
layout:setLeftMargin(399.1035)
layout:setRightMargin(12.8965)
layout:setTopMargin(23.8618)
layout:setBottomMargin(64.1382)
rc_bg_2:setBlendFunc({src = 1, dst = 771})
rc_bg_1:addChild(rc_bg_2)

--Create rc_bg_3
local rc_bg_3 = cc.Sprite:create("battle/ratechange/rc_bg_3.png")
rc_bg_3:setName("rc_bg_3")
rc_bg_3:setTag(34)
rc_bg_3:setCascadeColorEnabled(true)
rc_bg_3:setCascadeOpacityEnabled(true)
rc_bg_3:setPosition(22.9317, 51.4807)
layout = ccui.LayoutComponent:bindLayoutComponent(rc_bg_3)
layout:setPositionPercentX(0.9555)
layout:setPositionPercentY(0.9900)
layout:setPercentWidth(1.5833)
layout:setPercentHeight(0.7308)
layout:setSize({width = 38.0000, height = 38.0000})
layout:setLeftMargin(3.9317)
layout:setRightMargin(-17.9317)
layout:setTopMargin(-18.4807)
layout:setBottomMargin(32.4807)
rc_bg_3:setBlendFunc({src = 1, dst = 771})
rc_bg_2:addChild(rc_bg_3)

--Create Panel_2
local Panel_2 = ccui.Layout:create()
Panel_2:ignoreContentAdaptWithSize(false)
Panel_2:setClippingEnabled(true)
Panel_2:setBackGroundColorType(1)
Panel_2:setBackGroundColor({r = 150, g = 200, b = 255})
Panel_2:setBackGroundColorOpacity(0)
Panel_2:setTouchEnabled(true);
Panel_2:setLayoutComponentEnabled(true)
Panel_2:setName("Panel_2")
Panel_2:setTag(101)
Panel_2:setCascadeColorEnabled(true)
Panel_2:setCascadeOpacityEnabled(true)
Panel_2:setAnchorPoint(0.5000, 0.5000)
Panel_2:setPosition(-42.0004, -0.0001)
Panel_2:setScaleX(0.7000)
Panel_2:setScaleY(0.7000)
layout = ccui.LayoutComponent:bindLayoutComponent(Panel_2)
layout:setSize({width = 350.0000, height = 116.0000})
layout:setLeftMargin(-217.0004)
layout:setRightMargin(-132.9996)
layout:setTopMargin(-57.9999)
layout:setBottomMargin(-58.0001)
panel:addChild(Panel_2)

--Create node_fnt_1
innerCSD = require("ui.battle.bossratechange.uinum")
innerProject = innerCSD.create(callBackProvider)
local node_fnt_1 = innerProject.root
node_fnt_1.animation = innerProject.animation
node_fnt_1:setName("node_fnt_1")
node_fnt_1:setTag(47)
node_fnt_1:setCascadeColorEnabled(true)
node_fnt_1:setCascadeOpacityEnabled(true)
node_fnt_1:setPosition(42.5800, 60.3200)
layout = ccui.LayoutComponent:bindLayoutComponent(node_fnt_1)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.1217)
layout:setPositionPercentY(0.5200)
layout:setLeftMargin(42.5800)
layout:setRightMargin(307.4200)
layout:setTopMargin(55.6800)
layout:setBottomMargin(60.3200)
innerProject.animation:setTimeSpeed(1.0000)
node_fnt_1:runAction(innerProject.animation)
Panel_2:addChild(node_fnt_1)

--Create node_fnt_2
innerCSD = require("ui.battle.bossratechange.uinum")
innerProject = innerCSD.create(callBackProvider)
local node_fnt_2 = innerProject.root
node_fnt_2.animation = innerProject.animation
node_fnt_2:setName("node_fnt_2")
node_fnt_2:setTag(54)
node_fnt_2:setCascadeColorEnabled(true)
node_fnt_2:setCascadeOpacityEnabled(true)
node_fnt_2:setPosition(125.8700, 60.3200)
layout = ccui.LayoutComponent:bindLayoutComponent(node_fnt_2)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.3596)
layout:setPositionPercentY(0.5200)
layout:setLeftMargin(125.8700)
layout:setRightMargin(224.1300)
layout:setTopMargin(55.6800)
layout:setBottomMargin(60.3200)
innerProject.animation:setTimeSpeed(1.0000)
node_fnt_2:runAction(innerProject.animation)
Panel_2:addChild(node_fnt_2)

--Create node_fnt_3
innerCSD = require("ui.battle.bossratechange.uinum")
innerProject = innerCSD.create(callBackProvider)
local node_fnt_3 = innerProject.root
node_fnt_3.animation = innerProject.animation
node_fnt_3:setName("node_fnt_3")
node_fnt_3:setTag(61)
node_fnt_3:setCascadeColorEnabled(true)
node_fnt_3:setCascadeOpacityEnabled(true)
node_fnt_3:setPosition(213.1500, 60.3200)
layout = ccui.LayoutComponent:bindLayoutComponent(node_fnt_3)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.6090)
layout:setPositionPercentY(0.5200)
layout:setLeftMargin(213.1500)
layout:setRightMargin(136.8500)
layout:setTopMargin(55.6800)
layout:setBottomMargin(60.3200)
innerProject.animation:setTimeSpeed(1.0000)
node_fnt_3:runAction(innerProject.animation)
Panel_2:addChild(node_fnt_3)

--Create node_fnt_4
innerCSD = require("ui.battle.bossratechange.uinum")
innerProject = innerCSD.create(callBackProvider)
local node_fnt_4 = innerProject.root
node_fnt_4.animation = innerProject.animation
node_fnt_4:setName("node_fnt_4")
node_fnt_4:setTag(68)
node_fnt_4:setCascadeColorEnabled(true)
node_fnt_4:setCascadeOpacityEnabled(true)
node_fnt_4:setPosition(299.8900, 61.4800)
layout = ccui.LayoutComponent:bindLayoutComponent(node_fnt_4)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.8568)
layout:setPositionPercentY(0.5300)
layout:setLeftMargin(299.8900)
layout:setRightMargin(50.1100)
layout:setTopMargin(54.5200)
layout:setBottomMargin(61.4800)
innerProject.animation:setTimeSpeed(1.0000)
node_fnt_4:runAction(innerProject.animation)
Panel_2:addChild(node_fnt_4)

--Create Animation
result['animation'] = ccs.ActionTimeline:create()
  
result['animation']:setDuration(45)
result['animation']:setTimeSpeed(1.0000)

--Create PositionTimeline
local PositionTimeline = ccs.Timeline:create()

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(404.3475)
localFrame:setY(69.4682)
PositionTimeline:addFrame(localFrame)

result['animation']:addTimeline(PositionTimeline)
PositionTimeline:setNode(rc_bg_2)

--Create ScaleTimeline
local ScaleTimeline = ccs.Timeline:create()

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(rc_bg_2)

--Create RotationSkewTimeline
local RotationSkewTimeline = ccs.Timeline:create()

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0121)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0121)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(25)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(48.0000)
localFrame:setSkewY(48.0121)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(30)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(90.0000)
localFrame:setSkewY(90.0121)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(144.0000)
localFrame:setSkewY(144.0121)
RotationSkewTimeline:addFrame(localFrame)

result['animation']:addTimeline(RotationSkewTimeline)
RotationSkewTimeline:setNode(rc_bg_2)

--Create AnchorPointTimeline
local AnchorPointTimeline = ccs.Timeline:create()

localFrame = ccs.AnchorPointFrame:create()
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAnchorPoint({x = 0.2185, y = 0.1025})
AnchorPointTimeline:addFrame(localFrame)

result['animation']:addTimeline(AnchorPointTimeline)
AnchorPointTimeline:setNode(rc_bg_2)

--Create ScaleTimeline
local ScaleTimeline = ccs.Timeline:create()

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(0.0010)
localFrame:setScaleY(0.0010)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(10)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.3000)
localFrame:setScaleY(1.3000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(45)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(0.0010)
localFrame:setScaleY(0.0010)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(panel)

--Create RotationSkewTimeline
local RotationSkewTimeline = ccs.Timeline:create()

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(10)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0000)
RotationSkewTimeline:addFrame(localFrame)

result['animation']:addTimeline(RotationSkewTimeline)
RotationSkewTimeline:setNode(panel)

--Create AlphaTimeline
local AlphaTimeline = ccs.Timeline:create()

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(10)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(255)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(255)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(45)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

result['animation']:addTimeline(AlphaTimeline)
AlphaTimeline:setNode(panel)

--Create FrameEventTimeline
local FrameEventTimeline = ccs.Timeline:create()

localFrame = ccs.EventFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(false)
localFrame:setEvent("openend")
FrameEventTimeline:addFrame(localFrame)

localFrame = ccs.EventFrame:create()
localFrame:setFrameIndex(45)
localFrame:setTween(false)
localFrame:setEvent("closeend")
FrameEventTimeline:addFrame(localFrame)

result['animation']:addTimeline(FrameEventTimeline)
FrameEventTimeline:setNode(panel)
--Create Animation List
local open = {name="open", startIndex=0, endIndex=35}
result['animation']:addAnimationInfo(open)
local close = {name="close", startIndex=35, endIndex=45}
result['animation']:addAnimationInfo(close)

result['root'] = Node
return result;
end

return Result

