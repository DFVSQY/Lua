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

--[[
	Constructors are expressions that create and initialize tables. They are a distinctive feature of Lua and
	one of its most useful and versatile mechanisms. The simplest constructor is the empty constructor, {},
	Constructors also initialize lists.
]]
do
	local t1 = {"file", "edit", "selection"}

	-- This previous line is equivalent to these commands
	local t2 = {}
	t2[1] = "file"
	t2[2] = "edit"
	t2[3] = "selection"

	for index, value in ipairs(t1) do
		print(index, value)
	end

	-- output:
	-- 1       file
	-- 2       edit
	-- 3       selection

	for index, value in ipairs(t2) do
		print(index, value)
	end

	-- output:
	-- 1       file
	-- 2       edit
	-- 3       selection
end

--[[
	Lua also offers a special syntax to initialize a record-like table.
	The original expression, however, is faster, because Lua creates the table already with the right size.
]]
do
	-- faster
	local t1 = {x = 10, y = 20}
	print(t1.x, t1.y)							-- 10	20

	-- be equivalent to:
	local t2 = {}
	t2.x = 10
	t2["y"] = 20
	print(t2.x, t2.y)							-- 10	20
end

--[[
	We can mix record-style and list-style initializations in the same constructor.
]]
do
	local polyline = {
		color="blue",
		thickness=2,
		npoints=4,
		{x=0,   y=0},    						-- polyline[1]
		{x=-10, y=0},    						-- polyline[2]
		{x=-10, y=1},    						-- polyline[3]
		{x=0,   y=1}     						-- polyline[4]
   }
   print(polyline[2].x, polyline[2].y)			-- -10	0

	--[[
		Those two constructor forms have their limitations. For instance, we cannot initialize fields with negative indices,
		nor with string indices that are not proper identifiers. For such needs, there is another, more general, format.
		In this format, we explicitly write each index as an expression, between square brackets.
	]]
	local opnames = {["+"] = "add", ["-"] = "sub", ["*"] = "mul", ["/"] = "div"}
	local i = 20
	local s = "-"
	local a = {[i + 0] = s, [i + 1] = s..s, [i + 2] = s .. s .. s}

	print(opnames[s])    						-- sub
	print(a[22])         						-- ---

	-- We can always use a semicolon instead of a comma in a constructor.
	-- This facility is a leftover from older Lua versions and it is seldom used nowadays.
	local color = {r = 10; g = 20; b = 5}
	print(color.r, color.g, color.b)			-- 10	20	 5
end

--[[
	To represent a conventional array or a list, we simply use a table with integer keys.
	There is neither a way nor a need to declare a size; we just initialize the elements we need.

	We call such a list without holes a sequence(start index is 1), holes are nil elements inside array/list.
	For sequences, Lua offers the length operator (#). As we have seen, on strings it gives the number of
	bytes in the string. On tables, it gives the length of the sequence represented by the table.

	More precisely, a sequence is a table where the positive numeric keys comprise a set {1,...,n} for some n.
	(Remember that any key with value nil is actually not in the table.)
	In particular, a table with no numeric keys is a sequence with length zero.
]]
do
	local a = {}
	for i = 1, 10 do
		a[i] = i
	end
	print(#a)									-- 10

	local color = {r = 20, g = 10, b = 30}
	print(#color)								-- 0
end

--[[
	We can traverse all key–value pairs in a table with the pairs iterator.

	Due to the way that Lua implements tables, the order that elements appear in a traversal is undefined.
	The same program can produce different orders each time it runs.
	The only certainty is that each element will appear once during the traversal
]]
do
	local t = {"file", "edit", name = "vscode", language = "en"}
	for key, value in pairs(t) do
		print(key, value)
	end

	-- output:
	-- 1       file
	-- 2       edit
	-- language        en
	-- name    vscode
end

--[[
	For lists, we can use the ipairs iterator.
]]
do
	local t = {"file", "edit", "selection", "view", "go", "run"}
	for index, value in ipairs(t) do
		print(index, value)
	end

	-- output:
	-- 1       file
	-- 2       edit
	-- 3       selection
	-- 4       view
	-- 5       go
	-- 6       run

	local tt = {
		[1] = "file",
		[2] = "edit",
		[4] = "view",
		[6] = "run"
	}
	for index, value in ipairs(tt) do
		print(index, value)
	end

	-- output:
	-- 1       file
	-- 2       edit
end

--[[
	Another way to traverse a sequence is with a numerical for.
]]
do
	local t = {"file", "edit", "selection", "view", "go", "run"}
	for i = 1, #t do
		print(i, t[i])
	end

	-- output:
	-- 1       file
	-- 2       edit
	-- 1       file
	-- 2       edit
	-- 3       selection
	-- 4       view
	-- 5       go
	-- 6       run
end

--[[
	Some programming languages, such as C#, offer a safe navigation operator (written as ?. in C#).
	When we write a ?. b and a is nil, the result is also nil, instead of an error. Using that operator,
	we could write our previous example like this:

      zip = company?.director?.address?.zipcode

	Lua does not offer a safe navigation operator,
	However, we can emulate it in Lua with a bit of extra notation.
]]
do
	local E = {}     			-- can be reused in other similar expressions
    local company = _G.company

    local zip = (((company or E).director or E).address or E).zipcode
	print(zip)					-- nil
end

--[[
	The table library offers several useful functions to operate over lists and sequences.
]]
do
	local t = {10, 30, 40, 50}
	table.insert(t, 2, 20)
	for index, value in ipairs(t) do
		print(index, value)
	end

	-- output:
	-- 1       10
	-- 2       20
	-- 3       30
	-- 4       40
	-- 5       50

	table.remove(t, 3)
	for index, value in ipairs(t) do
		print(index, value)
	end

	-- output:
	-- 1       10
	-- 2       20
	-- 3       40
	-- 4       50

	-- When called without a position, it removes the last element of the sequence.
	table.remove(t)
	for index, value in ipairs(t) do
		print(index, value)
	end

	-- output:
	-- 1       10
	-- 2       20
	-- 3       40
end

--[[
	Lua 5.3 has introduced a more general function for moving elements in a table.
	The call table.move(a, f, e, t) moves the elements in table a from index f until e
	(both inclusive) to position t.

	Moves elements from table a1 to table a2, performing the equivalent to the following multiple assignment:
	a2[t],... = a1[f],...,a1[e].
	The default for a2 is a1.
	The destination range can overlap with the source range.
	The number of elements to be moved must fit in a Lua integer.
]]
do
	local t = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	local t1 = {}
	table.move(t, 4, 8, 1, t1)
	for index, value in ipairs(t1) do
		print(index, value)
	end

	-- output:
	-- 1       4
	-- 2       5
	-- 3       6
	-- 4       7
	-- 5       8

	table.move(t, 3, 5, 2)
	for index, value in ipairs(t) do
		print(index, value)
	end

	-- output:
	-- 1       1
	-- 2       3
	-- 3       4
	-- 4       5
	-- 5       5
	-- 6       6
	-- 7       7
	-- 8       8
	-- 9       9
	-- 10      10

	local tt1 = {4, 5, 6}
	local tt2 = {1, 2, 3}
	table.move(tt1, 1, #tt1, #tt2 + 1, tt2)
	for index, value in ipairs(tt2) do
		print(index, value)
	end

	-- output:
	-- 1       1
	-- 2       2
	-- 3       3
	-- 4       4
	-- 5       5
	-- 6       6
end