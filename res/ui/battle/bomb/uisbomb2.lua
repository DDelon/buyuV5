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

--Create boomeffect05
local boomeffect05 = cc.Sprite:create("battle/effect/boomeffect05_00.png")
boomeffect05:setName("boomeffect05")
boomeffect05:setTag(72)
boomeffect05:setCascadeColorEnabled(true)
boomeffect05:setCascadeOpacityEnabled(true)
boomeffect05:setScaleX(2.0000)
boomeffect05:setScaleY(2.0000)
layout = ccui.LayoutComponent:bindLayoutComponent(boomeffect05)
layout:setSize({width = 500.0000, height = 400.0000})
layout:setLeftMargin(-250.0000)
layout:setRightMargin(-250.0000)
layout:setTopMargin(-200.0000)
layout:setBottomMargin(-200.0000)
boomeffect05:setBlendFunc({src = 1, dst = 771})
Node:addChild(boomeffect05)

--Create Animation
result['animation'] = ccs.ActionTimeline:create()
  
result['animation']:setDuration(49)
result['animation']:setTimeSpeed(1.0000)

--Create FileDataTimeline
local FileDataTimeline = ccs.Timeline:create()

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(0)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_00.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(6)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_01.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(12)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_02.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(18)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_03.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(24)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_04.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(30)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_05.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(36)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_06.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(42)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_07.png")
FileDataTimeline:addFrame(localFrame)

localFrame = ccs.TextureFrame:create()
localFrame:setFrameIndex(48)
localFrame:setTween(false)
localFrame:setTextureName("battle/effect/boomeffect05_08.png")
FileDataTimeline:addFrame(localFrame)

result['animation']:addTimeline(FileDataTimeline)
FileDataTimeline:setNode(boomeffect05)

--Create FrameEventTimeline
local FrameEventTimeline = ccs.Timeline:create()

localFrame = ccs.EventFrame:create()
localFrame:setFrameIndex(49)
localFrame:setTween(false)
localFrame:setEvent("down_end")
FrameEventTimeline:addFrame(localFrame)

result['animation']:addTimeline(FrameEventTimeline)
FrameEventTimeline:setNode(boomeffect05)
--Create Animation List
local light_down = {name="light_down", startIndex=0, endIndex=49}
result['animation']:addAnimationInfo(light_down)

result['root'] = Node
return result;
end

return Result
