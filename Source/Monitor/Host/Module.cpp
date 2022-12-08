#ifdef SOUP_BUILD
module;
#endif

#ifdef _WIN32
#include <windows.h>

#ifdef CreateProcess
#undef CreateProcess
#endif

#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103)
#endif
#include <strsafe.h>
#pragma warning(pop)

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <atomic>
#include <array>
#include <codecvt>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

#ifdef SOUP_BUILD
export module Monitor.Host;
#ifdef _WIN32
import Detours;
import Monitor.Shared;
#endif
import Opal;

#else

// import Opal
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <functional>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <locale>
#include <map>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include "Utilities/Path.h"
#include "Utilities/SemanticVersion.h"
#include "IO/SystemConsoleManager.h"
#include "Logger/Log.h"
#include "Logger/ConsoleTraceListener.h"
#include "System/STLFileSystem.h"
#include "System/STLSystem.h"

#endif

using namespace Opal;

#ifdef SOUP_BUILD
#include "MockDetourProcessManager.h"
#include "ScopedDetourProcessManagerRegister.h"
#endif

#ifdef _WIN32
#include "WindowsDetourProcessManager.h"
#endif

#include "DetourCallbackLogger.h"
#include "ForkCallbackLogger.h"