--[[
	Full documentation about the goals of the game, and the variables and functions available to use can be found in the project folder.
]]


--Use this function to control your worker. It will be called every frame. If your worker has to move to a specific tile to perform the move, every call will move him one step closer to his targeted tile.
function decideNextMove()
	if(food == 0 and energy >= huntingEnergyCost) then --It is always smart to have some food in storage, so if you have none, go hunting!
		goHunting()
	elseif(energy >= miningEnergyCost) then --If you already have some food in storage, and you have enough energy to go mining, go mining!
		goMining()
	elseif(food > 0) then --You have no energy left after all that hard work. If you have food to replenish your energy, eat some!
		eatFood()
	end

	--Now it is up to you to finish this AI, so that it will build that monument as fast as possible.

	return 0 --This line should stay here, it tells the main program when you are done deciding on what your worker should do.
end