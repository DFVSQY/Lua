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

--[[
	Like global variables, table fields evaluate to nil when not initialized. Also like global variables,
	we can assign nil to a table field to delete it. This is not a coincidence: Lua stores global variables
	in ordinary tables.

	To represent structures, we use the field name as an index.
	Lua supports this representation by providing a.name as syntactic sugar for a["name"]
]]
do
	local t = {}
	t.a = 10									-- same as t["a"]
	print(t["a"], t.a)							-- 10	10
	print(t.x, t.y)								-- nil	nil

	local key = {}
	t[key] = "key"
	print(t[key], t.key, t["key"])				-- key		nil		nil

	t[10] = 10
	print(t[10], t[010])						-- 10	10

	--[[
		2 compares equal to 2.0, both values refer to the same table entry, when used as keys.
		More specifically, when used as a key, any float value that can be converted to an integer is converted.
		For instance, when Lua executes a[2.0] = 10, it converts the key 2.0 to 2.
		Float values that cannot be converted to integers remain unaltered.
	]]
	t[2.0] = 2
	print(t[2.0], t[2])							-- 2	2

	t[3.5] = 3.50
	print(t[3.5], t[3.50])						-- 3.5	3.5
end