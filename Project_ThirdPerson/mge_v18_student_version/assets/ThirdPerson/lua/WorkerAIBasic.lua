--[[
	Full documentation about the goals of the game, and the variables and functions available to use can be found in the project folder.
]]

function decideNextMove() --This AI will focus purely on building his monument as fast as possible, it completely ignores tool levels, thieves, and guards.
	if(money >= monumentBaseBuildPriceMoney+(monumentExtraBuildPriceMoney*monumentBuildStage) and metal >= monumentBaseBuildPriceMetal+(monumentExtraBuildPriceMetal*monumentBuildStage)) then
		buildMonument()
	elseif(money < monumentBaseBuildPriceMoney+(monumentExtraBuildPriceMoney*monumentBuildStage) and metal > monumentBaseBuildPriceMetal+(monumentExtraBuildPriceMetal*monumentBuildStage)) then
		sellMetal()
	elseif(energy < huntingEnergyCost*2 and food == 0) then
		goHunting()
	elseif(energy < huntingEnergyCost*2 and food > 0) then
		eatFood()
	elseif(energy >= miningEnergyCost-(toolLevelEnergyReduce*toolLevel)) then
		goMining()
	end

	return 0
end