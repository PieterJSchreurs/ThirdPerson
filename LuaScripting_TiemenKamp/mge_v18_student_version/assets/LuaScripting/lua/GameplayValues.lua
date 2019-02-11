
--Constant gameplay values					Changing these values will impact the overall flow and balance of the game. 

gridWidth = 11;								--Min size is 7 by 7, max size is 100 by 100. A large size is not advised, as the pathfinding will be extremely taxing on your performance.
gridHeight = 11;							--Min size is 7 by 7, max size is 100 by 100. A large size is not advised, as the pathfinding will be extremely taxing on your performance.
tileSize = 2.0;								--THIS IS BROKEN AS OF RIGHT NOW, DO NOT TOUCH!

miningEnergyCost = 10;						--How much energy does it cost per mining action.
miningMetalGain = 9;						--How much metal do you gain per mining action.
huntingEnergyCost = 10;						--How much energy does it cost per hunting action.
huntingFoodGain = 18;						--How much food do you gain per hunting action.

metalSellPrice = 4;							--How much money do you gain for selling one metal.
metalBuyPrice = 6;							--How much money do you pay for buying one metal.
foodSellPrice = 2;							--How much money do you gain for selling one food.
foodBuyPrice = 3;							--How much money do you pay for buying one food.
foodEnergyGain = 5;							--How much energy do you gain for eating one food.

thiefHirePriceMetal = 5;					--How much metal does it cost to hire a thief.
thiefHirePriceMoney = 20;					--How much money does it cost to hire a thief.
thiefStealPercentage = 0.020;				--How much money and metal does a thief steal from your opponent per action (percentage).

guardHirePriceMetal = 2;					--How much metal does it cost to hire a guard.
guardHirePriceMoney = 10;					--How much money does it cost to hire a guard.
guardSalaryAmount = 0.2;					--How much money do you have to pay the guards per action.
guardCatchRate = 25; 						--What is the chance per action for a guard to catch an opponent's thief. (percentage * 10, so 25 = 2.5%)

toolLevelUpgradePriceMetal = 5;				--How much metal does it cost to upgrade your tools.
toolLevelExtraUpgradePriceMetal = 5;		--How much extra metal does it cost to upgrade your tools per tool level. (first upgrade costs base, second upgrade costs base+extra, third upgrade costs base+extra+extra, etc.)
toolLevelUpgradePriceMoney = 20;			--How much money does it cost to upgrade your tools.
toolLevelExtraUpgradePriceMoney = 20;		--How much extra money does it cost to upgrade your tools per tool level. (first upgrade costs base, second upgrade costs base+extra, third upgrade costs base+extra+extra, etc.)
toolLevelEnergyReduce = 1;					--How much does one tool level reduce the energy cost of the mining and hunting action.

monumentBaseBuildPriceMetal = 12;			--How much metal does it cost to build a monument stage.
monumentExtraBuildPriceMetal = 4;			--How much extra metal does it cost to build a monument stage per build stage. (first stage costs base, second stage costs base+extra, third stage costs base+extra+extra, etc.)
monumentBaseBuildPriceMoney = 50;			--How much money does it cost to build a monument stage.
monumentExtraBuildPriceMoney = 15;			--How much extra money does it cost to build a monument stage per build stage. (first stage costs base, second stage costs base+extra, third stage costs base+extra+extra, etc.)
monumentBuildStageTargetAmount = 3;			--How many monument build stages do you need to win the game.