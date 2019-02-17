-----How to-----
You start by designing a level using Tiled. Create two tile layers, and name them "BaseTiles" and "EntityTiles".
On the BaseTiles layer you should paint all the tiles that relate to the surroundings, like islands and harbors.
On the EntityTiles layer you should paint all the tiles that represent objects IN the surroundings, like ships, treasures, and the finish tile.

Once you are finished designing your level, you should export it as a '.csv' file. This will give you 2 .csv files, one for each layer.
Place the '..._BaseTiles.csv' file into the '..\..\Project_ThirdPerson\mge_v18_student_version\assets\ThirdPerson\baseTiles' folder.
Place the '..._EntityTiles.csv' file into the '..\..\Project_ThirdPerson\mge_v18_student_version\assets\ThirdPerson\entityTiles' folder.
Now when you start up the game, the console will print out a list of all the levels you can load. Simply type in the name of the level you want to see,
and the engine will load it up.

How to start up the game you ask?
You start by opening the project solution, which is the 'mge.sln' file in the '..\..\Project_ThirdPerson\mge_v18_student_version\_vs2015' folder.
Once that has opened up, you should see a button with a small green 'play button' icon on it near the top of your screen. Press it and the game should start up.
If there are errors, sometimes simply pressing the play button again a few times will fix the issue. (Will be looked into and fixed later.)
If the errors don't go away, contact one of the engineers to see if they can help you.


-----Controls-----
The purple ship is the ship you currently have selected, you can switch your selection with the LEFT AND RIGHT ARROW KEYS.
Use WASD to move your currently selected ship around the board (you have to keep track of the amount of moves, and the allowed moves yourself)
Use Q and E to rotate your current ship.

If you want to switch between player ships and AI ships, press the SPACEBAR.

Currently there is no shooting or destroying of ships yet, so you need to keep track of that yourself.