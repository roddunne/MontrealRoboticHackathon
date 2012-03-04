local map = { [' ']='stop', w='forward', s='backward', a='left', d='right' }

function love.keypressed(key)
	local cmd = map[key]
	if not cmd then return end
	print(cmd)
	local file = io.open('../lua/RobotControl.json', 'w')
	file:write(string.format('{"cmd":"%s"}', cmd))
	file:close()
end
