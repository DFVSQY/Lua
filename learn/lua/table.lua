--[[
	Tables are the main (in fact, the only) data structuring mechanism in Lua, and a powerful one.
	We use tables to represent arrays, sets, records, and many other data structures in a simple, uniform,
	and efficient way. Lua  uses  tables  to  represent  packages  and  objects  as  well
]]

--[[
	Table in Lua is essentially an associative array. A table is an array that accepts not only numbers as indices,
	but also strings or any other value of the language (except nil).
]]
do
	local success, msg = pcall(function()
		local t = {}
		print(t[nil])														-- nil (read index can be nil)
	end)
	print(success, msg)														-- true nil

	success, msg = pcall(function()
		local t = {}
		t[nil] = 5															-- run error, because index is nil (write index can't be nil)
		print(t[nil])
	end)
	print(success, msg)														-- false	learn/lua/table.lua:20: table index is nil
end

--[[
	Create tables by means of a constructor expression.

	Programs manipulate only references (or pointers) to table.
	Lua never does hidden copies or creation of new tables behind the scenes.

	When a program has no more references to a table,
	the garbage collector will eventually delete the table and reuse its memory.
]]
do
	local t = {
		a = "A",
		b = "B",
		c = "C"
	}
	t["d"] = "D"

	for key, value in pairs(t) do
		print(key, value)
	end

	-- output:
	-- b       B
	-- c       C
	-- d       D
	-- a       A
end