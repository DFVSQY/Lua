local chunk1 = [[
	print(lproc.receive("chanel_1"))
]]

local chunk2 = [[
	lproc.send("chanel_1", "file", "edit", "selection")
]]

local lproc = require "lproc"
lproc.start(chunk1)
lproc.start(chunk2)

lproc.exit()