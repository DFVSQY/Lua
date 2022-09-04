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