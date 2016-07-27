## ===== instance function implementation template

int ${signature_name}(lua_State* L)
{
#if not $is_constructor
    ${namespaced_class_name}* cobj = (${namespaced_class_name}*)tolua_tousertype(L, 1, 0);
#end if
    int argc = lua_gettop(L) - 1;
#if len($arguments) > 0
    bool ok = true;

#end if
#if len($arguments) >= $min_args
    #set arg_count = len($arguments)
    #set arg_idx = $min_args
    #while $arg_idx <= $arg_count
    if (argc == $arg_idx)
    {
        #set $count = 0
        #while $count < $arg_idx
            #set $arg = $arguments[$count]
        ${arg.to_string($generator)} $argumentTips[$count];
            #set $count = $count + 1
        #end while
        #set $count = 0
        #set arg_list = ""
        #set arg_array = []
        #while $count < $arg_idx
            #set $arg = $arguments[$count]
            #set $gen_idx = $count+1
            #if not $is_constructor
                #set $gen_idx = $gen_idx + 1
            #end if
        ${arg.to_native({"generator": $generator,
                         "in_value": "argv[" + str(count) + "]",
                         "out_value": $argumentTips[$count],
                         "arg_idx": $gen_idx,
                         "class_name": $class_name,
                         "lua_namespaced_class_name": $generator.scriptname_from_native($namespaced_class_name, $namespace_name),
                         "func_name": $func_name,
                         "level": 2,
                         "arg":$arg,
                         "ntype": $arg.namespaced_name.replace("*", ""),
                         "scriptname": $generator.scriptname_from_native($arg.namespaced_name, $arg.namespace_name)})};
            #set $arg_to_push = $argumentTips[$count]
            #set $arg_array += [$arg_to_push]
            #set $count = $count + 1
        #end while
        #if $arg_idx > 0

        if (!ok)
        {
            tolua_error(L, "invalid arguments in function '${signature_name}'", nullptr);
            return 0;
        }
        #end if
    #set $arg_list = ", ".join($arg_array)
    #if $is_constructor

        ${namespaced_class_name}* cobj = new ${namespaced_class_name}($arg_list);
        tolua_pushusertype(L, (void*)cobj, "${generator.scriptname_from_native($namespaced_class_name, $namespace_name)}");
        tolua_register_gc(L, lua_gettop(L));
        return 1;
    #else
        #if $ret_type.name != "void"
            #if $ret_type.is_enum
        int ret = (int)cobj->${func_name}($arg_list);
            #else
        ${ret_type.get_whole_name($generator)} ret = cobj->${func_name}($arg_list);
            #end if
        ${ret_type.from_native({"generator": $generator,
                                "in_value": "ret",
                                "out_value": "ret",
                                "type_name": $ret_type.namespaced_name.replace("*", ""),
                                "ntype": $ret_type.get_whole_name($generator),
                                "class_name": $class_name,
                                "level": 2,
                                "scriptname": $generator.scriptname_from_native($ret_type.namespaced_name, $ret_type.namespace_name)})};
        return 1;
        #else
        cobj->${func_name}($arg_list);
        lua_settop(L, 1);
        return 1;
        #end if
    #end if
    }
        #set $arg_idx = $arg_idx + 1
    #end while
#end if
    return 0;
}
