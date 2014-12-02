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

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "ext/cmark.h"

#include <string.h>

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
	luaL_register(L, "cmark", methods);
#else
	luaL_newlib(L, methods);
#endif

	return 1;
}
