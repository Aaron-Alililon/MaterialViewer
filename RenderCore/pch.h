#ifndef PCH_H
#define PCH_H

#include <concepts>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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
