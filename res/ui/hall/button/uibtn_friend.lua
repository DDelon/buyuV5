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

--Create spr_msw_light_0_0
local spr_msw_light_0_0 = cc.Sprite:create("hall/btn/hall_btn_hyyj_light.png")
spr_msw_light_0_0:setName("spr_msw_light_0_0")
spr_msw_light_0_0:setTag(94)
spr_msw_light_0_0:setCascadeColorEnabled(true)
spr_msw_light_0_0:setCascadeOpacityEnabled(true)
spr_msw_light_0_0:setPosition(0.0000, 2.0000)
spr_msw_light_0_0:setOpacity(0)
layout = ccui.LayoutComponent:bindLayoutComponent(spr_msw_light_0_0)
layout:setSize({width = 204.0000, height = 90.0000})
layout:setLeftMargin(-102.0000)
layout:setRightMargin(-102.0000)
layout:setTopMargin(-47.0000)
layout:setBottomMargin(-43.0000)
spr_msw_light_0_0:setBlendFunc({src = 770, dst = 1})
Node:addChild(spr_msw_light_0_0)

--Create btn_enter_friend
local btn_enter_friend = ccui.Button:create()
btn_enter_friend:ignoreContentAdaptWithSize(false)
btn_enter_friend:loadTextureNormal("hall/btn/hall_btn_hyyj_0.png",0)
btn_enter_friend:loadTexturePressed("hall/btn/hall_btn_hyyj_1.png",0)
btn_enter_friend:loadTextureDisabled("hall/btn/hall_btn_hyyj_1.png",0)
btn_enter_friend:setTitleFontSize(14)
btn_enter_friend:setTitleColor({r = 65, g = 65, b = 70})
btn_enter_friend:setLayoutComponentEnabled(true)
btn_enter_friend:setName("btn_enter_friend")
btn_enter_friend:setTag(827)
btn_enter_friend:setCascadeColorEnabled(true)
btn_enter_friend:setCascadeOpacityEnabled(true)
layout = ccui.LayoutComponent:bindLayoutComponent(btn_enter_friend)
layout:setPositionPercentXEnabled(true)
layout:setPositionPercentYEnabled(true)
layout:setSize({width = 204.0000, height = 90.0000})
layout:setLeftMargin(-102.0000)
layout:setRightMargin(-102.0000)
layout:setTopMargin(-45.0000)
layout:setBottomMargin(-45.0000)
Node:addChild(btn_enter_friend)

--Create hall_btn_msw_1_1
local hall_btn_msw_1_1 = cc.Sprite:create("hall/btn/hall_btn_hyyj_0.png")
hall_btn_msw_1_1:setName("hall_btn_msw_1_1")
hall_btn_msw_1_1:setTag(84)
hall_btn_msw_1_1:setCascadeColorEnabled(true)
hall_btn_msw_1_1:setCascadeOpacityEnabled(true)
hall_btn_msw_1_1:setOpacity(0)
layout = ccui.LayoutComponent:bindLayoutComponent(hall_btn_msw_1_1)
layout:setSize({width = 204.0000, height = 90.0000})
layout:setLeftMargin(-102.0000)
layout:setRightMargin(-102.0000)
layout:setTopMargin(-45.0000)
layout:setBottomMargin(-45.0000)
hall_btn_msw_1_1:setBlendFunc({src = 770, dst = 1})
Node:addChild(hall_btn_msw_1_1)

--Create spr_msw_light_0
local spr_msw_light_0 = cc.Sprite:create("hall/btn/hall_btn_hyyj_light.png")
spr_msw_light_0:setName("spr_msw_light_0")
spr_msw_light_0:setTag(829)
spr_msw_light_0:setCascadeColorEnabled(true)
spr_msw_light_0:setCascadeOpacityEnabled(true)
spr_msw_light_0:setPosition(0.0000, 2.0000)
spr_msw_light_0:setOpacity(0)
layout = ccui.LayoutComponent:bindLayoutComponent(spr_msw_light_0)
layout:setSize({width = 204.0000, height = 90.0000})
layout:setLeftMargin(-102.0000)
layout:setRightMargin(-102.0000)
layout:setTopMargin(-47.0000)
layout:setBottomMargin(-43.0000)
spr_msw_light_0:setBlendFunc({src = 770, dst = 1})
Node:addChild(spr_msw_light_0)

--Create Animation
result['animation'] = ccs.ActionTimeline:create()
  
result['animation']:setDuration(70)
result['animation']:setTimeSpeed(1.0000)

--Create PositionTimeline
local PositionTimeline = ccs.Timeline:create()

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(0.0000)
localFrame:setY(2.0000)
PositionTimeline:addFrame(localFrame)

result['animation']:addTimeline(PositionTimeline)
PositionTimeline:setNode(spr_msw_light_0_0)

--Create ScaleTimeline
local ScaleTimeline = ccs.Timeline:create()

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(40)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(70)
localFrame:setTween(true)
localFrame:setTweenType(1)
localFrame:setScaleX(1.5000)
localFrame:setScaleY(1.5000)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(spr_msw_light_0_0)

--Create RotationSkewTimeline
local RotationSkewTimeline = ccs.Timeline:create()

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0000)
RotationSkewTimeline:addFrame(localFrame)

result['animation']:addTimeline(RotationSkewTimeline)
RotationSkewTimeline:setNode(spr_msw_light_0_0)

--Create AlphaTimeline
local AlphaTimeline = ccs.Timeline:create()

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(40)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(255)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(70)
localFrame:setTween(true)
localFrame:setTweenType(1)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

result['animation']:addTimeline(AlphaTimeline)
AlphaTimeline:setNode(spr_msw_light_0_0)

--Create FileDataTimeline
local FileDataTimeline = ccs.Timeline:create()

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(40)
localFrame:setTween(false)
localFrame:setTextureName("hall/btn/hall_btn_hyyj_light.png")
FileDataTimeline:addFrame(localFrame)

result['animation']:addTimeline(FileDataTimeline)
FileDataTimeline:setNode(spr_msw_light_0_0)

--Create BlendFuncTimeline
local BlendFuncTimeline = ccs.Timeline:create()

result['animation']:addTimeline(BlendFuncTimeline)
BlendFuncTimeline:setNode(spr_msw_light_0_0)

--Create PositionTimeline
local PositionTimeline = ccs.Timeline:create()

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(0.0000)
localFrame:setY(0.0000)
PositionTimeline:addFrame(localFrame)

result['animation']:addTimeline(PositionTimeline)
PositionTimeline:setNode(hall_btn_msw_1_1)

--Create ScaleTimeline
local ScaleTimeline = ccs.Timeline:create()

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(hall_btn_msw_1_1)

--Create RotationSkewTimeline
local RotationSkewTimeline = ccs.Timeline:create()

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0000)
RotationSkewTimeline:addFrame(localFrame)

result['animation']:addTimeline(RotationSkewTimeline)
RotationSkewTimeline:setNode(hall_btn_msw_1_1)

--Create AlphaTimeline
local AlphaTimeline = ccs.Timeline:create()

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(40)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(153)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(55)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

result['animation']:addTimeline(AlphaTimeline)
AlphaTimeline:setNode(hall_btn_msw_1_1)

--Create PositionTimeline
local PositionTimeline = ccs.Timeline:create()

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(0.0000)
localFrame:setY(2.0000)
PositionTimeline:addFrame(localFrame)

result['animation']:addTimeline(PositionTimeline)
PositionTimeline:setNode(spr_msw_light_0)

--Create ScaleTimeline
local ScaleTimeline = ccs.Timeline:create()

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(40)
localFrame:setTween(true)
localFrame:setTweenType(1)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(spr_msw_light_0)

--Create RotationSkewTimeline
local RotationSkewTimeline = ccs.Timeline:create()

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0000)
RotationSkewTimeline:addFrame(localFrame)

result['animation']:addTimeline(RotationSkewTimeline)
RotationSkewTimeline:setNode(spr_msw_light_0)

--Create AlphaTimeline
local AlphaTimeline = ccs.Timeline:create()

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(255)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(40)
localFrame:setTween(true)
localFrame:setTweenType(1)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

result['animation']:addTimeline(AlphaTimeline)
AlphaTimeline:setNode(spr_msw_light_0)

--Create FileDataTimeline
local FileDataTimeline = ccs.Timeline:create()

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(false)
localFrame:setTextureName("hall/btn/hall_btn_hyyj_light.png")
FileDataTimeline:addFrame(localFrame)

result['animation']:addTimeline(FileDataTimeline)
FileDataTimeline:setNode(spr_msw_light_0)

--Create BlendFuncTimeline
local BlendFuncTimeline = ccs.Timeline:create()

result['animation']:addTimeline(BlendFuncTimeline)
BlendFuncTimeline:setNode(spr_msw_light_0)
--Create Animation List
local light = {name="light", startIndex=0, endIndex=70}
result['animation']:addAnimationInfo(light)

result['root'] = Node
return result;
end

return Result
