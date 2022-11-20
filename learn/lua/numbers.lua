--[[
	Starting with version 5.3, Lua uses two alternative representations for numbers: 64-bit integer numbers,
	called simply integers, and double-precision floating-point numbers, called simply floats.
	For restricted platforms, we can compile Lua 5.3 as Small Lua, which uses 32-bit integers and single-precision floats.
]]

-- 数的表示
print(4)       						-- 4
print(4.1)     						-- 4.1
print(4.57e-3) 						-- 0.00457
print(4.57E-3) 						-- 0.00457
print(0.3e12)  						-- 300000000000.0
print(0.3E12)  						-- 300000000000.0
print(5E20)    						-- 5e+20

--[[
	Numerals with a decimal point or an exponent are considered floats; otherwise, they are treated as integers.
	Both integer and float values have type "number"
]]
do
	local a = 3			-- integer
	local b = 3.		-- float
	local c = 3.0		-- float
	local d = 3E2		-- float
	print(a, b, c, d)	-- 3       3.0     3.0     300.0
end

--[[
	They have the same type because, more often than not, they are interchangeable.
	Moreover, integers and floats with the same value compare as equal in Lua.
]]
print(1 == 1.0)			-- true
print(-3 == -3.0)		-- true
print(0.2e3 == 200)		-- true

--[[
	In the rare occasions when we need to distinguish between floats and integers, we can use math.type
]]
print(math.type(3))		-- integer
print(math.type(3.0))	-- float
print(math.type(3e1))	-- float

--[[
	Like many other programming languages, Lua supports hexadecimal constants, by prefixing them with 0x.
	Unlike many other programming languages, Lua supports also floating-point hexadecimal constants,
	which can have a fractional part and a binary exponent, prefixed by p or P.
]]
print(0xff)				-- 255
print(0xFF)				-- 255
print(0x1a3)			-- 419
print(0x1A3)			-- 419
print(0x0.2)			-- 0.125
print(0x1p-1)			-- 0.5
print(0x1P-1)			-- 0.5
print(0xa.b)			-- 10.6875
print(0xa.bp2)			-- 42.75
print(0x4p-1)			-- 2.0

--[[
	Lua supports also floating-point hexadecimal constants,
	which can have a fractional part and a binary exponent, prefixed by p or P.
	Lua can write numbers in this format using string.format with the %a option
]]
print(string.format("%a", 255))				-- 0x1.fep+7
print(string.format("%a", 419))				-- 0x1.a3p+8
print(string.format("%a", 0.125))			-- 0x1p-3
print(string.format("%a", 0.5))				-- 0x1p-1
print(string.format("%a", 10.6875))			-- 0x1.56p+3
print(string.format("%a", 2))				-- 0x1p+1
print(string.format("%a", 0.1))				-- 0x1.999999999999ap-4

--[[
	The addition of two integers is always an integer. The same is true for subtraction, multiplication, and negation.
	If both operands are integers, the operation gives an integer result; otherwise, the operation results in a float.
	In case of mixed operands, Lua converts the integer one to a float before the operation.
	Division does not follow that rule, because the division of two integers does not need to be an integer.
	To avoid different results between division of integers and divisions of floats,
	division always operates on floats and gives float results
]]
print(13 + 15)								-- 28
print(13.0 + 15.0)							-- 28.0
print(13.0 + 25)							-- 38.0
print(-(3 * 6.0))							-- -18.0
print(3.0 / 2.0)							-- 1.5
print(3.0 / 2)								-- 1.5
print(3 / 2)								-- 1.5
print(math.type(4 / 2))						-- float

--[[
	For  integer  division,  Lua  5.3  introduced  a  new  operator,  called  floor  division  and  denoted  by  //.
	As its name implies, floor division always rounds the quotient towards minus infinity, ensuring an integral result
	for all operands. With this definition, this operation can follow the same rule of the other arithmetic operators:
	if both operands are integers, the result is an integer; otherwise, the result is a float (with an integral value)
]]
print(4 // 2)								-- 2
print(4.0 // 2)								-- 2.0
print(1.5 // 0.5)							-- 3.0
print(-9 // 2)								-- -5
print(9 // -2)								-- -5
print(-9 // -2)								-- 4

--[[
	The modulo operator:
	a % b == a - ((a // b) * b)

	If both operands are integers, the result is an integer; otherwise, the result is a float.

	For integer operands, modulo has the usual meaning,
	with the result always having the same sign as the second  argument.


]]
print(4 % 2)								-- 0
print(4.0 % 2)								-- 0.0
print(1.5 % 0.5)							-- 0.0
print(-9 % 2)								-- 1
print(9 % -2)								-- -1
print(-9 % -2)								-- -1

--[[
	For real operands, modulo has some unexpected uses.
	For instance, x - x % 0.01 is x with exactly two decimal digits,
	and x - x % 0.001 is x with exactly three decimal digits.
]]
do
	local x = math.pi
	print(x - x % 0.01)        				-- 3.14
	print(x - x % 0.001)       				-- 3.141
	print(x % 0.01)							-- 0.0015926535897931
	print(-x % -0.01)						-- -0.0015926535897931
	print(-x % 0.01)						-- 0.0084073464102069
	print(x % -0.01)						-- -0.0084073464102069
end

--[[
	Lua also offers an exponentiation operator, denoted by a caret (^).
	Like division, it always operates on floats.
]]
print(4 ^ 0.5)								-- 2.0
print(8 ^ (1 / 3))							-- 2.0