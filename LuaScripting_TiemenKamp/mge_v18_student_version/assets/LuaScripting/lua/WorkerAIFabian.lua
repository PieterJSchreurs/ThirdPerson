--[[
	Full documentation about the goals of the game, and the variables and functions available to use can be found in the project folder.
]]


--Use this function to control your worker. It will be called every frame. If your worker has to move to a specific tile to perform the move, every call will move him one step closer to his targeted tile.
function decideNextMove()
	--function fabianWins()

	if(food == 0 and energy >= huntingEnergyCost) then --It is always smart to have some food in storage, so if you have none, go hunting!
		goHunting()
	elseif(toolLevel < opponentToolLevel + 2) then
		if(toolLevelUpgradePriceMoney + toolLevelExtraUpgradePriceMoney * toolLevel > money) then
			if(metalSellPrice > foodSellPrice and metal > 0) then 
				sellMetal()
			elseif(metalSellPrice < foodSellPrice and food > 0) then
				sellFood()
			elseif(metalSellPrice > foodSellPrice and energy >= miningEnergyCost) then
				goMining()
			elseif(metalSellPrice < foodSellPrice and energy >= huntingEnergyCost) then
				goHunting()
			else
				eatFood()
			end
		elseif(toolLevelUpgradePriceMetal + toolLevelExtraUpgradePriceMetal * toolLevel > metal) then
			if(energy >= miningEnergyCost) then
				goMining()
			else
				eatFood()
			end
		else
			upgradeTools()
		end
	elseif(thieves < 2) then
		if(thiefHirePriceMoney > money) then
			if(metalSellPrice > foodSellPrice and metal > 0) then 
				sellMetal()
			elseif(metalSellPrice < foodSellPrice and food > 0) then
				sellFood()
			elseif(metalSellPrice > foodSellPrice and energy >= miningEnergyCost) then
				goMining()
			elseif(metalSellPrice < foodSellPrice and energy >= huntingEnergyCost) then
				goHunting()
			end
		elseif(thiefHirePriceMetal > metal) then
			if(energy >= miningEnergyCost) then
				goMining()
			else
				eatFood()
			end
		else
			hireThief()
		end
	elseif(guards < opponentGuards / 2) then
		if(guardHirePriceMoney > money) then
			if(metalSellPrice > foodSellPrice and metal > 0) then 
				sellMetal()
			elseif(metalSellPrice < foodSellPrice and food > 0) then
				sellFood()
			elseif(metalSellPrice > foodSellPrice and energy >= miningEnergyCost) then
				goMining()
			elseif(metalSellPrice < foodSellPrice and energy >= huntingEnergyCost) then
				goHunting()
			end
		elseif(guardHirePriceMetal > metal) then
			if(energy >= miningEnergyCost) then
				goMining()
			else
				eatFood()
			end
		else
			hireGuard()
		end
	elseif(monumentBaseBuildPriceMoney + monumentExtraBuildPriceMoney * monumentBuildStage > money) then
		if(metalSellPrice > foodSellPrice and metal > 0) then 
			sellMetal()
		elseif(metalSellPrice < foodSellPrice and food > 0) then
			sellFood()
		elseif(metalSellPrice > foodSellPrice and energy >= miningEnergyCost) then
			goMining()
		elseif(metalSellPrice < foodSellPrice and energy >= huntingEnergyCost) then
				goHunting()
		else
			eatFood()
		end
	elseif(monumentBaseBuildPriceMetal + monumentExtraBuildPriceMetal * monumentBuildStage > metal) then
		if(energy >= miningEnergyCost) then
			goMining()
		else
			eatFood()
		end
	else 
		buildMonument()
	end

	--Now it is up to you to finish this AI, so that it will build that monument as fast as possible.

	return 0 --This line should stay here, it tells the main program when you are done deciding on what your worker should do.
end