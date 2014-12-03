/*
 * Copyright (c) 2014 James Turner <james@calminferno.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define LUA_COMPAT_MODULE "1"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "ext/cmark.h"

#include <string.h>


#define NODE "Node"
typedef struct cmark_node * Node;

/* note, luaL_typeerror was removed in lua 5.2, so we just copy the code */
static int S_typeerror(lua_State *L, int index, const char* tname)
{
	const char *msg = lua_pushfstring(L, "%s expected, got %s",
					  tname, luaL_typename(L, index));
	return luaL_argerror(L, index, msg);
}

static Node toNode(lua_State *L, int index)
{
	Node *node = (Node *)lua_touserdata(L, index);
	if (node == NULL) S_typeerror(L, index, NODE);
	return *node;
}

static Node checkNode (lua_State *L, int index)
{
	Node *node, n;
	luaL_checktype(L, index, LUA_TUSERDATA);
	node = (Node*)luaL_checkudata(L, index, NODE);
	if (node == NULL) S_typeerror(L, index, NODE);
	n = *node;
	if (!n) luaL_error(L, "null Node");
	return n;
}

static Node *pushNode (lua_State *L, Node n)
{
	Node *node = (Node *)lua_newuserdata(L, sizeof(Node));
	*node = n;
	luaL_getmetatable(L, NODE);
	lua_setmetatable(L, -2);
	return node;
}


static int Node_new (lua_State *L)
{
	pushNode(L, cmark_node_new(0));
	return 1;
}

static const luaL_Reg Node_methods[] = {
	{"new",Node_new},
	{0,0}
};

static int Node_gc(lua_State *L)
{
	Node n = toNode(L, 1);
	if (n) cmark_node_free(n);
	// TODO for debugging:
	printf("goodby Node (%p)\n", lua_touserdata(L, 1));
	return 0;
}

static int Node_tostring(lua_State *L)
{
	lua_pushfstring(L, "Node: %p", lua_touserdata(L,1));
	return 1;
}

static const luaL_Reg Node_meta[] = {
	{"__gc", Node_gc},
	{"__tostring", Node_tostring},
	{0,0}
};

int Node_register(lua_State *L)
{
	luaL_register(L, NODE, Node_methods);
	luaL_newmetatable(L, NODE);
	luaL_register(L, 0, Node_meta);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	return 1;
}

static int l_markdown(lua_State *L)
{
	char *res;

	const char *mkd = luaL_checkstring(L, 1);
	size_t len = strlen(mkd);

	res = cmark_markdown_to_html(mkd, len);
	len = strlen(res);

	lua_pushlstring(L, res, len);

	return 1;
}

int luaopen_cmark(lua_State *L)
{
	static const struct luaL_Reg methods[] = {
		{ "markdown",	l_markdown },
		{ NULL,		NULL }
	};

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501
	Node_register(L);
	luaL_register(L, "cmark", methods);
#else
	Node_register(L);
	luaL_newlib(L, methods);
#endif

	return 1;
}
