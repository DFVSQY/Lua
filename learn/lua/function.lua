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
	In both cases (expression and statement), a list of arguments enclosed in parentheses denotes the call;
	if the call has no arguments, we still must write an empty list () to denote it. There is a special case to this rule:
	if the function has one single argument and that argument is either a literal string or a table constructor,
	then the parentheses are optional.
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

--[[
	Functions in Lua can return multiple results, by listing them all after the return keyword.
	We get all results only when the call is the last (or the only) expression in a list of expressions.
	These lists appear in four constructions in Lua:
	multiple assignments, arguments to function calls, table constructors, and return statements.
]]
do
	local function func_0()
	end
	local function func_1()
		return "1_1"
	end
	local function func_2()
		return "2_1", "2_2"
	end

	--[[
		In a multiple assignment, a function call as the last (or only) expression produces
		as many results as needed to match the variables.
	]]
	do
		local x1, y1 = func_2()
		print(x1, y1)						-- 2_1     2_2

		local x2 = func_2()
		print(x2)							-- 2_1

		local x3, y3, z3 = 10, func_2()
		print(x3, y3, z3)					-- 10      2_1     2_2
	end

	--[[
		In a multiple assignment, if a function has fewer results than we need,
		Lua produces nils for the missing values.
	]]
	do
		local x1, y1 = func_0()
		print(x1, y1)						-- nil     nil

		local x2, y2 = func_1()
		print(x2, y2)						-- 1_1     nil

		local x3, y3, z3 = func_2()
		print(x3, y3, z3)					-- 2_1     2_2     nil
	end

	--[[
		Multiple results only happen when the call is the last (or only) expression in a list.
		A function call that is not the last element in the list always produces exactly one result.
	]]
	do
		local x1, y1 = func_2(), 10
		print(x1, y1)						-- 2_1     10

		local x2, y2 = func_0(), 20, 30
		print(x2, y2)						-- nil     20
	end

	--[[
		When a function call is the last (or the only) argument to another call,
		all results from the first call go as arguments.
	]]
	do
		print(func_0())						-- (nothing  output)
		print(func_1())						-- 1_1
		print(func_2())						-- 2_1		2_2
		print(func_2(), 10)					-- 2_1		10
	end

	--[[
		When the call to foo2 appears inside an expression, Lua adjusts the number of results to one;
	]]
	do
		print(func_2() .. "Edit")			-- 2_1Edit
	end

	--[[
		A constructor also collects all results from a call, without any adjustments.
		As always, this behavior happens only when the call is the last expression in the list;
		calls in any other position produce exactly one result.
	]]
	do
		local t1 = {func_0()}
		print(t1[1])								-- nil

		local t2 = {func_1()}
		print(t2[1], t2[2])							-- 1_1     nil

		local t3 = {func_2()}
		print(t3[1], t3[2], t3[3])					-- 2_1     2_2     nil

		local t4 = {func_0(), func_2(), 10}
		print(t4[1], t4[2], t4[3], t4[4])			-- nil     2_1     10      nil
	end

	--[[
		A statement like return f() returns all values returned by f.
	]]
	do
		local function test(i)
			if i == 0 then return func_0() end
			if i == 1 then return func_1() end
			if i == 2 then return func_2() end
		end

		print(test(0))								-- (nothing output)
		print(test(1))								-- 1_1
		print(test(2))								-- 2_1		2_2
	end

	--[[
		We can force a call to return exactly one result by enclosing it in an extra pair of parentheses.
	]]
	do
		print((func_0()))							-- nil
		print((func_1()))							-- 1_1
		print((func_2()))							-- 2_1

		local function test(i)
			if i == 0 then return (func_0()) end
			if i == 1 then return (func_1()) end
			if i == 2 then return (func_2()) end
		end
		print(test(0))								-- nil
		print(test(1))								-- 1_1
		print(test(2))								-- 2_1
	end
end