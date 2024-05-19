#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "CautionDefinitions.h"

// # STD ----------------------------
#include <string>
#include <algorithm>
#include <type_traits>
#include <memory>
#include <fstream>
#include <sstream>
#include <limits>
#include <cassert>
#include <cstdint>
#include <atomic>
#include <map>
#include <unordered_map>
#include <cstdio>
#include <filesystem>
#include <regex>
#include <locale>
#include <codecvt>
#include <array>
#include <deque>
#include <vector> 
#include <iostream>
#include <cstdarg>
#include <thread>
#include <condition_variable>
#include <set>
#include <variant>
#include <cstring>

// CautionCommon
#include <Platform/CautionPlatform.h>

// # DX and Platform
#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
//#include <d3dx12.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

// Thirdparty Libraries
#include <json.hpp>
#include <glm/glm.hpp>