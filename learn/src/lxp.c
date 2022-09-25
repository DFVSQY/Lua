#include <stdlib.h>
#include "expat.h"
#include "lua.h"
#include "lauxlib.h"

typedef struct lxp_userdata
{
	XML_Parser parser;
	lua_State *L;
} lxp_userdata;

static void f_start_element(void *ud, const char *name, const char **atts)
{
	lxp_userdata *xpu = (lxp_userdata *)ud;
	lua_State *L = xpu->L;

	lua_getfield(L, 3, "StartElement");
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		return;
	}

	lua_pushvalue(L, 1);	 /* push the parser ('self') */
	lua_pushstring(L, name); /* push tag name */

	/* create and fill the attribute table */
	lua_newtable(L);
	for (; *atts; atts += 2)
	{
		lua_pushstring(L, *(atts + 1)); /* push value */
		lua_setfield(L, -2, *atts);		/* table[*atts] = *(atts + 1) */
	}

	lua_call(L, 3, 0); /* call the handler */
}

static void f_char_data(void *ud, const char *s, int len)
{
	/* get lua state from userdata */
	lxp_userdata *xpu = (lxp_userdata *)ud;
	lua_State *L = xpu->L;

	/* get handler from callback table */
	lua_getfield(L, 3, "CharacterData");
	if (lua_isnil(L, -1)) /* no handler */
	{
		lua_pop(L, 1);
		return;
	}

	lua_pushvalue(L, 1);		/* push the parser ('self') */
	lua_pushlstring(L, s, len); /* push Char data */
	lua_call(L, 2, 0);			/* call the handler */
}

static void f_end_element(void *ud, const char *name)
{
	/* get lua state from userdata */
	lxp_userdata *xpu = (lxp_userdata *)ud;
	lua_State *L = xpu->L;

	/* get handler from callback table */
	lua_getfield(L, 3, "EndElement");
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		return;
	}

	lua_pushvalue(L, 1);	 /* push the parser ('self') */
	lua_pushstring(L, name); /* push tag name */
	lua_call(L, 2, 0);		 /* call the handler */
}

static int lxp_make_parser(lua_State *L)
{
	/* create a parser object */
	lxp_userdata *xpu = (lxp_userdata *)lua_newuserdata(L, sizeof(lxp_userdata));

	/* pre-initialize it, in case of error */
	xpu->parser = NULL;

	/* set metatable for the userdata */
	luaL_getmetatable(L, "LuaBook.Expat");
	lua_setmetatable(L, -2);

	/* create the Expat parser */
	XML_Parser p = xpu->parser = XML_ParserCreate(NULL);
	if (!p)
		luaL_error(L, "XML_ParserCreate failed");

	/* check and store the callback table*/
	luaL_checktype(L, 1, LUA_TTABLE); /* check the callback table */
	lua_pushvalue(L, 1);			  /* push table */
	lua_setuservalue(L, -2);		  /* set it as the user value of the metatable */

	/* configure expat parser */
	XML_SetUserData(p, xpu);								  /* set user data */
	XML_SetElementHandler(p, f_start_element, f_end_element); /* set start and end callback */
	XML_SetCharacterDataHandler(p, f_char_data);			  /* set char data callback */

	return 1;
}

/*
It gets two arguments: the parser object (the self of the method) and an optional piece of XML data.
When called without any data, it informs Expat that the document has no more parts.
*/
static int lxp_parse(lua_State *L)
{
	/* get and check first argument (should be a parser) */
	lxp_userdata *xpu = (lxp_userdata *)luaL_checkudata(L, 1, "LuaBook.Expat");

	/* check if it is not closed */
	luaL_argcheck(L, xpu->parser != NULL, 1, "parser is closed");

	size_t len;

	/* get second argument (a string) */
	const char *s = luaL_optlstring(L, 2, NULL, &len);

	/*
	put callback table at stack index 3.

	When lxp_parse calls XML_Parse,
	the latter function will call the handlers for each relevant element that it finds in the given piece of document.
	These handlers will need to access the callback table,
	so lxp_parse puts this table at stack index three (right after the parameters)
	*/
	lua_settop(L, 2);
	lua_getuservalue(L, 1);

	/* set lua state */
	xpu->L = L;

	/*
	call Expat to parse string.

	the last argument to this function tells Expat whether the given piece of text is the last one.
	When we call parse without an argument, s will be NULL, so this last argument will be true.
	*/
	int status = XML_Parse(xpu->parser, s, (int)len, s == NULL);

	/* return error code */
	lua_pushboolean(L, status);

	return 1;
}

static int lxp_close(lua_State *L)
{
	lxp_userdata *xpu = (lxp_userdata *)luaL_checkudata(L, 1, "LuaBook.Expat");

	/* free Expat parser (if there is one) */
	if (xpu->parser)
		XML_ParserFree(xpu->parser);

	xpu->parser = NULL; /* avoid closing it again */

	return 0;
}

static const struct luaL_Reg lxp_meths[] = {
	{"parse", lxp_parse},
	{"close", lxp_close},
	{"__gc", lxp_close},
	{NULL, NULL},
};

static const struct luaL_Reg lxp_funcs[] = {
	{"new", lxp_make_parser},
	{NULL, NULL},
};

int luaopen_lxp(lua_State *L)
{
	/* create metatable */
	luaL_newmetatable(L, "LuaBook.Expat");

	/* mt.__index == mt */
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	/* register methods for the metatable */
	luaL_setfuncs(L, lxp_meths, 0);

	/* create a table and register functions (only lxp.new) */
	luaL_newlib(L, lxp_funcs);

	return 1;
}