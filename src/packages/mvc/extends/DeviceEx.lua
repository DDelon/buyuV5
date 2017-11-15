--  android  ios  通用接口
-- Author: lee
-- Date: 2016-09-12 11:12:06
--
local CLASS_NAME =
{
    android = "com.weile.api.NativeHelper",
    ios = "AppController",
    windows = "",
}
local DeviceEx = device
if device.platform=="ios" then
    cc.exports.luaoc = require("cocos.cocos2d.luaoc")
elseif device.platform=="android" then
    cc.exports.luaj = require("cocos.cocos2d.luaj")
end

--pickertype camera album library

local function getclassname_()
    return CLASS_NAME[device.platform]
end

function DeviceEx.callImagePicker(type, callback, allowedit)
    if device.platform == "android" then
        local javaParams = { type, callback, allowedit }
       local ok,ret=  luaj.callStaticMethod(getclassname_(), "callImagePicker", javaParams)
    elseif device.platform == "ios" then
        local args = { pickertype = type, listener = callback, allowedit = allowedit }
        local ok, ret = luaoc.callStaticMethod(getclassname_(), "callImagePicker", args)
        if ok then

        end
    end
end