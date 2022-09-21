--[[
	用于测试userdata示例
]]

local array = require "array"
if array then
	local a = array.new(1000)
	for i = 1, 1000 do
		array.set(a, i, i % 2 == 0) -- a[i] == (i % 2) == 0
	end
	print(array.get(a, 10)) -- true
	print(array.get(a, 11)) -- false
	print(array.size(a))	-- 1000
end