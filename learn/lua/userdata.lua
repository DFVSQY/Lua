--[[
	用于测试userdata示例
]]

if _G.use_c_array then
	local array = require "array"
	local a = array.new(1000)
	for i = 1, 1000 do
		array.set(a, i, i % 2 == 0) -- a[i] == (i % 2) == 0
	end
	print(array.get(a, 10)) -- true
	print(array.get(a, 11)) -- false
	print(array.size(a))	-- 1000
end

if _G.use_c_array_meta then
	local array_meta = require "arraymeta"

	--[[
		此时会检测到userdata类型错误，输出：
		bad argument #1 to 'size' (LuaBook.array expected, got FILE*)
	]]
	array_meta.size(io.stdin)
end