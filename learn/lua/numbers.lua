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
	with the result always having the same sign as the second argument.
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

--[[
	Lua provides the following relational operators:
	<   >   <=  >=  ==  ~=

	All these operators always produce a Boolean value.

	Comparison of numbers always disregards their subtypes;
	it makes no difference whether the number is represented as an integer or as a float.
	What matters is its mathematical value.
	(Nevertheless, it is slightly more efficient to compare numbers with the same subtypes.)
]]
print(3 ~= nil)								-- true
print(3 == 3.0)								-- true
print(3 >= 5.0)								-- false

--[[
	Lua provides a standard math library with a set of mathematical functions.
	All trigonometric functions work in radians.
	We can use the functions deg and rad to convert between degrees and radians.
]]
print(math.pi)								-- 3.1415926535898
print(math.huge)							-- inf
print(math.sin(math.pi / 6))				-- 0.5
print(math.deg(math.pi))					-- 180.0
print(math.rad(180))						-- 3.1415926535898

--[[
	Random-number generator
]]
math.randomseed(os.time())					-- default seed is 1
print(math.random())						-- [0,1)
print(math.random(6))						-- [1,6]
print(math.random(1, 10))					-- [1,10]

--[[
	Rounding functions

	The math library offers three rounding functions: floor, ceil, and modf.
	Floor rounds towards minus infinite, ceil rounds towards plus infinite, and modf rounds towards zero.
	They return an integer result if it fits in an integer;
	otherwise, they return a float (with an integral value, of course).
	The function modf, besides the rounded value, also returns the fractional part of the number as a second result.
]]
print(math.floor(3.3))						-- 3
print(math.floor(-3.3))						-- -4
print(math.ceil(3.3))						-- 4
print(math.ceil(-3.3))						-- -3
print(math.modf(3.3))						-- 3       0.3
print(math.modf(-3.3))						-- -3      -0.3
print(math.floor(2^70))						-- 1.1805916207174e+21
do
	local function round(x)
		local f = math.floor(x)
		if x == f then return f end
		return math.floor(x + 0.5)
	end

	print(math.floor(2^52 + 1))				-- 4503599627370497
	print(math.floor(2^52 + 1.5))			-- 4503599627370498	(It's wrong result, it should be 4503599627370497)
	print(round(2^52 + 1))					-- 4503599627370497
	print(round(3.5))						-- 4
	print(round(3.2))						-- 3
	print(round(3.7))						-- 4
	print(round(-3.5))						-- -3
	print(round(-3.2))						-- -3
	print(round(-3.7))						-- -4
end

--[[
	Representation Limits
]]
do
	print(math.maxinteger)					-- 9223372036854775807 (this is 2^63 - 1)
	print(0x7fffffffffffffff)				-- 9223372036854775807
	print(2^63 - 1)							-- 9.2233720368548e+18 (because it's a float number in Lua, it doesn't equal math.maxinteger)

	print(math.mininteger)					-- -9223372036854775808 (this is -(2^63))
	print(-(2^63))							-- -9.2233720368548e+18 (because it's a float number in Lua, it doesn't equal math.mininteger)
	print(0x8000000000000000)				-- -9223372036854775808
end

--[[
	When we compute an integer operation that would result in a value smaller than
	mininteger or larger than maxinteger, the result wraps around.
]]
do
	print(math.maxinteger + 1 == math.mininteger)				-- true
	print(math.mininteger - 1 == math.maxinteger)				-- true
	print(-math.mininteger == math.mininteger)					-- true
	print(math.mininteger // -1 == math.mininteger)				-- true
end

--[[
	float precision
]]
do
	print(12.7 - 20 + 7.3)										-- -8.8817841970013e-16

	print(math.maxinteger + 2)									-- -9223372036854775807 (wrong result)
	print(math.maxinteger + 2.0)								-- 9.2233720368548e+18 (wrong result)

	print(math.maxinteger + 1.0 ==  math.maxinteger + 2.0)		-- true (float addition)
end

--[[
	To force a number to be a float, we can simply add 0.0 to it. An integer always can be converted to a float.
]]
print(-3 + 0.0)													-- -3.0
print(math.maxinteger + 0.0)									-- 9.2233720368548e+18

--[[
	Any integer up to 253 (which is 9007199254740992) has an exact representation
	as a double-precision floating-point number.
	Integers with larger absolute values may lose precision when converted to a float.
]]
print(9007199254740991 + 0.0 == 9007199254740991)				-- true
print(9007199254740992 + 0.0 == 9007199254740992)				-- true
print(9007199254740993 + 0.0 == 9007199254740993)				-- false
print(9007199254740993 + 0.0 == 9007199254740992 + 0.0)			-- true

--[[
	To force a number to be an integer, we can OR it with zero.

	Lua does this kind of conversion only when the number has an exact representation as an integer,
	that is, it has no fractional part and it is inside the range of integers.
	Otherwise, Lua raises an error
]]
do
	print(2^53)														-- 9.007199254741e+15 (always be float)
	print(2^53 | 0)													-- 9007199254740992 (integer)

	local success, err_msg
	success, err_msg = pcall(function()
		print(3.2 | 0)												-- fractional part
	end)
	print(success, err_msg)											-- false   learn/lua/numbers.lua:273: number has no integer representation

	success, err_msg = pcall(function()
		print(2 ^ 64 | 0)											-- out of range
	end)
	print(success, err_msg)											-- false   learn/lua/numbers.lua:278: number has no integer representation

	success, err_msg = pcall(function()
		math.random(1, 3.5)											-- fractional part
	end)
	print(success, err_msg)											-- false   learn/lua/numbers.lua:283: bad argument #2 to 'random' (number has no integer representation)
end

--[[
	To round a fractional number, we must explicitly call a rounding function.
	Another way to force a number into an integer is to use math.tointeger,
	which returns nil when the number cannot be converted.
]]
do
	print(math.tointeger(-258.0))   								-- -258
    print(math.tointeger(2^30))   									-- 1073741824
    print(math.tointeger(5.01))   									-- nil (not an integral value)
    print(math.tointeger(2^64))   									-- nil (out of range)
end