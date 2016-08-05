
int ${out_value}Func = toluafix_ref_function(L, ${arg_idx}, 0);
if (${out_value}Func != 0)
{
    ${out_value} = [=](${lambda_parameters}) -> ${ret_type.name} {
        #set arg_count = len($param_types)
        #set $count = 0
        auto lock = cocos2d::ScriptEngineManager::getInstance()->getLockGuard();
        
        toluafix_get_function_by_refid(L, ${out_value}Func);
        #while $count < $arg_count
            #set $arg = $param_types[$count]
        ${arg.from_native({ "generator": $generator,
                            "in_value": "larg" + str(count),
                            "out_value": "largv[" + str(count) + "]",
                            "class_name": $class_name,
                            "level": 2,
                            "ntype": str($arg) })};
            #set $count = $count + 1
        #end while

        auto S = LuaEngine::getInstance()->getLuaStack();
        #if $ret_type.name != "void"
        return S->executeFunction(${arg_count});
        #else
        S->executeFunction(${arg_count});
        #end if
    };
}