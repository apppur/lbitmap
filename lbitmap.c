#include <lua.h>
#include <lauxlib.h>

#include <limits.h>

#define BITS_PER_WORD (CHAR_BIT * sizeof (unsigned int))
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD))
#define checkbitmap(L) (bitmap *)luaL_checkudata(L, 1, "bitmap")

typedef struct bitmap {
    int sz;
    unsigned int bits[1];
} bitmap;

static int lnew(lua_State * L) {
    int i;
    size_t sz;
    bitmap * ptr;

    int n = luaL_checkinteger(L, 1);
    luaL_argcheck(L, n >= 1, 1, "invalid size");
    sz = sizeof(bitmap) + I_WORD(n-1) * sizeof(unsigned int);
    ptr = (bitmap *)lua_newuserdata(L, sz);
    ptr->sz = n;
    for (i = 0; i <= I_WORD(n-1); i++) {
        ptr->bits[i] = 0;
    }

    luaL_getmetatable(L, "bitmap");
    lua_setmetatable(L, -2);

    return 1;
}

/*
static unsigned int * getindex(lua_State * L, unsigned int *mask) {
    bitmap * ptr = checkbitmap(L);
    int index = luaL_checkinteger(L, 2) - 1;
    luaL_argcheck(L, 0 <= index && index < ptr->sz, 2, "index out of range");

    *mask = I_BIT(index);
    return &ptr->bits[I_WORD(index)];
}
*/

static int lset(lua_State * L) {
    //bitmap * ptr = (bitmap *)lua_touserdata(L, 1);
    bitmap * ptr = checkbitmap(L);
    int index = luaL_checkinteger(L, 2) - 1;

    luaL_argcheck(L, ptr != NULL, 1, "bit map expected");
    luaL_argcheck(L, 0 <= index && index < ptr->sz, 2, "index out of range");
    luaL_checkany(L, 3);

    if (lua_toboolean(L, 3))
        ptr->bits[I_WORD(index)] |= I_BIT(index);
    else
        ptr->bits[I_WORD(index)] &= ~I_BIT(index);

    return 0;
}

static int lget(lua_State * L) {
    //bitmap * ptr = (bitmap *) lua_touserdata(L, 1);
    bitmap * ptr = checkbitmap(L);
    int index = luaL_checkinteger(L, 2) - 1;

    luaL_argcheck(L, ptr != NULL, 1, "bit map expected");
    luaL_argcheck(L, 0 <= index && index < ptr->sz, 2, "index out of range");

    lua_pushboolean(L, ptr->bits[I_WORD(index)] & I_BIT(index));

    return 1;
}

static int lsize(lua_State * L) {
    //bitmap * ptr = (bitmap *)lua_touserdata(L, 1);
    bitmap * ptr = checkbitmap(L);
    luaL_argcheck(L, ptr != NULL, 1, "bit map expected");
    lua_pushinteger(L, ptr->sz);

    return 1;
}

static int ltostring(lua_State *L) {
    bitmap * ptr = checkbitmap(L);
    lua_pushfstring(L, "bitmap(%d)", ptr->sz);

    return 1;
}


static const struct luaL_Reg l0[] = {
    {"new", lnew},
    {NULL, NULL}
};

static const struct luaL_Reg l1[] = {
    {"__newindex", lset},
    {"__index", lget},
    {"__len", lsize},
    {"__tostring", ltostring},
    //{"set", lset},
    //{"get", lget},
    //{"size", lsize},
    {NULL, NULL}
};

int luaopen_lbitmap(lua_State * L) {
    luaL_newmetatable(L, "bitmap");
    luaL_setfuncs(L, l1, 0);

    luaL_newlib(L, l0);

    return 1;
}
