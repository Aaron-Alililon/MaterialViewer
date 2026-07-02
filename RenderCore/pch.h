#ifndef PCH_H
#define PCH_H

#include <concepts>
#include <vector>
#include <array>
#include <span>
#include <memory>
#include <algorithm>
#include <queue>
#include <filesystem>
#include <fstream>
#include <numbers>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <wrl/client.h>

#include <assert.h>

#include "Logger.h"

#endif //PCH_H
