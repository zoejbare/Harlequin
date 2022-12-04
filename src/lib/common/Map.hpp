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

#include "../Harlequin.h"

//----------------------------------------------------------------------------------------------------------------------

#define HQ_MAP_IMPL_FORCE_STD_MAP           0
#define HQ_MAP_IMPL_FORCE_STD_UNORDERED_MAP 0

#if defined(HQ_PLATFORM_PS3)
	#undef HQ_MAP_IMPL_FORCE_STD_MAP
	#define HQ_MAP_IMPL_FORCE_STD_MAP 1

#elif defined(HQ_PLATFORM_PSVITA)
	#undef HQ_MAP_IMPL_FORCE_STD_UNORDERED_MAP
	#define HQ_MAP_IMPL_FORCE_STD_UNORDERED_MAP 1

#endif

//----------------------------------------------------------------------------------------------------------------------

#if HQ_MAP_IMPL_FORCE_STD_MAP
	#include <map>

	#define HQ_MAP_IS_UNORDERED 0
	#define HQ_MAP_TYPE std::map
	#define HQ_MAP_NODE_TYPE(key_type, value_type) std::pair<key_type const, value_type>
	#define HQ_MAP_FUNC_INSERT(dict, key, value) ((dict)[key] = (value))
	#define HQ_MAP_FUNC_REMOVE(dict, key) (dict).erase(key)
	#define HQ_MAP_FUNC_CONTAINS(dict, key) ((dict).count(key) > 0)

	#if defined(HQ_PLATFORM_PS3)
		#define HQ_MAP_FUNC_GET(dict, key) (dict)[(key)]
	#else
		#define HQ_MAP_FUNC_GET(dict, key) (dict).at(key)
	#endif

	#define HQ_MAP_FUNC_RESERVE(dict, count) // No equivalent function for std::map
	#define HQ_MAP_FUNC_CLEAR(dict) (dict).clear()
	#define HQ_MAP_FUNC_SIZE(dict) (dict).size()
	#define HQ_MAP_ITER_KEY(iter) (iter).first
	#define HQ_MAP_ITER_VALUE(iter) (iter).second
	#define HQ_MAP_ITER_PTR_KEY(iter) (iter)->first
	#define HQ_MAP_ITER_PTR_VALUE(iter) (iter)->second

#elif HQ_MAP_IMPL_FORCE_STD_UNORDERED_MAP
	#include <unordered_map>

	#define HQ_MAP_IS_UNORDERED 1
	#define HQ_MAP_TYPE std::unordered_map
	#define HQ_MAP_NODE_TYPE(key_type, value_type) std::pair<key_type const, value_type>
	#define HQ_MAP_FUNC_INSERT(dict, key, value) ((dict)[key] = (value))
	#define HQ_MAP_FUNC_REMOVE(dict, key) (dict).erase(key)
	#define HQ_MAP_FUNC_CONTAINS(dict, key) ((dict).count(key) > 0)
	#define HQ_MAP_FUNC_GET(dict, key) (dict).at(key)
	#define HQ_MAP_FUNC_RESERVE(dict, count) (dict).reserve(count)
	#define HQ_MAP_FUNC_CLEAR(dict) (dict).clear()
	#define HQ_MAP_FUNC_SIZE(dict) (dict).size()
	#define HQ_MAP_ITER_KEY(iter) (iter).first
	#define HQ_MAP_ITER_VALUE(iter) (iter).second
	#define HQ_MAP_ITER_PTR_KEY(iter) (iter)->first
	#define HQ_MAP_ITER_PTR_VALUE(iter) (iter)->second

#else
	#include <SkipProbe/SkipProbe.hpp>

	#define HQ_MAP_IS_UNORDERED 1
	#define HQ_MAP_TYPE SkipProbe::HashMap
	#define HQ_MAP_NODE_TYPE(key_type, value_type) SkipProbe::LinkedNode<key_type, value_type>
	#define HQ_MAP_FUNC_INSERT(dict, key, value) (dict).Insert((key), (value))
	#define HQ_MAP_FUNC_REMOVE(dict, key) (dict).Delete(key)
	#define HQ_MAP_FUNC_CONTAINS(dict, key) (dict).Contains(key)
	#define HQ_MAP_FUNC_GET(dict, key) (dict).Get(key)
	#define HQ_MAP_FUNC_RESERVE(dict, count) (dict).Reserve(count)
	#define HQ_MAP_FUNC_CLEAR(dict) (dict).Clear()
	#define HQ_MAP_FUNC_SIZE(dict) (dict).Size()
	#define HQ_MAP_ITER_KEY(iter) (iter).key
	#define HQ_MAP_ITER_VALUE(iter) (iter).value
	#define HQ_MAP_ITER_PTR_KEY(iter) (iter)->key
	#define HQ_MAP_ITER_PTR_VALUE(iter) (iter)->value

#endif

//----------------------------------------------------------------------------------------------------------------------

#undef HQ_MAP_IMPL_FORCE_STD_MAP
#undef HQ_MAP_IMPL_FORCE_STD_UNORDERED_MAP

//----------------------------------------------------------------------------------------------------------------------
