--[[
	用于测试资源管理的示例
]]

if _G.use_c_dir_lib then
	local dir = require "dir"
	for file_name in dir.open(".") do
		print(file_name)
	end
end

if _G.use_c_xml_parser then
	local count = 0

	local callbacks = {
		StartElement = function(parser, tagname, attr)
			local attr_str = ""
			if attr then
				attr_str = attr_str .. "("
				for key, value in pairs(attr) do
					if attr_str ~= "(" then attr_str = attr_str .. " " end
					attr_str = attr_str .. key .. "=" .. value
				end
				attr_str = attr_str .. ")"
			end
			io.write("+ ", string.rep("    ", count), tagname, attr_str, "\n")
			count = count + 1
		end,
		EndElement = function(parser, tagname)
			count = count - 1
			io.write("- ", string.rep("    ", count), tagname, "\n")
		end,
		CharacterData = function(parser, charData)
			io.write("  ", string.rep("    ", count - 1), charData, "\n")
		end
	}

	local xml_parser = require "lxp"
	local p = xml_parser.new(callbacks)		-- create new xml parser

	local lines = {'<tag cap="5">hi</tag>'}
	for _, line in ipairs(lines) do
		assert(p:parse(line))					-- parse the line
		assert(p:parse("\n"))					-- add newline
	end

	assert(p:parse())						-- finish document
	p:close()								-- close parser

	-- output result:
	-- + tag(cap=5)
	--   hi
	-- - tag
end