#include <AkaNetCore/LockFreePool.h>

template<typename T> AkaNetCore::LockFreePool<T>::LockFreePool()
{
	head.store({ nullptr,0 }, std::memory_order_relaxed);
}

template<typename T>
void AkaNetCore::LockFreePool<T>::Push(T* obj)
{
	Node* node = new Node{ obj, nullptr };

	TaggedPtr oldHead = head.load(std::memory_order_acquire);

	while (true)
	{
		node->next = oldHead.ptr;

		TaggedPtr newHead;
		newHead.ptr = node;
		newHead.tag = oldHead.tag + 1;

		if (head.compare_exchange_weak(
			oldHead,
			newHead,
			std::memory_order_release,
			std::memory_order_acquire)) return;
	}
}

template<typename T>
T* AkaNetCore::LockFreePool<T>::Pop()
{
	TaggedPtr oldHead = head.load(std::memory_order_acquire);

	while (oldHead.ptr)
	{
		Node* next = oldHead.ptr->next;

		TaggedPtr newHead;
		newHead.ptr = next;
		newHead.tag = oldHead.tag + 1;

		if (head.compare_exchange_weak(
			oldHead,
			newHead,
			std::memory_order_acq_rel,
			std::memory_order_acquire))
		{
			T* result = oldHead.ptr->data;

			delete oldHead.ptr;

			return result;
		}
	}

	return nullptr;
}

template<typename T>
AkaNetCore::WrappedLockFreePool<T>::WrappedLockFreePool(size_t size)
{
	for (int i = 0; i < size; i++) pool.Push(new T);
}

template<typename T>
T* AkaNetCore::WrappedLockFreePool<T>::Acquire()
{
	if (T* obj = pool.Pop()) return obj;
	else return new T();
}

template<typename T>
void AkaNetCore::WrappedLockFreePool<T>::Release(T* obj)
{
	obj->Init();
	pool.Push(obj);
}
