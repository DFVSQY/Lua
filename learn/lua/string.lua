--[[
	Strings in Lua are sequences of bytes. The Lua core is agnostic about how these bytes encode text.
	Lua is eight-bit clean and its strings can contain bytes with any numeric code, including embedded zeros.
	This means that we can store any binary data into a string. We can also store Unicode strings in any
	representation (UTF-8, UTF-16, etc.); however, there are several good reasons to use UTF-8 whenever possible.
	The standard string library that comes with Lua assumes one-byte characters, but it can handle UTF-8
	strings quite reasonably. Moreover, since version 5.3, Lua comes with a small library to help the use of
	UTF-8 encoding.

	Strings in Lua are subject to automatic memory management, like all other Lua objects (tables, functions, etc.).
	This means that we do not have to worry about allocation and deallocation of strings; Lua handles it for us
]]

--[[
	Strings in Lua are immutable values. We cannot change a character inside a string, as we can in C;
	instead, we create a new string with the desired modifications,
]]
do
	local a = "one string"
	local b = string.gsub(a, "one", "another")
	print(a)										-- one string
	print(b)										-- another string
end

--[[
	We can get the length of a string using the length operator (denoted by #).

	This operator always counts the length in bytes, which is not the same as characters in some encodings.
]]
do
	local a = "vscode"
	print(#a)										-- 6

	local b = "学习Lua"
	print(#b)										-- 9
end

--[[
	We can concatenate two strings with the concatenation operator .. (two dots).
	If any operand is a number, Lua converts this number to a string.
]]
do
	print("hello " .. "lua")						-- hello lua
	print("lua5" .. 3)								-- lua53
end

--[[
	We can delimit literal strings by single or double matching quotes.
]]
do
	local a = "Hello"
	local b = 'World'
	print(a, b)										-- Hello   World
end

--[[
	We can specify a character in a literal string also by its numeric value through the escape sequences \ddd and \xhh,
	where ddd is a sequence of up to three decimal digits and hh is a sequence of exactly two hexadecimal digits.
]]
do
	local a = 'ALO123'
	local b = '\x41LO\04923'						-- 0x41 (65 in decimal) is the ASCII code for A, and 49 is the code for the digit 1
	print(a)										-- ALO123
	print(b)										-- ALO123
	print('\x41\x4c\x4f\x31\x32\x33')				-- ALO123

	local c = '\x41BC\97bc'
	local d = '\x41BC\097bc'
	print(c)										-- ABCabc
	print(d)										-- ABCabc
end

--[[
	Since Lua 5.3, we can also specify UTF-8 characters with the escape sequence \u{h... h};
	we can write any number of hexadecimal digits inside the brackets.
]]
do
	print("\u{4E2D}\u{56FD}")						-- 中国
end

--[[
	We can delimit literal strings also by matching double square brackets, as we do with long comments.
	Literals in this bracketed form can run for several lines and do not interpret escape sequences.
	Moreover, it ignores the first character of the string when this character is a newline.
	This form is especially convenient for writing strings that contain large pieces of code.
]]
do
	local ss = [[
		<html>
		<head>
			<title>An HTML Page</title>
		</head>
		<body>
			<a href="http://www.lua.org">Lua</a>
		</body>
		</html>
	]]
	print(ss)										-- <html>
													-- <head>
													-- 		<title>An HTML Page</title>
													-- </head>
													-- <body>
													-- 		<a href="http://www.lua.org">Lua</a>
													-- </body>
													-- </html>
end

--[=[
	Sometimes, we may need to enclose a piece of code containing something like a = b[c[i]] (notice the ]] in this code),
	or we may need to enclose some code that already has some code commented out.
	To handle such cases, we can add any number of equals signs between the two opening brackets, as in [===[.
	After this change, the literal string ends only at the next closing brackets with the same number of equals signs
	in between (]===], in our example).  The scanner ignores any pairs of brackets with a different number of equals signs.
	By choosing an appropriate number of signs, we can enclose any literal string without having to modify it in any way.

	This same facility is valid for comments, too.
]=]
do
	local ss = [=[
		local b, c = [1], ['file']
		local a = b[c[i]]
		print(a)
	]=]
	print(ss)										-- local b, c = [1], ['file']
													-- local a = b[c[i]]
													-- print(a)
end

--[[
	Long strings are the ideal format to include literal text in our code,
	but we should not use them for non-text literal
]]
do
	local ss = [[
		\u{4E2D}\u{56FD}
		\x41BC\097bc
		\x41LO\04923
	]]
	print(ss)										-- \u{4E2D}\u{56FD}
													-- \x41BC\097bc
													-- \x41LO\04923
end

--[[
	Moreover, end-of-line sequences like "\r\n" may be normalized to "\n" when read.
	Instead, it is better to code arbitrary binary data using numeric escape sequences either in decimal or in hexadecimal.

	 However, this poses a problem for long strings, because they would result in quite long lines.
	 For those situations, since version 5.2 Lua offers the escape sequence \z:
	 it skips all subsequent space characters in the string until the first non-space character.
]]
do
	local ss = "Hello Lua Code Editor: \z
				Visual Studio Code"
	print(ss)										-- Hello Lua Code Editor: Visual Studio Code
end

--[[
	Lua provides automatic conversions between numbers and strings at run time.
	Any numeric operation applied to a string tries to convert the string to a number.
	Conversely, whenever Lua finds a number where it expects a string, it converts the number to a string.

	The rule for arithmetic operations is that the result is an integer only when both operands are integers;
	a string is not an integer, so any arithmetic operation with strings is handled as a floating-point operation.
]]
do
	print("10" + 2)									-- 12.0
	print(tonumber("10") + 2)						-- 12
	print("10" + "20")								-- 30.0
	print(math.sin(tostring(math.pi) / 6))			-- 0.5

	print(10 .. 24 .. "code")						-- 1024code
	print(string.len(1024))							-- 4
end

--[[
	When we write the concatenation operator right after a numeral, we must separate them with a space;
	otherwise, Lua thinks that the first dot is a decimal point.
]]
do
	print(10 .. 20)									-- 1020
	print(10 ..20)									-- 1020
	-- print(10.. 20)								-- this is wrong, malformed number near '10..'
	-- print(10... 20)								-- this is wrong, malformed number near '10...'
end

--[[
	To convert a string to a number explicitly, we can use the function tonumber,
	which returns nil if the string does not denote a proper number.
	Otherwise, it returns integers or floats, following the same rules of the Lua scanner.
]]
do
	print(tonumber("  -3 "))        				-- -3
	print(tonumber(" 10e4 "))       				-- 100000.0
	print(tonumber("10e"))          				-- nil   (not a valid number)
	print(tonumber("0x1.3p-4"))     				-- 0.07421875
end

--[[
	By default, tonumber assumes decimal notation, but we can specify any base between 2 and 36 for the conversion.
]]
do
	print(tonumber("100101", 2))       				-- 37
	print(tonumber("fff", 16))         				-- 4095
	print(tonumber("-ZZ", 36))         				-- -1295

	-- the string does not represent a proper numeral in the given base, so tonumber returns nil
	print(tonumber("987", 8))          				-- nil
end

--[[
	To convert a number to a string, we can call the function tostring
]]
do
	print(tostring(10) == "10")						-- true
end

--[[
	Unlike arithmetic operators, order operators never coerce their arguments.
	Moreover, 2 < 15 is obviously true, but "2" < "15" is false (alphabetical order).
	To avoid inconsistent results, Lua raises an error when we mix strings and numbers in an order comparison,
	such as 2 < "15".
]]
do
	print(2 < 15)									-- true
	print("2" < "15")								-- false

	local suc, msg = pcall(function()
		print(2 < "15")
	end)
	print(suc, msg)									-- false	learn/lua/string.lua:229: attempt to compare number with string
end

--[[
	The power of a raw Lua interpreter to manipulate strings is quite limited.
	A program can create string literals, concatenate them, compare them, and get string lengths.
	However, it cannot extract substrings or examine their contents.
	The full power to manipulate strings in Lua comes from its string library.

	The call string.len(s) returns the length of a string s; it is equivalent to #s.
]]
do
	local a = "vscode"
	print(#a, string.len(a))										-- 6	6

	local b = "学习Lua"
	print(#b, string.len(b))										-- 9	9

	local c = string.rep(b, 5, ":")
	print(c, #c, string.len(c))										-- 学习Lua:学习Lua:学习Lua:学习Lua:学习Lua	49	49

	local d = string.reverse("Hello World")
	print(d)														-- dlroW olleH

	local e = string.lower("Hello World")
	print(e)														-- hello world

	local f = string.upper("Hello World");
	print(f)														-- HELLO WORLD

	local title = "selection"
	print(string.sub(title, 1, -1))									-- selection
	print(string.sub(title, 1, 4))									-- sele
	print(string.sub(title, 5, -1))									-- ction
	print(string.sub(title, -1, -1))								-- n
end

--[[
	The functions string.char and string.byte convert between characters and their internal numeric representations.
	The function string.char gets zero or more integers, converts each one to a character,
	and returns a string concatenating all these characters.
	The call string.byte(s, i) returns the internal numeric representation of the i-th character of the string s;
	the second argument is optional; the call string.byte(s) returns the internal numeric representation of the first
	(or single) character of s
]]
do
	local a = string.char(97, 98, 99)
	print(a)														-- abc

	local b = "abc"
	print(string.byte(b, 1, -1))									-- 97      98      99
end

--[[
	A nice idiom is {string.byte(s, 1, -1)}, which creates a list with the codes of all characters in s.
	(This idiom only works for strings somewhat shorter than 1 MB. Lua limits its stack size,
	which in turn limits the maximum number of returns from a function. The default stack limit is one million entries.)
]]
do
	local codes = {string.byte("abc", 1, -1)}
	print(codes[1], codes[2], codes[3])								-- 97      98      99
end

--[[
	The function string.format is a powerful tool for formatting strings and converting numbers to strings.
	It returns a copy of its first argument, the so-called format string,
	with each directive in that string replaced by a formatted version of its correspondent argument.
	The directives in the format string have rules similar to those of the C function printf.
	A directive is a percent sign plus a letter that tells how to format the argument:
	d for a decimal integer, x for hexadecimal, f for a floating-point number, s for strings, plus several others
]]
do
	print(string.format("x = %d  y = %d", 10, 20))   							-- x = 10  y = 20
    print(string.format("x = %x", 200))              							-- x = c8
    print(string.format("x = 0x%X", 200))            							-- x = 0xC8
    print(string.format("x = 0x%x", 200))            							-- x = 0xc8
    print(string.format("x = %f", 200))              							-- x = 200.000000

    local tag, title = "h1", "a title"
    print(string.format("<%s>%s</%s>", tag, title, tag))						-- <h1>a title</h1>
end

--[[
	Between the percent sign and the letter, a directive can include other options that control the details of
	the formatting, such as the number of decimal digits of a floating-point number.
]]
do
	-- the %.4f means a floating-point number with four digits after the decimal point.
	print(string.format("pi = %.4f", math.pi))      							-- pi = 3.1416

	-- the %02d means a decimal number with zero padding and at least two digits
	-- the %2d, without the zero, would use blanks for padding.
    local d, m, y = 5, 11, 1990
    print(string.format("%02d/%02d/%04d", d, m, y)) 							-- 05/11/1990
    print(string.format("%2d/%2d/%4d", d, m, y)) 								--  5/11/1990
end

--[[
	We can call all functions from the string library as methods on strings, using the colon operator.
]]
do
	local a = "VSCode"
	print(a:sub(3, -1))															-- Code

	local b = "Hello World"
	print(b:lower())															-- hello world
end

--[[
	The string library includes also several functions based on pattern matching.
	The function string.find searches for a pattern in a given string.
	It returns the initial and final positions of the pattern in the string, or nil if it cannot find the pattern.
]]
do
	print(string.find("hello world", "wor"))   									-- 7   9
    print(string.find("hello world", "war"))   									-- nil
end

--[[
	The function string.gsub (Global SUBstitution) replaces all occurrences of a pattern in a string with another string.
	It also returns, as a second result, the number of replacements it made.
]]
do
	print(string.gsub("hello world", "l", "."))     							-- he..o wor.d    3
	print(string.gsub("hello world", "ll", ".."))   							-- he..o world    1
	print(string.gsub("hello world", "a", "."))     							-- hello world    0
end