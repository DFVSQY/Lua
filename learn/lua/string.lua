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