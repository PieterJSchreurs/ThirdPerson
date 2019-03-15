--[[
	Full documentation about the goals of the game, and the variables and functions available to use can be found in the project folder.
]]


--Use this function to control your worker. It will be called every frame. If your worker has to move to a specific tile to perform the move, every call will move him one step closer to his targeted tile.
function decideNextMove()
	if(food == 0) then --It is always smart to have some food in storage, so if you have none, go hunting or else buy food!
		if(energy >= huntingEnergyCost) then
			goHunting()
		elseif(money >= foodSellPrice) then
			buyFood()
		end
	elseif(metal >= monumentBaseBuildPriceMetal + monumentExtraBuildPriceMetal and money >= monumentBaseBuildPriceMoney + monumentExtraBuildPriceMoney) then --When we have enough to build, we want to build!
		buildMonument()
	elseif(money >= metal and energy >= miningEnergyCost) then --If we are as rich or richer than the amount of metal we have...
		goMining()
	elseif(metal > money) then --If we have more metal than money...
		sellMetal()
	elseif(food > 0) then --After all that hard work you have no energy left. If you have food to replenish your energy, eat some!
		eatFood()
	end

	return 0 --This line should stay here, it tells the main program when you are done deciding on what your worker should do.
end