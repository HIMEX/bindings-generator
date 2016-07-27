
#for luaclass in $sorted_classes
#if $in_listed_classes(luaclass)
\#include "${prefix}_${luaclass}.h"
#end if
#end for

TOLUA_API int register_all_${prefix}(lua_State* L)
{
    tolua_open(L);

    #if $target_ns
    tolua_module(L, "${target_ns}", 0);
    tolua_beginmodule(L, "${target_ns}");
    #else
    tolua_module(L, nullptr, 0);
    tolua_beginmodule(L, nullptr);
    #end if

    #for luaclass in $sorted_classes
    #if $in_listed_classes(luaclass)
    register_${prefix}_${luaclass}(L);
    #end if
    #end for

    tolua_endmodule(L);
    return 1;
}
#if $macro_judgement
\#endif
#end if