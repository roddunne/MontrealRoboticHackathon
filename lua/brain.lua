
rs232 = require("luars232")

local err = io.stderr

-- API key for uploading to cloud
local APIKEY='22DDB1CB2F8B4486'

-- Serial port name
--port_name = '/dev/tty.usbmodemfa131'
port_name = '/dev/tty.usbserial-A4016T68'
--port_name = '/dev/tty.Bluetooth_V3-DevB-1'

-- Local copy of sensor values (by name)
local sensors = {}

-- Map of command names to raw character commands the Arduino sketch expects
local command_map = { stop=' ', forward='w', backward='s', left='a', right='d'}

-- Manage upload times
local upload_rate = 30
local upload_time = os.time() + upload_rate

-- Uploads sensor values to cloud
local function upload()
	local data = string.format('key=%s&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s&field7=%s&field8=%s',
		APIKEY,
		sensors.light or '', sensors.temperature or '',
		sensors.flame or '', sensors.sound or '',
		sensors.front_ir or '', sensors.rear_ir or '',
		sensors.vibration or '', sensors.tilt or '')
	print('uploading:', data)
	os.execute('curl -d "' .. data .. '" http://bots.myrobots.com/update')
end

-- Process a line with no newlines
local function process_line(line)
	--print(line)
	local token, name, value = line:match('(%w+) ([%w_]+) (%w+)')
	if token == 'sensor' then
		--print(name, value)
		sensors[name] = value
	end
end

-- Get a remote command
local function get_remote_command()
	--os.execute('curl --silent http://192.168.0.111/RobotCommand.json -o RobotCommand.json')
	local file = io.open('RobotControl.json', 'r')
	local json = file:read('*all')
	file:close()
	local cmd = json:match('%s*{%s*"%w+"%s*:%s*"(%w+)"%s*}%s*')
	if not cmd then
	    cmd = json:match("%s*{%s*'%w+'%s*:%s*'(%w+)'%s*}%s*")
	end
	return cmd
end

-- open port
local e, p = rs232.open(port_name)
if e ~= rs232.RS232_ERR_NOERROR then
	-- handle error
	err:write(string.format("can't open serial port '%s', error: '%s'\n",
			port_name, rs232.error_tostring(e)))
	return
end

-- set port settings
assert(p:set_baud_rate(rs232.RS232_BAUD_115200) == rs232.RS232_ERR_NOERROR)
assert(p:set_data_bits(rs232.RS232_DATA_8) == rs232.RS232_ERR_NOERROR)
assert(p:set_parity(rs232.RS232_PARITY_NONE) == rs232.RS232_ERR_NOERROR)
assert(p:set_stop_bits(rs232.RS232_STOP_1) == rs232.RS232_ERR_NOERROR)
assert(p:set_flow_control(rs232.RS232_FLOW_OFF)  == rs232.RS232_ERR_NOERROR)
-- flow values: RS232_FLOW_OFF RS232_FLOW_XON_XOFF RS232_FLOW_HW

err:write(string.format("OK, port open with values '%s'\n", tostring(p)))

local time = os.time()
local cmd = 'a'

-- any incomplete data we might have read
local incomplete_line = ''

local latest_cmd = 'stop'

while true do
	-- get sensor values
	while true do
		-- read 256 bytes with 100ms timeout
		local e, data_read, size = p:read(256, 100)
		assert(e == rs232.RS232_ERR_NOERROR)
		if data_read then
			-- process data a line at a time
			while true do
				local i = data_read:find('\r\n')
				if not i then break end
				local line = incomplete_line .. data_read:sub(1, i-1)
				process_line(line)
				incomplete_line = ''
				data_read = data_read:sub(i+2)
			end
			assert(data_read)
			incomplete_line = data_read
		else
			-- no more data available right now
			break;
		end
	end

	-- control
	local cmd = get_remote_command()
	if latest_cmd ~= cmd then
		-- send command to robot
		print('COMMAND', cmd, command_map[cmd])
		local raw_cmd = command_map[cmd]
		if raw_cmd then
			p:write(raw_cmd)
		end
		latest_cmd = cmd
	end

	-- upload
	if upload_time <= os.time() then
	    upload()
	    upload_time = os.time() + upload_rate
	end
end

-- write without timeout
e, len_written = p:write("test")
assert(e == rs232.RS232_ERR_NOERROR)

-- write with timeout 100 msec
e, len_written = p:write("test\n", timeout)
assert(e == rs232.RS232_ERR_NOERROR)

-- close
assert(p:close() == rs232.RS232_ERR_NOERROR)
