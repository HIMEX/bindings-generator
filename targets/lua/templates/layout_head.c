\#include "${out_file}.h"
#if $macro_judgement
$macro_judgement
#end if
#for header in $headers
    #set relative = os.path.relpath(header, $search_path)
    #if not '..' in relative
\#include "${relative.replace(os.path.sep, '/')}"
    #else
\#include "${os.path.basename(header)}"
    #end if
#end for
\#include "tolua_fix.h"
\#include "LuaBasicConversions.h"
\#include "lua_himex_conversions.h"
\#include "lua_helpers.h"
#if $cpp_headers
#for header in $cpp_headers
\#include "${header}"
#end for
#end if
