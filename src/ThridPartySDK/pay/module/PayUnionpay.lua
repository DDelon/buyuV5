local PayUnionpay = class("PayUnionpay", require("ThridPartySDK/pay/module/PayBase"))

function PayUnionpay.create()
	local obj = PayUnionpay.new();
	obj:init(payInfo);
	return obj;
end

function PayUnionpay:init()
	self.super.init(self);
end

function PayUnionpay:doPay(payInfo)
	payInfo["type"] = "unionpay_client";
	self.super.doPay(self, payInfo)
end

return PayUnionpay;