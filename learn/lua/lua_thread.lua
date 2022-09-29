local function foo(x)
	coroutine.yield(x + 1, x)
end

function _G.foo1(x)
	foo(x+1)
	return 2 * x
end