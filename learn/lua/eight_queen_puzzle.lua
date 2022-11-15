--[[
	A first step to solving the eight-queen puzzle is to note that any valid solution must have exactly one queen
	in each row. Therefore, we can represent potential solutions with a simple array of eight numbers, one for
	each row; each number tells at which column is the queen at that row. For instance, the array {3, 7, 2,
	1, 8, 6, 5, 4} means that the queens are in the squares (1,3), (2,7), (3,2), (4,1), (5,8), (6,6), (7,5),
	and (8,4). (By the way, this is not a valid solution; for instance, the queen in square (3,2) can attack the
	one in square (4,1).) Note that any valid solution must be a permutation of the integers 1 to 8, as a valid
	solution also must have exactly one queen in each column.
]]


local N = 8    -- board size

-- check whether position (n,c) is free from attacks
local function isplaceok (a, n, c)
	for i = 1, n - 1 do   					-- for each queen already placed
		if (a[i] == c) or                	-- same column?
			(a[i] - i == c - n) or        	-- same diagonal?
			(a[i] + i == c + n) then      	-- same diagonal?
			return false            		-- place can be attacked
		end
	end
	return true    							-- no attacks; place is OK
end

-- print a board
local function printsolution (a)
	for i = 1, N do      									-- for each row
		for j = 1, N do    									-- and for each column
			io.write(a[i] == j and "X" or "-", " ") 		-- write "X" or "-" plus a space
		end
		io.write("\n")
	end
	io.write("\n")
end

-- add to board 'a' all queens from 'n' to 'N'
local function addqueen (a, n)
	if n > N then    										-- all queens have been placed?
		printsolution(a)
		return
	end

	-- try to place n-th queen
	for c = 1, N do											-- foreach column
		if isplaceok(a, n, c) then
			a[n] = c    									-- place n-th queen at column 'c'
			addqueen(a, n + 1)
		end
	end
end

-- run the program
addqueen({}, 1)