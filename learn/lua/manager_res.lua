--[[
	用于测试资源管理的示例
]]

if _G.use_c_dir_lib then
	local dir = require "dir"
	for file_name in dir.open(".") do
		print(file_name)
	end
end