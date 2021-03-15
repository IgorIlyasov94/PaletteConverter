#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <shobjidl.h>

#include <wrl/client.h>

#include <vector>
#include <array>
#include <exception>
#include <utility>
#include <algorithm>
#include <memory>
#include <iterator>
#include <any>
#include <random>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <locale>
#include <codecvt>

using namespace Microsoft::WRL;
