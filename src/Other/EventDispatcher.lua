local EventDispatcher = class ("EventDispatcher")

function EventDispatcher.create()
	local dispatcher = EventDispatcher.new();
	dispatcher:init();
	return dispatcher;
end

function EventDispatcher:init()
	self.listenerByKey = {}
	self.listenerByView = {}
end

function EventDispatcher:registerCustomListener(key, obj, func)
	if not key then
		print("监听器名称错误")
		return
	end
	if not obj then
		print("监听器对象错误")
		return
	end
	if not self.listenerByKey[key] then
		self.listenerByKey[key] = {}
	end
	if not self.listenerByKey[key][obj] then
		self.listenerByKey[key][obj] = func
	end
end

function EventDispatcher:removeListenerByKey(key, obj)
	if self.listenerByKey[key] then
		if obj then
			self.listenerByKey[key][obj] = nil
		else
			self.listenerByKey[key] = nil
		end
	end
end

function EventDispatcher:removeListenerByView(obj)
	if obj then
		for k,v in pairs(self.listenerByKey) do
			if v[obj] then
				v[obj] = nil
			end
		end
	end
end

function EventDispatcher:removeAllListener()
	self.listenerByKey = {}
	self.listenerByView = {}
end

function EventDispatcher:dispatch(key, ...)
	if self.listenerByKey[key] then
		for k,v in pairs(self.listenerByKey[key]) do
			v(...)
		end
	end
end

return EventDispatcher;