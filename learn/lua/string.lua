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
