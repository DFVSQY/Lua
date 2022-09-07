--[[
	用于记录一些窗口配置
]]

width, height = 50, 30

background = {r = 0.3, g = 0.5, b = 0.2}

local color_names = {"WHITE", "RED", "GREEN", "BLUE"}
for _, color_name in ipairs(color_names) do
	local color = _G[color_name]
	if color then
		print("name:", color_name, "red:", color.red, "green:", color.green, "blue:", color.blue)
	end
end

function CalRectArea(rectW, rectH)
	print("lua CalRectArea call", rectW, rectH)
	return rectW * rectH
end

if c_sin then
	local result = c_sin(math.pi / 6)
	print("sin30 = " .. result)
end

if c_get_dir_files then
	print("=== beign files ===")
	local files, err_msg = c_get_dir_files("./learn")
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
	print("use_math_lib:", tostring(use_math_lib))
	if use_math_lib then
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