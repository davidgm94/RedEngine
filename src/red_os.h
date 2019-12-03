#pragma once

// TODO: update preprocessor names (not correct at the moment)
#if _WIN64
#include "os/red_win32.h"
#elif __linux__
#include "os/red_linux.h"
#elif __APPLE__
#endif
