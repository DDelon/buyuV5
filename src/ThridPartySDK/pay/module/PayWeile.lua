local PayWeile = class("PayWeile", require("ThridPartySDK/pay/module/PayBase"))

function PayWeile.create()
	local obj = PayWeile.new();
	obj:init(payInfo);
	return obj;
end

function PayWeile:init()
	self.super.init(self);
end

function PayWeile:doPay(payInfo)
	self.super.doPay(self, payInfo)
end

function PayWeile:orderRequest(payInfo, callFunc)
    callFunc(payInfo);
end

return PayWeile;