#set has_constructor = False
#if $current_class.methods.has_key('constructor')
#set has_constructor = True
${current_class.methods.constructor.generate_code($current_class)}
#set $class_name = $current_class.namespaced_class_name.replace("::", ".")
#set $signature_name = $class_name+":destructor"
int ${current_class.methods.constructor.signature_name.replace("constructor", "destructor")}(lua_State* L)
{
    ${$current_class.namespaced_class_name}* cobj = nullptr;

    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "${class_name}", 0, &tolua_err)) goto tolua_lerror;

    cobj = (${$current_class.namespaced_class_name}*)tolua_tousertype(L, 1, 0);

    if (!cobj) 
    {
        tolua_error(L, "invalid 'cobj' in function '${signature_name}'", nullptr);
        return 0;
    }

    delete cobj;
    return 1;

tolua_lerror:
    tolua_error(L, "#ferror in function '${signature_name}'.", &tolua_err);
    return 0;
}

#end if
#
#set generator = $current_class.generator
#set methods = $current_class.methods_clean()
#set st_methods = $current_class.static_methods_clean()
#
#if $generator.script_control_cpp
static int ${generator.prefix}_${current_class.class_name}_finalize(lua_State* L)
{
    printf("luabindings: finalizing Lua object (${current_class.class_name})");
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "${current_class.class_name}", 0, &tolua_err) ||
        !tolua_isnoobj(L, 2, &tolua_err))
    {
        goto tolua_lerror;
    }
    else
    {
        ${current_class.namespaced_class_name}* self = (${current_class.namespaced_class_name}*) tolua_tousertype(L, 1, 0);
        if (!self) tolua_error(L, "invalid 'self' in function 'delete'", nullptr);
        delete self;
    }
    return 0;

tolua_lerror:
    tolua_error(L, "#ferror in function 'delete'.", &tolua_err);
    return 0;
}
#end if

int register_${generator.prefix}_${current_class.class_name}(lua_State* L)
{
    tolua_usertype(L, "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}");
    #if len($current_class.parents) > 0
        #if $generator.script_control_cpp and $current_class.has_constructor
    tolua_cclass(L, "${current_class.class_name}", "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}", "${generator.scriptname_from_native($current_class.parents[0].namespaced_class_name, $current_class.parents[0].namespace_name)}", ${generator.prefix}_${current_class.class_name}_finalize);
        #else
    tolua_cclass(L, "${current_class.class_name}", "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}", "${generator.scriptname_from_native($current_class.parents[0].namespaced_class_name, $current_class.parents[0].namespace_name)}", nullptr);
        #end if
    #else
        #if $generator.script_control_cpp and $current_class.has_constructor
    tolua_cclass(L, "${current_class.class_name}", "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}", "", lua_${generator.prefix}_${current_class.class_name}_finalize);
        #else
    tolua_cclass(L, "${current_class.class_name}", "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}", "", nullptr);
        #end if
    #end if

    tolua_beginmodule(L, "${current_class.class_name}");
    #if has_constructor
        tolua_function(L, "new", ${generator.prefix}_${current_class.class_name}_constructor);
        tolua_function(L, ".collector", ${generator.prefix}_${current_class.class_name}_destructor);
    #end if
    #for m in methods
        #set fn = m['impl']
        tolua_function(L, "${m['name']}", ${fn.signature_name});
    #end for
    #for m in st_methods
        #set fn = m['impl']
        tolua_function(L, "${m['name']}", ${fn.signature_name});
    #end for
    tolua_endmodule(L);
    std::string typeName = typeid(${current_class.namespaced_class_name}).name();
    g_luaType[typeName] = "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}";
    g_typeCast["${current_class.class_name}"] = "${generator.scriptname_from_native($current_class.namespaced_class_name, $current_class.namespace_name)}";
    return 1;
}
