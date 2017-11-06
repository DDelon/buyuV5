-- Author: lee
-- Date: 2016-08-25 10:43:06
--获取次对象请使用 hallmanager:GetHttp()

local HttpParams= require("common.HttpParams")
local Timer = require("common.Timer")
local M = class("HttpProxy")

M.IS_PROXY=true
M.REQUEST_RESPONSE_STRING       = 0  --返回字符串结果
M.REQUEST_RESPONSE_ARRAY_BUFFER = 1  --CLuaMsgHeader
M.REQUEST_RESPONSE_BLOB         = 2  --CLuaMsgHeader
M.REQUEST_RESPONSE_DOCUMENT     = 3  --CLuaMsgHeader
M.REQUEST_RESPONSE_JSON         = 4  --返回json 格式结果
M.REQUEST_RESPONSE_LUA          = 5  --返回lua 格式结果
 
local listeners_={}
local timer_
local ignore_prefix_={"://","http://","https://"}

local function handleurl_(url)
    assert(url,"url is nil error")
    for _,v in ipairs(ignore_prefix_ ) do
        if FishGF.StartWith(url, tostring(v))  then
            url = string.sub(url,#v+1)
            break
        end
    end
    return url
end

local function cancel_(handle)    
    if timer_ and handle then
        timer_:kill(handle)  
    end
    listeners_[handle]=nil  
end
 
-- -- 拼接参数
local function joinParams_(params)
    return HttpParams:BuildQuery(params) 
end
 
local function startTimer_(timeout,callback)    
    timeout=timeout or 15
    assert(type(timeout)=="number","timeout var type error ")
    timer_= timer_ or Timer.new()
    local handle= timer_:runOnce(function(dt,dat,entryid)
        if listeners_[entryid] then
            listeners_[entryid]=nil
            if callback then callback(-1,"请求超时") end
        end
    end,timeout)
    return handle
end

--生成 URL-encode 之后的请求字符串
function M:BuildQuery(params)
    return HttpParams:BuildQuery(params)
end

function M:BuildEncryptData(params)
    return HttpParams:BuildEncryptData(params)
end

function M:RegisterHttpProxyEvent(obj,longlived)
    self.manager_=obj
    self.longlived_=longlived
    obj.event["OnMsgHttpProxyReply"]=handler(self, self.OnMsgHttpProxyReply)    
    timer_=Timer.new()
    return self
end

function M:OnMsgHttpProxyReply(mgrobj,key,code,msgstream) 
    printf("OnMsgHttpProxyReply key: %d code : %d ", key,code )       
    if listeners_[key] and listeners_[key][2]  then 
        local msg 
        if code~=200 and msgstream then
            msg = "http代理请求失败 :" ..tostring( msgstream:ReadStringA())
        else
            msg = self:HttpProxyResponseHandler(listeners_[key].responseType,msgstream) or "数据解析失败"
        end
        local func = checktable(listeners_[key])[2]
        if  func and type(func)=="function" then
            func(code,msg)
        end
        printf("proxy response_handler_: key %d , respcode %d ,data %s",key,code,json.encode(checktable(listeners_[key])))
    end
    if not self.longlived_ then
        mgrobj:CloseSocket()
        printf("proxy connect closed")
    end
    cancel_(key)
end

function M:HttpProxyResponseHandler(responseType,msgstream)
    local func={
        [self.REQUEST_RESPONSE_STRING]=function(msg) 
            return msg
        end,
        [self.REQUEST_RESPONSE_JSON]=function(msg) 
            local ok, datatable = pcall(function() return json.decode(msg) end)
            return ok and datatable or nil
        end,
        [self.REQUEST_RESPONSE_LUA]=function(msg) 
            local ok, datatable = pcall(function() return loadstring(msg)(); end)
            return ok and datatable or nil
        end,
    }
    if msgstream and responseType and func[responseType] then
        local respstring= msgstream:ReadStringA()
        return func[responseType](respstring)
    else
        return msgstream
    end
end

function M:SendProxyRequest(url, callback, post, data,timeout,responseType)
    print("SendProxyRequest url:"..url.." data:"..data)
    if not self.manager_ then
        if callback then callback(-1,"连接失败") end
        printf("连接失败")
        return
    end
    url=handleurl_(url)
    local handle= startTimer_(timeout,callback)
    listeners_[handle]={url, callback, post, data,timeout,responseType = responseType or self.REQUEST_RESPONSE_STRING }

    local function sendRequest()
        local msg = CLuaMsgHeader.New()             
        msg:WriteLonglong(handle)
        msg:WriteBool(true)
        printf("request method:%s , url:%s, data:%s", FishGF.iif(post,"POST","GET"),tostring(url),tostring(data))
        msg:WriteStringA(url)

        if post and data and #data>0 then  
            if type(data) == "table" then
                data = joinParams_(data)
            end 
            msg:WriteStringA(data) 
            msg.position=msg.position-1
        end
        self.manager_:SendHttpProxyRequest(msg,post,FishCD.MSG_HEADER_FLAG.COMPRESS+FishCD.MSG_HEADER_FLAG.ENCODE+FishCD.MSG_HEADER_FLAG.MASK)         
    end
    if self.longlived_ then
        sendRequest()
    else  
        self.manager_:startConnect(function(obj,ok,errcode)
            if ok then
                sendRequest()
            else
                printf("连接登录代理服务器失败 %s",tostring(errcode))
                cancel_(handle)
                local func = checktable(listeners_[handle])[2]
                if  func and type(func)=="function" then
                    func(errcode,"连接服务器失败")
                end
            end
        end)
    end
    return self,handle
end

-- post 方式请求 
function M:Post(url, callback, params, iscrypt,timeout,responseType)
    if params then
        if type(params) == "table" then
            params = joinParams_(params)
        end 
    end
    return self:SendProxyRequest(url, callback, true, params,timeout,responseType)
end

-- get方式请求   
function M:Get(url, callback, params, iscrypt,timeout,responseType)
    if params then
        if type(params) == "table" then
            params = joinParams_(params)
        end
        url = url .. "?" .. params 
    end
    return self:SendProxyRequest(url, callback, false,"",timeout,responseType)
end

function M:UploadFile(url,callback,filename)
    local errmsg
    local timeout
    local handle
    --if listeners_[-1] then errmsg="不支持多个文件同时上传" end
    if not self.manager_ then errmsg="连接失败"   end
    if errmsg then 
        if callback then callback(-1,errmsg) end
        printf(tostring(errmsg))
        return self, -1
    end
    url=handleurl_(url)
    local ext= string.lower(gg.GetFileExt(filename) or "无")
    if ext=="png" or ext=="jpg" or ext=="jpeg" then
        url=url.."?ext=".. ext
        handle=-1
        if timeout then 
            handle= startTimer_(timeout,callback)
        end
        listeners_[handle]={url, callback, true, "","0",responseType =self.REQUEST_RESPONSE_STRING }
        printf(" proxy uploadfile handle %d, data :%s",handle,json.encode(checktable(listeners_[handle])))
        self.manager_:UploadFileData(url,filename,handle,FishCD.MSG_HEADER_FLAG.COMPRESS+FishCD.MSG_HEADER_FLAG.ENCODE+FishCD.MSG_HEADER_FLAG.MASK)
    elseif callback then
        callback(99,"不支持"..tostring(ext).."类型文件")
        printf(tostring("不支持"..tostring(ext).."类型文件"))
    end
    return self,handle
end

function M:CancelRequest(handle)
    cancel_(handle)
end

function M:Shutdown()
    self.manager_=nil 
    if timer_ then
        timer_:killAll()
    end
    listeners_[{}]=nil  
    listeners_={}
end

return M

