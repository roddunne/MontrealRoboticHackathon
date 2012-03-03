
rs232 = require("luars232")

-- Linux
--port_name = '/dev/tty.usbmodemfa131'
port_name = '/dev/tty.usbserial-A4016T68'

-- (Open)BSD
-- port_name = "/dev/cua00"

-- Windows
-- port_name = "COM1"

-- ANSI C (and therefore Lua) has no built-in sleep function
-- so make one here
local clock = os.clock
function sleep(n)  -- seconds
  local t0 = clock()
  while clock() - t0 <= n do end
end

local out = io.stderr

-- open port
local e, p = rs232.open(port_name)
if e ~= rs232.RS232_ERR_NOERROR then
	-- handle error
	out:write(string.format("can't open serial port '%s', error: '%s'\n",
			port_name, rs232.error_tostring(e)))
	return
end

-- set port settings
assert(p:set_baud_rate(rs232.RS232_BAUD_115200) == rs232.RS232_ERR_NOERROR)
assert(p:set_data_bits(rs232.RS232_DATA_8) == rs232.RS232_ERR_NOERROR)
assert(p:set_parity(rs232.RS232_PARITY_NONE) == rs232.RS232_ERR_NOERROR)
assert(p:set_stop_bits(rs232.RS232_STOP_1) == rs232.RS232_ERR_NOERROR)
assert(p:set_flow_control(rs232.RS232_FLOW_OFF)  == rs232.RS232_ERR_NOERROR)

out:write(string.format("OK, port open with values '%s'\n", tostring(p)))

local time = os.time()
local cmd = 'a'

-- seems like our robot doesn't obey in the first few seconds?
sleep(3)
p:write('s')

while true do
	-- listen to what the robot says
	while true do
		-- read with timeout
		local read_len = 1 -- read one byte
		local timeout = 100 -- in miliseconds
		local e, data_read, size = p:read(read_len, timeout)
		assert(e == rs232.RS232_ERR_NOERROR)
		if data_read then
			io.write(data_read)
			io.flush()
		else
			break;
		end
	end

	-- tell the robot to do something
	if time < os.time() then
	    -- send command
	    --print('COMMANDING', cmd)
	    --p:write(cmd)
	    --cmd = cmd == 'a' and 'd' or 'a'
	    --time = os.time()
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
