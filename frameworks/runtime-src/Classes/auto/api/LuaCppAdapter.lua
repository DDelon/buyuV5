
--------------------------------
-- @module LuaCppAdapter
-- @parent_module 

--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getPlayerId 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] changeTimeline 
-- @param self
-- @param #int timelineIndex
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] playerFire 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] startNets 
-- @param self
-- @param #cc.Layer pNetsLayer
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] setCannon 
-- @param self
-- @param #cc.Node pCannon
-- @param #int playerId
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] openRankWeb 
-- @param self
-- @param #string url
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] luaUseCppFun 
-- @param self
-- @param #map_table data
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] exitGame 
-- @param self
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] addPlayer 
-- @param self
-- @param #map_table map
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getPathPos 
-- @param self
-- @param #int pathId
-- @param #int frame
-- @return vec3_table#vec3_table ret (return value: vec3_table)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] callFish 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] bindUI 
-- @param self
-- @param #map_table map
-- @param #ccui.TextBMFont text
-- @param #ccui.TextBMFont addText
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] callLuaFunc 
-- @param self
-- @param #string globalTabName
-- @param #string funcName
-- @param #string type
-- @param #map_table val
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] setLuaNode 
-- @param self
-- @param #int type
-- @param #cc.Node pManagerLayer
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getConfigData 
-- @param self
-- @param #string configName
-- @param #string keyID
-- @param #string keyName
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] loadDataBin 
-- @param self
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] setFishState 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getConfigDataByName 
-- @param self
-- @param #string keyName
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] removePlayerBullet 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getNBombKilledFishes 
-- @param self
-- @param #int bombLv
-- @param #vec2_table pos
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getLuaValue 
-- @param self
-- @param #string globalTabName
-- @param #string valName
-- @param #cc.Value val
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] startGame 
-- @param self
-- @param #cc.Layer pManagerLayer
-- @param #cc.Layer pFishLayer
-- @param #map_table data
-- @param #array_table killedFishes
-- @param #array_table calledFishes
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getVIPResId 
-- @param self
-- @param #int playerId
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getCurFrame 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] addDebugLayer 
-- @param self
-- @param #cc.Layer pLayer
-- @param #string roomId
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] setGunRate 
-- @param self
-- @param #int gunRate
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] otherPlayerBulletCollision 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] updateNBombRate 
-- @param self
-- @param #int rate
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] startBullet 
-- @param self
-- @param #cc.Layer pBulletLayer
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] bulletChangeTarget 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] fishAccelerateOut 
-- @param self
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] frameRunning 
-- @param self
-- @param #int frame
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] updateThunderRate 
-- @param self
-- @param #int rate
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] debugLog 
-- @param self
-- @param #string log
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] fishTimeLineCome 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] fishSwithColor 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] syncFrame 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] fishGroupCome 
-- @param self
-- @param #map_table data
-- @return LuaCppAdapter#LuaCppAdapter self (return value: LuaCppAdapter)
        
--------------------------------
-- 
-- @function [parent=#LuaCppAdapter] getInstance 
-- @param self
-- @return LuaCppAdapter#LuaCppAdapter ret (return value: LuaCppAdapter)
        
return nil
