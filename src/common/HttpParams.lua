 --http请求参数
 -- Date: 2017-06-07 11:47:56
local M={}
-- 加密后的字符串替换处理将其中的 “/” 替换为 “-”,将 “+” 替换为 “,”。结果为：
local function cryptencode_(crypt)
    crypt = Helper.StringReplace(Helper.StringReplace(crypt, "/", "-"), "+", ",")
    return crypt
end

-- 解密 时字符串替换处理
local function cryptdecode_(crypt)
    local input = string.gsub(crypt, "%-", "/")
    return (string.gsub(input, "%,", "+"))
end

-- 拼接参数
local function joinParams_(params)
    local param_str = ""
    for k, v in pairs(checktable(params)) do
        param_str = param_str .. string.format("%s=%s&", tostring(k), tostring(v));
    end
    if param_str and string.len(param_str) > 1 then
        param_str = string.sub(param_str, 1, -2)
    end
    return param_str
end

-- 加密
local function encrypt_(params)
    local crypt = Helper.CryptStr(params, URLKEY);
    return cryptencode_(crypt)
end
 
function M:Encrypt(params)
    return encrypt_(params)
end
--生成 URL-encode 之后的请求字符串
function M:BuildQuery(params)
    return joinParams_(params)
end

function M:BuildEncryptData(params)
    return encrypt_(joinParams_(params))
end

return M