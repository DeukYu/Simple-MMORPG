#pragma once
#include "Allocator.h"

class MemoryPool;
class Memory
{
	enum
	{
		// ~1024���� 32����, ~2048���� 128����, ~4096���� 256����
		POOL_COUNT = (1024 / 32) + (2048 / 128) + (4096 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*>	mPools;
	MemoryPool*			mPoolTable[MAX_ALLOC_SIZE + 1];
};

template <typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...);	// placement new
	return memory;
}

template <typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	XRelease(obj);
}