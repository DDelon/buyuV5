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
panel:setBackGroundColorOpacity(102)
panel:setLayoutComponentEnabled(true)
panel:setName("panel")
panel:setTag(5)
panel:setCascadeColorEnabled(true)
panel:setCascadeOpacityEnabled(true)
layout = ccui.LayoutComponent:bindLayoutComponent(panel)
Node:addChild(panel)

--Create img_bg
local img_bg = ccui.ImageView:create()
img_bg:ignoreContentAdaptWithSize(false)
img_bg:loadTexture("battle/newbietask/nb_bg.png",0)
img_bg:setScale9Enabled(true)
img_bg:setCapInsets({x = 20, y = 45, width = 14, height = 47})
img_bg:setLayoutComponentEnabled(true)
img_bg:setName("img_bg")
img_bg:setTag(3)
img_bg:setCascadeColorEnabled(true)
img_bg:setCascadeOpacityEnabled(true)
layout = ccui.LayoutComponent:bindLayoutComponent(img_bg)
layout:setSize({width = 350.0000, height = 120.0000})
layout:setLeftMargin(-175.0000)
layout:setRightMargin(-175.0000)
layout:setTopMargin(-60.0000)
layout:setBottomMargin(-60.0000)
panel:addChild(img_bg)

--Create spr_prop_bg
local spr_prop_bg = cc.Sprite:create("battle/magicprop/magicprop_bg0.png")
spr_prop_bg:setName("spr_prop_bg")
spr_prop_bg:setTag(4)
spr_prop_bg:setCascadeColorEnabled(true)
spr_prop_bg:setCascadeOpacityEnabled(true)
spr_prop_bg:setPosition(-110.0000, 0.0000)
spr_prop_bg:setScaleX(0.8600)
spr_prop_bg:setScaleY(0.8600)
layout = ccui.LayoutComponent:bindLayoutComponent(spr_prop_bg)
layout:setSize({width = 114.0000, height = 114.0000})
layout:setLeftMargin(-167.0000)
layout:setRightMargin(53.0000)
layout:setTopMargin(-57.0000)
layout:setBottomMargin(-57.0000)
spr_prop_bg:setBlendFunc({src = 1, dst = 771})
panel:addChild(spr_prop_bg)

--Create spr_prop
local spr_prop = cc.Sprite:create("common/prop/prop_013.png")
spr_prop:setName("spr_prop")
spr_prop:setTag(46)
spr_prop:setCascadeColorEnabled(true)
spr_prop:setCascadeOpacityEnabled(true)
spr_prop:setPosition(-110.3281, 0.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(spr_prop)
layout:setSize({width = 112.0000, height = 112.0000})
layout:setLeftMargin(-166.3281)
layout:setRightMargin(54.3281)
layout:setTopMargin(-56.0000)
layout:setBottomMargin(-56.0000)
spr_prop:setBlendFunc({src = 1, dst = 771})
panel:addChild(spr_prop)

--Create fnt_prop_num
local fnt_prop_num = ccui.TextBMFont:create()
fnt_prop_num:setFntFile("fnt/Common_num_01.fnt")
fnt_prop_num:setString([[20]])
fnt_prop_num:setLayoutComponentEnabled(true)
fnt_prop_num:setName("fnt_prop_num")
fnt_prop_num:setTag(49)
fnt_prop_num:setCascadeColorEnabled(true)
fnt_prop_num:setCascadeOpacityEnabled(true)
fnt_prop_num:setAnchorPoint(1.0000, 0.5000)
fnt_prop_num:setPosition(-67.4928, -37.3800)
layout = ccui.LayoutComponent:bindLayoutComponent(fnt_prop_num)
layout:setSize({width = 38.0000, height = 30.0000})
layout:setLeftMargin(-105.4928)
layout:setRightMargin(67.4928)
layout:setTopMargin(22.3800)
layout:setBottomMargin(-52.3800)
panel:addChild(fnt_prop_num)

--Create img_desc_bg
local img_desc_bg = ccui.ImageView:create()
img_desc_bg:ignoreContentAdaptWithSize(false)
img_desc_bg:loadTexture("common/layerbg/com_bg_grxx.png",0)
img_desc_bg:setScale9Enabled(true)
img_desc_bg:setCapInsets({x = 20, y = 20, width = 1, height = 1})
img_desc_bg:setLayoutComponentEnabled(true)
img_desc_bg:setName("img_desc_bg")
img_desc_bg:setTag(334)
img_desc_bg:setCascadeColorEnabled(true)
img_desc_bg:setCascadeOpacityEnabled(true)
img_desc_bg:setPosition(50.2700, 20.0000)
img_desc_bg:setOpacity(153)
img_desc_bg:setColor({r = 0, g = 1, b = 81})
layout = ccui.LayoutComponent:bindLayoutComponent(img_desc_bg)
layout:setSize({width = 210.0000, height = 36.0000})
layout:setLeftMargin(-54.7300)
layout:setRightMargin(-155.2700)
layout:setTopMargin(-38.0000)
layout:setBottomMargin(2.0000)
panel:addChild(img_desc_bg)

--Create text_desc
local text_desc = ccui.Text:create()
text_desc:ignoreContentAdaptWithSize(true)
text_desc:setTextAreaSize({width = 0, height = 0})
text_desc:setFontSize(24)
text_desc:setString([[获得20000鱼币]])
text_desc:setLayoutComponentEnabled(true)
text_desc:setName("text_desc")
text_desc:setTag(43)
text_desc:setCascadeColorEnabled(true)
text_desc:setCascadeOpacityEnabled(true)
text_desc:setPosition(50.0000, 20.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(text_desc)
layout:setSize({width = 156.0000, height = 24.0000})
layout:setLeftMargin(-28.0000)
layout:setRightMargin(-128.0000)
layout:setTopMargin(-32.0000)
layout:setBottomMargin(8.0000)
panel:addChild(text_desc)

--Create img_process_bg
local img_process_bg = ccui.ImageView:create()
img_process_bg:ignoreContentAdaptWithSize(false)
img_process_bg:loadTexture("battle/gunupgrade/bl_pic_bar_bg.png",0)
img_process_bg:setScale9Enabled(true)
img_process_bg:setCapInsets({x = 13, y = 14, width = 16, height = 1})
img_process_bg:setLayoutComponentEnabled(true)
img_process_bg:setName("img_process_bg")
img_process_bg:setTag(41)
img_process_bg:setCascadeColorEnabled(true)
img_process_bg:setCascadeOpacityEnabled(true)
img_process_bg:setPosition(50.0000, -19.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(img_process_bg)
layout:setSize({width = 210.0000, height = 26.0000})
layout:setLeftMargin(-55.0000)
layout:setRightMargin(-155.0000)
layout:setTopMargin(6.0000)
layout:setBottomMargin(-32.0000)
panel:addChild(img_process_bg)

--Create loading_bar_process
local loading_bar_process = ccui.LoadingBar:create()
loading_bar_process:loadTexture("battle/newbietask/nb_pic_bar.png",0)
loading_bar_process:ignoreContentAdaptWithSize(false)
loading_bar_process:setPercent(50)
loading_bar_process:setLayoutComponentEnabled(true)
loading_bar_process:setName("loading_bar_process")
loading_bar_process:setTag(42)
loading_bar_process:setCascadeColorEnabled(true)
loading_bar_process:setCascadeOpacityEnabled(true)
loading_bar_process:setPosition(105.0000, 13.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(loading_bar_process)
layout:setPositionPercentXEnabled(true)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.5000)
layout:setPositionPercentY(0.5000)
layout:setPercentWidth(0.9810)
layout:setPercentHeight(0.8462)
layout:setSize({width = 206.0000, height = 22.0000})
layout:setLeftMargin(2.0000)
layout:setRightMargin(2.0000)
layout:setTopMargin(2.0000)
layout:setBottomMargin(2.0000)
img_process_bg:addChild(loading_bar_process)

--Create process_percentage
local process_percentage = ccui.TextBMFont:create()
process_percentage:setFntFile("fnt/Common_num_01.fnt")
process_percentage:setString([[10000&20000]])
process_percentage:setLayoutComponentEnabled(true)
process_percentage:setName("process_percentage")
process_percentage:setTag(45)
process_percentage:setCascadeColorEnabled(true)
process_percentage:setCascadeOpacityEnabled(true)
process_percentage:setPosition(105.0000, 13.0000)
process_percentage:setScaleX(0.7500)
process_percentage:setScaleY(0.7500)
layout = ccui.LayoutComponent:bindLayoutComponent(process_percentage)
layout:setPositionPercentXEnabled(true)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.5000)
layout:setPositionPercentY(0.5000)
layout:setPercentWidth(0.9714)
layout:setPercentHeight(1.1538)
layout:setSize({width = 204.0000, height = 30.0000})
layout:setLeftMargin(3.0000)
layout:setRightMargin(3.0000)
layout:setTopMargin(-2.0000)
layout:setBottomMargin(-2.0000)
img_process_bg:addChild(process_percentage)

--Create spr_draw_effect
local spr_draw_effect = cc.Sprite:create("common/btn/com_btn_orange_t_0.png")
spr_draw_effect:setName("spr_draw_effect")
spr_draw_effect:setTag(14)
spr_draw_effect:setCascadeColorEnabled(true)
spr_draw_effect:setCascadeOpacityEnabled(true)
spr_draw_effect:setPosition(51.0000, -1.0000)
spr_draw_effect:setOpacity(0)
layout = ccui.LayoutComponent:bindLayoutComponent(spr_draw_effect)
layout:setSize({width = 182.0000, height = 52.0000})
layout:setLeftMargin(-40.0000)
layout:setRightMargin(-142.0000)
layout:setTopMargin(-25.0000)
layout:setBottomMargin(-27.0000)
spr_draw_effect:setBlendFunc({src = 1, dst = 1})
panel:addChild(spr_draw_effect)

--Create btn_draw
local btn_draw = ccui.Button:create()
btn_draw:ignoreContentAdaptWithSize(false)
btn_draw:loadTextureNormal("common/btn/com_btn_orange_t_0.png",0)
btn_draw:loadTexturePressed("common/btn/com_btn_orange_t_0.png",0)
btn_draw:loadTextureDisabled("common/btn/com_btn_orange_t_0.png",0)
btn_draw:setTitleFontSize(14)
btn_draw:setTitleColor({r = 65, g = 65, b = 70})
btn_draw:setScale9Enabled(true)
btn_draw:setCapInsets({x = 76, y = 17, width = 46, height = 18})
btn_draw:setLayoutComponentEnabled(true)
btn_draw:setName("btn_draw")
btn_draw:setTag(48)
btn_draw:setCascadeColorEnabled(true)
btn_draw:setCascadeOpacityEnabled(true)
btn_draw:setPosition(51.0000, -1.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(btn_draw)
layout:setSize({width = 182.0000, height = 52.0000})
layout:setLeftMargin(-40.0000)
layout:setRightMargin(-142.0000)
layout:setTopMargin(-25.0000)
layout:setBottomMargin(-27.0000)
panel:addChild(btn_draw)

--Create spr_btn_title
local spr_btn_title = cc.Sprite:create("battle/newbietask/nb_pic_djlq.png")
spr_btn_title:setName("spr_btn_title")
spr_btn_title:setTag(49)
spr_btn_title:setCascadeColorEnabled(true)
spr_btn_title:setCascadeOpacityEnabled(true)
spr_btn_title:setPosition(91.0000, 28.0020)
layout = ccui.LayoutComponent:bindLayoutComponent(spr_btn_title)
layout:setPositionPercentXEnabled(true)
layout:setPositionPercentYEnabled(true)
layout:setPositionPercentX(0.5000)
layout:setPositionPercentY(0.5385)
layout:setPercentWidth(0.7253)
layout:setPercentHeight(0.8077)
layout:setSize({width = 132.0000, height = 42.0000})
layout:setLeftMargin(25.0000)
layout:setRightMargin(25.0000)
layout:setTopMargin(2.9980)
layout:setBottomMargin(7.0020)
spr_btn_title:setBlendFunc({src = 1, dst = 771})
btn_draw:addChild(spr_btn_title)

--Create Animation
result['animation'] = ccs.ActionTimeline:create()
  
result['animation']:setDuration(90)
result['animation']:setTimeSpeed(1.0000)

--Create ScaleTimeline
local ScaleTimeline = ccs.Timeline:create()

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(42)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(90)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.2000)
localFrame:setScaleY(1.2000)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(spr_draw_effect)

--Create AlphaTimeline
local AlphaTimeline = ccs.Timeline:create()

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(42)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(43)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(255)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(70)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(204)
AlphaTimeline:addFrame(localFrame)

localFrame = ccs.AlphaFrame:create()
localFrame:setFrameIndex(90)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setAlpha(0)
AlphaTimeline:addFrame(localFrame)

result['animation']:addTimeline(AlphaTimeline)
AlphaTimeline:setNode(spr_draw_effect)

--Create PositionTimeline
local PositionTimeline = ccs.Timeline:create()

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(51.0000)
localFrame:setY(-1.0000)
PositionTimeline:addFrame(localFrame)

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(10)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(51.0010)
localFrame:setY(10.0000)
PositionTimeline:addFrame(localFrame)

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(51.0019)
localFrame:setY(-1.0000)
PositionTimeline:addFrame(localFrame)

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(2)
localFrame:setX(51.0029)
localFrame:setY(10.0000)
PositionTimeline:addFrame(localFrame)

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(51.0029)
localFrame:setY(-1.0000)
PositionTimeline:addFrame(localFrame)

localFrame = ccs.PositionFrame:create()
localFrame:setFrameIndex(42)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setX(51.0029)
localFrame:setY(-1.0000)
PositionTimeline:addFrame(localFrame)

result['animation']:addTimeline(PositionTimeline)
PositionTimeline:setNode(btn_draw)

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
localFrame:setFrameIndex(10)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setScaleX(1.1000)
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
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(2)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.1000)
ScaleTimeline:addFrame(localFrame)

localFrame = ccs.ScaleFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(true)
localFrame:setTweenType(2)
localFrame:setScaleX(1.0000)
localFrame:setScaleY(1.0000)
ScaleTimeline:addFrame(localFrame)

result['animation']:addTimeline(ScaleTimeline)
ScaleTimeline:setNode(btn_draw)

--Create RotationSkewTimeline
local RotationSkewTimeline = ccs.Timeline:create()

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0000)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(10)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(3.0006)
localFrame:setSkewY(3.0000)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(15)
localFrame:setTween(true)
localFrame:setTweenType(0)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(-0.0002)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(20)
localFrame:setTween(true)
localFrame:setTweenType(2)
localFrame:setSkewX(-3.0000)
localFrame:setSkewY(-3.0004)
RotationSkewTimeline:addFrame(localFrame)

localFrame = ccs.RotationSkewFrame:create()
localFrame:setFrameIndex(35)
localFrame:setTween(true)
localFrame:setTweenType(2)
localFrame:setSkewX(0.0000)
localFrame:setSkewY(0.0000)
RotationSkewTimeline:addFrame(localFrame)

result['animation']:addTimeline(RotationSkewTimeline)
RotationSkewTimeline:setNode(btn_draw)
--Create Animation List
local animation0 = {name="animation0", startIndex=0, endIndex=90}
result['animation']:addAnimationInfo(animation0)

result['root'] = Node
return result;
end

return Result

