#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <array>
#include <functional>
#include <algorithm>
#include <utility>
#include <cmath>
#include <cstdint>
#include <memory>
#include <chrono>
#include <regex>
#include <optional>

#include "GBC/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "GBC/Core/Logger.h"
#include "GBC/Core/Core.h"
#include "GBC/Debug/Profiler.h"

#if GBC_PLATFORM_WINDOWS
	#include <windows.h>
#endif
