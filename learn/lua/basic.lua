-- Opens the named file and executes its contents as a Lua chunk
dofile("learn/lua/test_chunk.lua")		-- test_chunk:     1
dofile("learn/lua/test_chunk.lua")		-- test_chunk:     2

-- You should avoid identifiers starting with an underscore followed by
-- one or more upper-case letters (e.g.,_VERSION)
-- they are reserved for special uses in Lua.
print(_G._VERSION)						-- output: Lua 5.3

--[[
	this is a
	multiline comment
]]

--[[ To reactivate the code, we add a single hyphen to the line.
	print("file edit selection")
--]]

-- Lua  needs  no  separator  between  consecutive  statements,
-- but  we  can  use  a  semicolon  if  we  wish.
-- the following four chunks are all valid and equivalent
do
	do						-- 001
		local a = 1
		local b = a * 2
		print(a, b)
	end

	do						-- 002
		local a = 1;
		local b = a * 2;
		print(a, b)
	end

	do						-- 003
		local a = 1; local b = a * 2
		print(a, b)
	end

	do						-- 004
		local a = 1  local b = a * 2    -- ugly, but valid
		print(a, b)
	end
end

-- There are eight basic types in Lua:
-- nil, Boolean, number, string, userdata, function, thread, and table
do
	print(type(nil))            --> nil
	print(type(true))           --> boolean
	print(type(10.4 * 3))       --> number
	print(type("Hello world"))  --> string
	print(type(io.stdin))       --> userdata
	print(type(print))          --> function
	print(type(type))           --> function
	print(type({}))             --> table
	print(type(type(X)))        --> string

	local co = coroutine.create(function()
		print("hi")
	end)
	print(type(co))   --> thread: 0x8071d98
end

-- The result of the and operator is its first operand if that operand is false;
-- otherwise, the result is its second operand.
-- The result of the or operator is its first operand if it is not false;
-- otherwise, the result is its second operand
do
	print(4 and 5)         	--> 5
    print(nil and 13)      	--> nil
    print(false and 13)    	--> false
    print(0 or 5)          	--> 0
    print(false or "hi")   	--> "hi"
	print(nil or false)    	--> false
end

-- The not operator always gives a Boolean value
do
	print(not nil)      	--> true
    print(not false)    	--> true
    print(not 0)        	--> false
    print(not not 1)    	--> true
	print(not not nil)  	--> false
end