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
	X = 480,
	Y = 320,

}

player = {
	acceleration = 200,
	buoyancy = 0.5,
	friction = 0.1,
	radius = 7.5,
	rebound = 0.0,
	jumpSpeed = -20,
}

gravity = {
	X = 0.0,
	Y = 100.0,
}
