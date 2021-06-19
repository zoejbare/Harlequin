//
// Copyright (c) 2021, Zoe J. Bare
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "../XenonScript.h"

//----------------------------------------------------------------------------------------------------------------------

// These should only ever be set to a non-zero value when testing those specific implementations.
#define XENON_MAP_TEST_FORCE_STD_MAP 0
#define XENON_MAP_TEST_FORCE_STD_UNORDERED_MAP 0

//----------------------------------------------------------------------------------------------------------------------

#if XENON_MAP_TEST_FORCE_STD_MAP || defined(XENON_PLATFORM_PS3)
	#include <map>

	#define XENON_MAP_IS_UNORDERED 0
	#define XENON_MAP_TYPE std::map
	#define XENON_MAP_NODE_TYPE(key_type, value_type) std::pair<key_type const, value_type>
	#define XENON_MAP_FUNC_INSERT(dict, key, value) ((dict)[key] = (value))
	#define XENON_MAP_FUNC_REMOVE(dict, key) (dict).erase(key)
	#define XENON_MAP_FUNC_CONTAINS(dict, key) ((dict).count(key) > 0)
	#define XENON_MAP_FUNC_GET(dict, key) (dict).at(key)
	#define XENON_MAP_FUNC_RESERVE(dict, count) // No equivalent function for std::map
	#define XENON_MAP_FUNC_CLEAR(dict) (dict).clear()
	#define XENON_MAP_FUNC_SIZE(dict) (dict).size()
	#define XENON_MAP_ITER_KEY(iter) (iter).first
	#define XENON_MAP_ITER_VALUE(iter) (iter).second
	#define XENON_MAP_ITER_PTR_KEY(iter) (iter)->first
	#define XENON_MAP_ITER_PTR_VALUE(iter) (iter)->second

#elif XENON_MAP_TEST_FORCE_STD_UNORDERED_MAP || defined(XENON_PLATFORM_XBOX_360) || defined(XENON_PLATFORM_PSVITA)
	#include <unordered_map>

	#define XENON_MAP_IS_UNORDERED 1
	#define XENON_MAP_TYPE std::unordered_map
	#define XENON_MAP_NODE_TYPE(key_type, value_type) std::pair<key_type const, value_type>
	#define XENON_MAP_FUNC_INSERT(dict, key, value) ((dict)[key] = (value))
	#define XENON_MAP_FUNC_REMOVE(dict, key) (dict).erase(key)
	#define XENON_MAP_FUNC_CONTAINS(dict, key) ((dict).count(key) > 0)
	#define XENON_MAP_FUNC_GET(dict, key) (dict).at(key)
	#define XENON_MAP_FUNC_RESERVE(dict, count) (dict).reserve(count)
	#define XENON_MAP_FUNC_CLEAR(dict) (dict).clear()
	#define XENON_MAP_FUNC_SIZE(dict) (dict).size()
	#define XENON_MAP_ITER_KEY(iter) (iter).first
	#define XENON_MAP_ITER_VALUE(iter) (iter).second
	#define XENON_MAP_ITER_PTR_KEY(iter) (iter)->first
	#define XENON_MAP_ITER_PTR_VALUE(iter) (iter)->second

#else
	#include <SkipProbe/SkipProbe.hpp>

	#define XENON_MAP_IS_UNORDERED 1
	#define XENON_MAP_TYPE SkipProbe::HashMap
	#define XENON_MAP_NODE_TYPE(key_type, value_type) SkipProbe::LinkedNode<key_type, value_type>
	#define XENON_MAP_FUNC_INSERT(dict, key, value) (dict).Insert((key), (value))
	#define XENON_MAP_FUNC_REMOVE(dict, key) (dict).Delete(key)
	#define XENON_MAP_FUNC_CONTAINS(dict, key) (dict).Contains(key)
	#define XENON_MAP_FUNC_GET(dict, key) (dict).Get(key)
	#define XENON_MAP_FUNC_RESERVE(dict, count) (dict).Reserve(count)
	#define XENON_MAP_FUNC_CLEAR(dict) (dict).Clear()
	#define XENON_MAP_FUNC_SIZE(dict) (dict).Size()
	#define XENON_MAP_ITER_KEY(iter) (iter).key
	#define XENON_MAP_ITER_VALUE(iter) (iter).value
	#define XENON_MAP_ITER_PTR_KEY(iter) (iter)->key
	#define XENON_MAP_ITER_PTR_VALUE(iter) (iter)->value

#endif

//----------------------------------------------------------------------------------------------------------------------

#undef XENON_MAP_TEST_FORCE_STD_MAP
#undef XENON_MAP_TEST_FORCE_STD_UNORDERED_MAP

//----------------------------------------------------------------------------------------------------------------------
