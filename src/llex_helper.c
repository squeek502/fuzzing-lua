#include "llex_helper.h"

#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <llex.h>
#include <lzio.h>
#include <lstring.h>
#include <lparser.h>
#include <lcode.h>
#include <ldo.h>
#include <lfunc.h>
#include <ltable.h>

FILE* LLEX_HELPER_OUTPUT_FILE = NULL;

typedef struct LoadS {
	const char *s;
	size_t size;
} LoadS;

static const char *getS(lua_State *L, void *ud, size_t *size) {
	LoadS *ls = (LoadS *)ud;
	(void)L;
	if (ls->size == 0) return NULL;
	*size = ls->size;
	ls->size = 0;
	return ls->s;
}

// copied from lparser.c
static void open_func(LexState *ls, FuncState *fs) {
	lua_State *L = ls->L;
	Proto *f = luaF_newproto(L);
	fs->f = f;
	fs->prev = ls->fs;  /* linked list of funcstates */
	fs->ls = ls;
	fs->L = L;
	ls->fs = fs;
	fs->pc = 0;
	fs->lasttarget = -1;
	fs->jpc = NO_JUMP;
	fs->freereg = 0;
	fs->nk = 0;
	fs->np = 0;
	fs->nlocvars = 0;
	fs->nactvar = 0;
	fs->bl = NULL;
	f->source = ls->source;
	f->maxstacksize = 2;  /* registers 0/1 are always valid */
	fs->h = luaH_new(L, 0, 0);
	/* anchor table of constants and prototype (to avoid being collected) */
	sethvalue2s(L, L->top, fs->h);
	incr_top(L);
	setptvalue2s(L, L->top, f);
	incr_top(L);
}

static void protected_lex(lua_State *L, void *ud)
{
	(void)L; // fix warning
	LexState* lexstate = (LexState*)ud;
	while (1) {
		luaX_next(lexstate);
		if (LLEX_HELPER_OUTPUT_FILE) {
			fprintf(LLEX_HELPER_OUTPUT_FILE, "%s", luaX_token2str(lexstate, lexstate->t.token));
			if (lexstate->t.token != TK_EOS) { fprintf(LLEX_HELPER_OUTPUT_FILE, " "); }
		}
		if (lexstate->t.token == TK_EOS) {
			break;
		}
	}
}

void llex_fuzz(const uint8_t *data, size_t size)
{
	lua_State *L = luaL_newstate();

	LoadS ls;
	ls.s = (const char*)data;
	ls.size = size;
	lua_Reader reader = getS;
	ZIO z;
	luaZ_init(L, &z, reader, &ls);

	Mbuffer buff;
	luaZ_initbuffer(L, &buff);
	Mbuffer lexbuff;
	luaZ_initbuffer(L, &lexbuff);

	// valid funcstate is required during lexing
	FuncState funcstate;
	LexState lexstate;
	lexstate.L = L;
	lexstate.buff = &lexbuff;
	luaX_setinput(L, &lexstate, &z, luaS_new(L, "fuzz"));
	open_func(&lexstate, &funcstate);

	// protected call to avoid panic
	int status = luaD_pcall(L, protected_lex, &lexstate, savestack(L, L->top), L->errfunc);
	switch (status)
	{
	case LUA_ERRSYNTAX:
	case LUA_ERRRUN:
		if (LLEX_HELPER_OUTPUT_FILE) {
			fprintf(LLEX_HELPER_OUTPUT_FILE, "\n%s", lua_tostring(L, -1));
		}
		break;
	case LUA_ERRMEM:
	case LUA_ERRERR:
		printf("fatal error: %s\n", lua_tostring(L, -1));
		exit(1);
		break;
	case 0:
		break;
	}

	luaZ_freebuffer(L, &lexbuff);
	luaZ_freebuffer(L, &buff);

	lua_close(L);
	return;
}
