print("hello world")

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