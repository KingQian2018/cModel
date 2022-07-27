set(lua_src
    lcorolib.c
    ldump.c
    llex.c
    lobject.h
    lstate.c
    ltm.c
    lualib.h
    lapi.c
    lctype.c
    lfunc.c
    llex.h
    lopcodes.c
    lstate.h
    ltm.h
    lundump.c
    lapi.h
    lctype.h
    lfunc.h
    llimits.h
    lopcodes.h
    lstring.c
    lundump.h
    lauxlib.c
    ldblib.c
    lgc.c
    lmathlib.c
    lopnames.h
    lstring.h
    lutf8lib.c
    lauxlib.h
    ldebug.c
    lgc.h
    lmem.c
    loslib.c
    lstrlib.c
    lbaselib.c
    ldebug.h
    linit.c
    lmem.h
    lparser.c
    ltable.c
    lvm.h
    lvm.c
    lcode.c
    ldo.c
    liolib.c
    loadlib.c
    lparser.h
    ltable.h
    lzio.c
    lcode.h
    ldo.h
    ljumptab.h
    lobject.c
    lprefix.h
    ltablib.c
    luaconf.h
    lzio.h
)

add_compile_options(-std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3)

add_library(lib_lua STATIC ${lua_src})
add_executable(lua lua.h lua.c)
add_executable(luac luac.c)
target_link_libraries(lua lib_lua)
target_link_libraries(luac lib_lua)

