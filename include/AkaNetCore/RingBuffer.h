#pragma once
#include <AkaNetCore/Core.h>

#include <atomic>
#include <vector>

namespace AkaNetCore
{
	typedef class RingBuffer RBUF;

	class RingBuffer
	{
	private:
		vector<char>				buf;

		struct alignas(64) CacheLineAtomic
		{
			atomic<size_t>			value = 0;
			char					padding[64 - sizeof(atomic<size_t>)];
		};

		mutable CacheLineAtomic		head;
		mutable CacheLineAtomic		tail;

		size_t						capacity;
		size_t						mask;
	public:
		RingBuffer(size_t size);
		bool TryPush(const char* __restrict data, size_t len);
		bool TryPop(char* __restrict dest, size_t len);
		bool Peek(char* __restrict dest, size_t len) const;
		size_t Size() const;
		void Clear();
	};
}