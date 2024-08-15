#pragma once

#include "./redbud/io/color.h"

namespace htf
{
namespace color
{

// ------------------------------------------------------------------
// ! 只用于 color
#define GREEN redbud::io::state::manual << redbud::io::fg::green
#define RED redbud::io::state::manual << redbud::io::fg::red
#define YELLOW redbud::io::state::manual << redbud::io::fg::yellow
#define RESET redbud::io::state::manual << redbud::io::fg::reset
// ------------------------------------------------------------------

#define COLOR(color, what) \
    color << what << RESET

}   //
}   // namespace htf
