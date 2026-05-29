#pragma once
#include <AkaNetCore/Platform/Win32/Core.h>

#include <atomic>

namespace AkaNetCore
{
	class LockFreePoolAvailable
	{
		virtual void Init() = 0;
	};

	template<typename T> class LockFreePool
	{
	private:
		struct Node
		{
			T*				data;
			Node*			next;
		};
		struct TaggedPtr
		{
			Node*			ptr;
			uint64_t		tag;
		};
		std::atomic <TaggedPtr>	head;
	public:
		LockFreePool();
		void Push(T* obj);
		T* Pop();
	};
	template<typename T> using LFPOOL = LockFreePool<T>;

	template<typename T> class WrappedLockFreePool
	{
		static_assert(std::is_base_of<LockFreePoolAvailable, T>::value,
			"T must derive from LockFreePoolAvailable");
	private:
		LFPOOL<T> pool = {};
	public:
		WrappedLockFreePool(size_t size);
		T* Acquire();
		void Release(T* obj);
	};
	template<typename T> using WLFPOOL = WrappedLockFreePool<T>;
}