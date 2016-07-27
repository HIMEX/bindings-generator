## ===== instance function implementation template - for overloaded functions

int ${signature_name}(lua_State* L)
{
#if not $is_constructor
    ${namespaced_class_name}* cobj = (${namespaced_class_name}*)tolua_tousertype(L, 1, 0);
#end if
    int argc = lua_gettop(L) - 1;
    bool ok = true;

#for func in $implementations
    if (argc == $func.min_args)
    {
#if len($func.arguments) >= $func.min_args
    #set arg_count = len($func.arguments)
    #set arg_idx = $func.min_args
    #while $arg_idx <= $arg_count
        #set arg_list = ""
        #set arg_array = []
        #if $func.min_args >= 0
        #set $count = 0
        #while $count < $arg_idx
        #set $arg = $func.arguments[$count]
        ${arg.to_string($generator)} $func.argumentTips[$count];
        ${arg.to_native({"generator": $generator,
                         "in_value": "argv[" + str(count) + "]",
                         "out_value": $func.argumentTips[$count],
                         "arg_idx": $count+2,
                         "class_name": $class_name,
                         "lua_namespaced_class_name": $generator.scriptname_from_native($namespaced_class_name, $namespace_name),
                         "func_name": $func_name,
                         "level": 2,
                         "arg":$arg,
                         "ntype": $arg.namespaced_name.replace("*", ""),
                         "scriptname": $generator.scriptname_from_native($arg.namespaced_name, $arg.namespace_name)})};
        #set $arg_to_push = $func.argumentTips[$count]
        #set $arg_array += [$arg_to_push]
        #set $count = $count + 1
    #end while

        if (ok)
        {
    #set $arg_list = ", ".join($arg_array)
#end if
#if $is_constructor
            cobj = new ${namespaced_class_name}($arg_list);
            tolua_pushusertype(L, (void*)cobj, "${generator.scriptname_from_native($namespaced_class_name, $namespace_name)}");
            tolua_register_gc(L, lua_gettop(L));
            return 1;
#else
    #if $func.ret_type.name != "void"
        #if $func.ret_type.is_enum
            int ret = (int)cobj->${func.func_name}($arg_list);
        #else
            ${func.ret_type.get_whole_name($generator)} ret = cobj->${func.func_name}($arg_list);
        #end if
            ${func.ret_type.from_native({"generator": $generator,
                                         "in_value": "ret",
                                         "out_value": "ret",
                                         "type_name": $func.ret_type.namespaced_name.replace("*", ""),
                                         "ntype": $func.ret_type.get_whole_name($generator),
                                         "class_name": $class_name,
                                         "level": 2,
                                         "scriptname": $generator.scriptname_from_native($func.ret_type.namespaced_name, $func.ret_type.namespace_name)})};
            return 1;
    #else
            cobj->${func.func_name}($arg_list);
            lua_settop(L, 1);
            return 1;
        #end if
    #end if
        }
    #set $arg_idx = $arg_idx + 1
    #end while
#end if
    }

#end for
    tolua_error(L, "invalid arguments in function '${signature_name}'", nullptr);
    return 0;
}
