#pragma once

namespace htf
{

// * 仅 DEBUG 时有效
    
#define _HTF_ERROR_WHERE __FILE__ ":" + std::to_string(__LINE__) + ": "

#ifdef DEBUG
#define HTF_ERROR_WHERE _HTF_ERROR_WHERE
#else
#define HTF_ERROR_WHERE ""
#endif

}//