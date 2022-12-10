//
// Copyright (c) 2022, Zoe J. Bare
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

#include "Array.hpp"

#include <assert.h>
#include <functional>

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_HASH_MAP_DEFAULT_BUCKET_COUNT 128

//----------------------------------------------------------------------------------------------------------------------

template <typename TKey, typename TValue, typename FHasher = std::hash<TKey>, typename FCompare = std::equal_to<TKey>>
struct HqHashMap
{
	struct NodeData
	{
		TKey key;
		TValue value;
	};

	struct Node
	{
		NodeData data;

		bool occupied;
	};

	struct Iterator
	{
		Iterator() : pData(nullptr), bucketIndex(0), nodeIndex(0) {}

		NodeData* pData;

		size_t bucketIndex;
		size_t nodeIndex;
	};

	typedef HqArray<Node>      NodeArray;
	typedef HqArray<NodeArray> BucketArray;

	BucketArray buckets;

	size_t count;

	static void Initialize(HqHashMap& output)
	{
		output.count = 0;

		BucketArray::Initialize(output.buckets);
	}

	static void Dispose(HqHashMap& map)
	{
		if(map.buckets.pData)
		{
			// Dispose of each bucket.
			for(size_t bucketIndex = 0; bucketIndex < map.buckets.capacity; ++bucketIndex)
			{
				NodeArray::Dispose(map.buckets.pData[bucketIndex]);
			}

			// Dispose of the map.
			BucketArray::Dispose(map.buckets);

			Initialize(map);
		}
	}

	static void Allocate(HqHashMap& output)
	{
		Initialize(output);

		BucketArray::Reserve(output.buckets, _HQ_HASH_MAP_DEFAULT_BUCKET_COUNT);

		// Initialize each bucket.
		for(size_t bucketIndex = 0; bucketIndex < output.buckets.capacity; ++bucketIndex)
		{
			NodeArray& bucket = output.buckets.pData[bucketIndex];

			// Initialize the bucket and reserve the bare minimum number of nodes.
			NodeArray::Initialize(bucket);
			NodeArray::Reserve(bucket, 1);

			// Initialize each node in the bucket.
			for(size_t nodeIndex = 0; nodeIndex < bucket.capacity; ++nodeIndex)
			{
				Node& node = bucket.pData[nodeIndex];

				node.occupied = false;
			}
		}
	}

	static void Copy(HqHashMap& dest, const HqHashMap& src)
	{
		Dispose(dest);

		if(src.buckets.pData)
		{
			Allocate(dest);

			Iterator iter;
			while(IterateNext(const_cast<HqHashMap&>(src), iter))
			{
				Insert(dest, iter.pData->key, iter.pData->value);
			}
		}
	}

	static void Clear(HqHashMap& map)
	{
		if(map.buckets.pData && map.count > 0)
		{
			// Visit each bucket.
			for(size_t bucketIndex = 0; bucketIndex < map.buckets.capacity; ++bucketIndex)
			{
				NodeArray& bucket = map.buckets.pData[bucketIndex];

				// Clear each occupied node in the current bucket.
				for(size_t nodeIndex = 0; nodeIndex < bucket.count; ++nodeIndex)
				{
					bucket.pData[nodeIndex].occupied = false;
				}

				bucket.count = 0;
			}

			map.count = 0;
		}
	}

	static bool Insert(HqHashMap& map, const TKey& key, const TValue& value)
	{
		if(map.buckets.pData)
		{
			const FHasher hasher = FHasher();
			const FCompare compare = FCompare();

			const size_t keyHash = hasher(key);
			const size_t bucketIndex = keyHash % map.buckets.capacity;

			NodeArray& bucket = map.buckets.pData[bucketIndex];

			bool foundKeyNode = false;
			bool foundFreeNode = false;
			size_t insertIndex = 0;

			// We could iterate through twice to search for a node with the input key, then the first unoccupied node,
			// but since we have to go through the whole bucket regardless, it makes more sense to do it all in a single pass.
			for(size_t nodeIndex = 0; nodeIndex < bucket.capacity; ++nodeIndex)
			{
				Node& node = bucket.pData[nodeIndex];

				// Buckets are tightly packed, so if we hit an unoccupied node,
				// that means the input key doesn't exist in the map yet.
				if(!node.occupied)
				{
					foundFreeNode = true;
					insertIndex = nodeIndex;
					break;
				}

				// The current node is occupied, so check to see if it matches the input key.
				// If it does, we'll effectively overwrite it with the input value.
				if(compare(key, node.data.key))
				{
					foundKeyNode = true;
					insertIndex = nodeIndex;
					break;
				}
			}

			if(!foundKeyNode && !foundFreeNode)
			{
				// Set the insert index to the end of the bucket.
				foundFreeNode = true;
				insertIndex = bucket.capacity;

				// Resize the bucket to accomodate the new node
				NodeArray::Reserve(bucket, bucket.capacity + 1);
				assert(bucket.pData != nullptr);
			}

			Node& node = bucket.pData[insertIndex];

			node.data.key = key;
			node.data.value = value;
			node.occupied = true;

			if(!foundKeyNode)
			{
				++bucket.count;
				++map.count;
				return true;
			}

			return false;
		}

		return false;
	}

	static bool Remove(HqHashMap& map, const TKey& key)
	{
		if(map.buckets.pData && map.count > 0)
		{
			const FHasher hasher = FHasher();
			const FCompare compare = FCompare();

			const size_t keyHash = hasher(key);
			const size_t bucketIndex = keyHash % map.buckets.capacity;

			NodeArray& bucket = map.buckets.pData[bucketIndex];

			// Search for the node inside the bucket.
			for(size_t nodeIndex = 0; nodeIndex < bucket.capacity; ++nodeIndex)
			{
				Node& node = bucket.pData[nodeIndex];

				// Buckets are tightly packed, so we can stop searching at the first unoccupied node.
				if(!node.occupied)
				{
					break;
				}

				// Check if the node matches the input key, but only if this is an occupied node.
				if(compare(key, node.data.key))
				{
					--bucket.count;
					--map.count;

					// Swap the data from the last node in this bucket to the current
					// node to keep the bucket tightly packed.
					bucket.pData[nodeIndex].data = bucket.pData[bucket.count].data;
					bucket.pData[bucket.count].occupied = false;
					return true;
				}
			}
		}

		return false;
	}

	static bool Contains(HqHashMap& map, const TKey& key)
	{
		if(map.buckets.pData && map.count > 0)
		{
			const FHasher hasher = FHasher();
			const FCompare compare = FCompare();

			const size_t keyHash = hasher(key);
			const size_t bucketIndex = keyHash % map.buckets.capacity;

			NodeArray& bucket = map.buckets.pData[bucketIndex];

			// Search for the node inside the bucket.
			for(size_t nodeIndex = 0; nodeIndex < bucket.capacity; ++nodeIndex)
			{
				Node& node = bucket.pData[nodeIndex];

				// Buckets are tightly packed, so we can stop searching at the first unoccupied node.
				if(!node.occupied)
				{
					break;
				}

				// Check if the node matches the input key, but only if this is an occupied node.
				if(compare(key, node.data.key))
				{
					return true;
				}
			}
		}

		return false;
	}

	static bool Get(HqHashMap& map, const TKey& key, TValue& outputValue)
	{
		if(map.buckets.pData && map.count > 0)
		{
			const FHasher hasher = FHasher();
			const FCompare compare = FCompare();

			const size_t keyHash = hasher(key);
			const size_t bucketIndex = keyHash % map.buckets.capacity;

			NodeArray& bucket = map.buckets.pData[bucketIndex];

			// Search for the node inside the bucket.
			for(size_t nodeIndex = 0; nodeIndex < bucket.capacity; ++nodeIndex)
			{
				Node& node = bucket.pData[nodeIndex];

				// Buckets are tightly packed, so we can stop searching at the first unoccupied node.
				if(!node.occupied)
				{
					break;
				}

				// Check if the node matches the input key, but only if this is an occupied node.
				if(compare(key, node.data.key))
				{
					outputValue = node.data.value;
					return true;
				}
			}
		}

		return false;
	}

	static bool Set(HqHashMap& map, const TKey& key, const TValue& value)
	{
		if(map.buckets.pData && map.count > 0)
		{
			const FHasher hasher = FHasher();
			const FCompare compare = FCompare();

			const size_t keyHash = hasher(key);
			const size_t bucketIndex = keyHash % map.buckets.capacity;

			NodeArray& bucket = map.buckets.pData[bucketIndex];

			// Search for the node inside the bucket.
			for(size_t nodeIndex = 0; nodeIndex < bucket.capacity; ++nodeIndex)
			{
				Node& node = bucket.pData[nodeIndex];

				// Buckets are tightly packed, so we can stop searching at the first unoccupied node.
				if(!node.occupied)
				{
					break;
				}

				// Check if the node matches the input key, but only if this is an occupied node.
				if(compare(key, node.data.key))
				{
					node.data.value = value;
					return true;
				}
			}
		}

		return false;
	}

	static bool IterateNext(HqHashMap& map, Iterator& iter)
	{
		if(map.buckets.pData && map.count > 0)
		{
			if(!iter.pData)
			{
				// This is an empty iterator, so we start back at the beginning.
				iter.bucketIndex = 0;
				iter.nodeIndex = 0;
			}

			for(; iter.bucketIndex < map.buckets.capacity; ++iter.bucketIndex)
			{
				NodeArray& bucket = map.buckets.pData[iter.bucketIndex];
				if(iter.nodeIndex == bucket.count)
				{
					// We're at the end of the current bucket; wrap around to the start of the next bucket.
					iter.nodeIndex = 0;
					continue;
				}

				Node& node = bucket.pData[iter.nodeIndex];

				iter.pData = &node.data;

				// Move to the next node index, so the next time this function is called,
				// it knows where to pick up scanning for occupied nodes.
				++iter.nodeIndex;

				return true;
			}

			iter.pData = nullptr;
		}

		return false;
	}
};

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_HASH_MAP_DEFAULT_BUCKET_COUNT

//----------------------------------------------------------------------------------------------------------------------
