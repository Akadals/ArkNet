#include <AkaNetCore/RingBuffer.h>

AkaNetCore::RingBuffer::RingBuffer(size_t size)
{
	size_t cap = 1;
	while (cap < size) cap <<= 1;

	mask = (capacity = cap) - 1;
	buf.resize(capacity);
}

bool AkaNetCore::RingBuffer::TryPush(const char* __restrict data, size_t len)
{
	if (!len) return true;

	size_t h = head.value.load(std::memory_order_acquire);
	size_t t = tail.value.load(std::memory_order_relaxed);

	if (capacity - (t - h) - 1 < len) return false;

	size_t idx = t & mask;

	if (len >= 64)
	{
		_mm_prefetch((char*)&buf[idx], _MM_HINT_T0);
		_mm_prefetch((char*)&buf[0], _MM_HINT_T0);
	}


	size_t resume = capacity - idx;
	size_t first = len < resume ? len : resume;

	memcpy(&buf[idx], data, first);
	memcpy(&buf[0], data + first, len - first);

	tail.value.store(t + len, std::memory_order_release);
	return true;
}

bool AkaNetCore::RingBuffer::TryPop(char* __restrict dest, size_t len)
{
	if (!len) return true;

	size_t h = head.value.load(std::memory_order_relaxed);
	size_t t = tail.value.load(std::memory_order_acquire);

	if (t - h < len) return false;

	size_t idx = h & mask;

	if (len >= 64)
	{
		_mm_prefetch((char*)&buf[idx], _MM_HINT_T0);
		_mm_prefetch((char*)&buf[0], _MM_HINT_T0);
	}

	size_t resume = capacity - idx;
	size_t first = len < resume ? len : resume;

	memcpy(dest, &buf[idx], first);
	memcpy(dest + first, &buf[0], len - first);

	head.value.store(h + len, std::memory_order_release);
	return true;
}

bool AkaNetCore::RingBuffer::Peek(char* __restrict dest, size_t len) const
{
	if (!len) return true;

	size_t h = head.value.load(std::memory_order_relaxed);
	size_t t = tail.value.load(std::memory_order_acquire);

	if ((t - h) < len) return false;

	size_t idx = h & mask;

	if (len >= 64)
	{
		_mm_prefetch((char*)&buf[idx], _MM_HINT_T0);
		_mm_prefetch((char*)&buf[0], _MM_HINT_T0);
	}

	size_t resume = capacity - idx;
	size_t first = len < resume ? len : resume;

	memcpy(dest, &buf[idx], first);
	memcpy(dest + first, &buf[0], len - first);

	return true;
}

size_t AkaNetCore::RingBuffer::Size() const
{
	size_t h = head.value.load(std::memory_order_relaxed);
	size_t t = tail.value.load(std::memory_order_acquire);
	return t - h;
}

void AkaNetCore::RingBuffer::Clear()
{
	head.value.store(0, std::memory_order_relaxed);
	tail.value.store(0, std::memory_order_relaxed);

	ZeroMemory(buf.data(), buf.size());
}
