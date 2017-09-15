PhysicsFPS = 60
StepsPerRender = 1
frameskip = false
limitFPS = true

fpsText = {
	font = "consola.ttf",
	size = 12,
	position = {
		X=0,
		Y=0,
	},
}

winSize = {
--GBC
	--X = 160,
	--Y = 144,
--GBA
	--X = 240,
	--Y = 160,
--NES
	--X = 256,
	--Y = 240,
--Usado
--	X = 480,
	X = 1000,
	--Y = 320,
	Y = 600
}

--[[Platformer
player = {

	acceleration = 20,
	buoyancy = 3.0,
	friction = 0.1,
	radius = 7.5,
	rebound = 0.0,
	jumpSpeed = -10,
}

gravity = {
	X = 0.0,
	Y = 20.0,
}
--]]


---[[Top Down 
player = {
	acceleration = 30,
	buoyancy = 4.0,
	friction = 2.0,
	radius = 7.5,
	rebound = 0.0,
	jumpSpeed = 0,
}

gravity = {
	X = 0.0,
	Y = 0.0,
}
--]]

