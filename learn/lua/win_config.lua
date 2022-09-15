--[[
	用于记录一些窗口配置
]]

_G.width, _G.height = 50, 30

_G.background = {r = 0.3, g = 0.5, b = 0.2}

local color_names = {"WHITE", "RED", "GREEN", "BLUE"}
for _, color_name in ipairs(color_names) do
	local color = _G[color_name]
	if color then
		print("name:", color_name, "red:", color.red, "green:", color.green, "blue:", color.blue)
	end
end

function _G.CalRectArea(rectW, rectH)
	print("lua CalRectArea call", rectW, rectH)
	return rectW * rectH
end

if _G.c_sin then
	local result = _G.c_sin(math.pi / 6)
	print("sin30 = " .. result)
end

if _G.c_get_dir_files then
	print("=== beign files ===")
	local files, err_msg = _G.c_get_dir_files("./learn")
	if not files then
		print("open dir error:", err_msg)
	else
		for _, file in ipairs(files) do
			print(file)
		end
	end
	print("=== end files ===")
end

-- 用于测试C Module注册的函数
do
	if _G.use_math_lib then
		local mymath = require "mymathlib"
		if not mymath then
			print("can't load mymathlib file")
		else
			local sum = mymath.add(3, 2)
			print("sum:", sum)
		local sub = mymath.sub(3, 2)
		print("sub:", sub)
		end
	end
end

_G.array_t = {3, 5, 8, 10}
_G.each_array_e = function(value)
	return value * value
end

if _G.c_split_string then
	local ss = _G.c_split_string("file:edit:selection:view", ":")
	print("===== beign c split string")
	for index, value in ipairs(ss) do
		print(index, value)
	end
	print("===== end c split string")
end

if _G.c_tconcat then
	local t = {"visual", "studio", "code"}
	local s = _G.c_tconcat(t)
	print("c_tconcat result:" .. s)
end

if _G.c_counter then
	local c1 = _G.c_counter()
	print("c1:", c1(), c1(), c1())

	local c2 = _G.c_counter()
	print("c2:", c2(), c2(), c2())
end