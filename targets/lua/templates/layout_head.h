\#pragma once
#if $macro_judgement
$macro_judgement
#end if
#if $hpp_headers
#for header in $hpp_headers
\#include "${header}"
#end for
#end if 

\#ifdef __cplusplus
extern "C" {
\#endif

\#include "tolua++.h"

\#ifdef __cplusplus
}
\#endif
