--[[
	Full documentation about the goals of the game, and the variables and functions available to use can be found in the project folder.
]]

--Use this function to control your worker. It will be called every frame. If your worker has to move to a specific tile to perform the move, every call will move him one step closer to his targeted tile.
function decideNextMove ()
--This AI utilizes all aspects of the game (not optimally). 
--Besides trying to collect all the resources needed to finish his monument, he will also do the following:
--He will hire thieves if his opponent has more money than him.
--He will hire guards if the opponent has thieves.
--He will upgrade his tools if he has the resources to do so (unless he could also build part of his monument, in which case the monument gets priority).
	if(opponentThieves-1 > guards and money >= guardHirePriceMoney and metal >= guardHirePriceMetal)  then
		hireGuard()
	elseif(guards > opponentThieves-1 and guards > 0) then
		fireGuard()
	elseif(money >= thiefHirePriceMoney and metal >= thiefHirePriceMetal and opponentMoney > money) then
		hireThief()
	elseif(money >= monumentBaseBuildPriceMoney+(monumentExtraBuildPriceMoney*monumentBuildStage) and metal >= monumentBaseBuildPriceMetal+(monumentExtraBuildPriceMetal*monumentBuildStage)) then
		buildMonument()
	elseif(money >= toolLevelUpgradePriceMoney+(toolLevelExtraUpgradePriceMoney*toolLevel) and metal >= toolLevelUpgradePriceMetal+(toolLevelExtraUpgradePriceMetal*toolLevel)) then
		upgradeTools()
	elseif(money < toolLevelUpgradePriceMoney+(toolLevelExtraUpgradePriceMoney*toolLevel) and metal > toolLevelUpgradePriceMetal+(toolLevelExtraUpgradePriceMetal*toolLevel)) then
		sellMetal()
	elseif(energy < huntingEnergyCost*2 and food == 0) then 
		goHunting()
	elseif(energy < huntingEnergyCost*2 and food > 0) then
		eatFood()
	elseif(energy >= miningEnergyCost -(toolLevelEnergyReduce*toolLevel)) then
		goMining()
	end
	return 0
end