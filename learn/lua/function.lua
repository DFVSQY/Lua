--[[
	Functions are the main mechanism for abstraction of statements and expressions in Lua. Functions can both carry out
	a specific task (what is sometimes called a procedure or a subroutine in other languages) or compute and return values.
	In the first case, we use a function call as a statement; in the second case, we use it as an expression.
]]
do
	local pi = math.pi
	local c = math.sin(pi) + math.cos(pi)		-- expression
	print(c)									-- -1.0 (statement)
end

--[[
	In both cases (expression and statement), a list of arguments enclosed in parentheses denotes the call; if the call has no arguments,
	we still must write an empty list () to denote it. There is a special case to this rule: if the function has
	one single argument and that argument is either a literal string or a table constructor, then the parentheses
	are optional.
]]
do
	print "hello world"								-- hello world
	print [[
		Welcome to learn lua.
		This is a simple language.
	]]												-- Welcome to learn lua.
													-- This is a simple language.

	local function f(t)
		for key, value in pairs(t) do
			print(key, value)
		end
	end
	f {x = 20, y = 30}								-- x	30
													-- y	20

	print(type{})									-- table
end

--[[
	A Lua program can use functions defined both in Lua and in C (or in any other language used by the host application).
	Typically, we resort to C functions both to achieve better performance and to access facilities not easily accessible
	directly from Lua, such as operating-system facilities. As an example, all functions from the standard Lua libraries
	are written in C. However, when calling a function, there is no difference between functions defined in Lua and
	functions defined in C.
]]
do
	local t = {"file", "selection", "edit", "view", "go", "run", "terminal", "help"}
	table.sort(t)									-- table.sort is a C function.

	for index, value in ipairs(t) do				-- ipair is a C function.
		print(index, value)							-- print is a C function.
	end

	-- output:
	-- 1       edit
	-- 2       file
	-- 3       go
	-- 4       help
	-- 5       run
	-- 6       selection
	-- 7       terminal
	-- 8       view
end

--[[
	We can call a function with a number of arguments different from its number of parameters.
	Lua adjusts the number of arguments to the number of parameters by throwing away extra arguments
	and supplying nils to extra parameters.
]]
do
	local function f(a, b)
		print(a, b)
	end

	f(1)												-- 1       nil
	f(1, 2)												-- 1       2
	f(1, 2, 3)											-- 1       2

	local count = 0
	local function add_cnt(cnt)
		cnt = cnt or 1									-- default value
		count = count + cnt
	end

	add_cnt(1)
	print(count)										-- 1
	add_cnt(3)
	print(count)										-- 4
end