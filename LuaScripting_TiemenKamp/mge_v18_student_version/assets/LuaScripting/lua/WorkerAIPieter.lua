--[[
	Full documentation about the goals of the game, and the variables and functions available to use can be found in the project folder.
]]


--Use this function to control your worker. It will be called every frame. If your worker has to move to a specific tile to perform the move, every call will move him one step closer to his targeted tile.
function decideNextMove()

    if(((metal > toolLevelUpgradePriceMetal + (toolLevelExtraUpgradePriceMetal * toolLevel)) or (metal > monumentBaseBuildPriceMetal + (toolLevelExtraUpgradePriceMetal * monumentBuildStage))) and (money < monumentBaseBuildPriceMoney + (monumentExtraBuildPriceMoney * monumentBuildStage) or money < toolLevelUpgradePriceMoney + (toolLevelExtraUpgradePriceMoney * toolLevel))) then
        sellMetal()
    elseif(opponentThieves > guards and money > guardHirePriceMoney and metal > guardHirePriceMetal) then
        hireGuard()
    elseif(opponentGuards < thieves -1 and money > thiefHirePriceMoney and metal > guardHirePriceMetal) then
        hireThief()
    elseif(opponentThieves < guards - 2) then
        fireGuard()
    elseif(toolLevel < opponentToolLevel -1 and metal > toolLevelUpgradePriceMetal + (toolLevelExtraUpgradePriceMetal * toolLevel) and money > toolLevelUpgradePriceMoney + (toolLevelExtraUpgradePriceMoney * toolLevel)) then
        upgradeTools()
    elseif(food == 0 and energy >= huntingEnergyCost and money > foodBuyPrice) then
        buyFood()
    elseif(monumentBuildStage < monumentBuildStageTargetAmount and metal > monumentBaseBuildPriceMetal + (toolLevelExtraUpgradePriceMetal * monumentBuildStage) and money > monumentBaseBuildPriceMoney + (monumentExtraBuildPriceMoney * monumentBuildStage)) then
        buildMonument()
    elseif(food == 0 and energy >= huntingEnergyCost) then --It is always smart to have some food in storage, so if you have none, go hunting!
        goHunting()
    elseif(energy >= miningEnergyCost) then --If you already have some food in storage, and you have enough energy to go mining, go mining!
        goMining()
    elseif(food > 0) then --You have no energy left after all that hard work. If you have food to replenish your energy, eat some!
        eatFood()
	end
    return 0 --This line should stay here, it tells the main program when you are done deciding on what your worker should do.
end