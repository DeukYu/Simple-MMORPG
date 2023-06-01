#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize) : mAllocSize(allocSize)
{
	::InitializeSListHead(&mHeader);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&mHeader)))
	{
		::_aligned_free(memory);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->mAllocSize = 0;

	// Pool에 메모리 반납
	::InterlockedPushEntrySList(&mHeader, static_cast<PSLIST_ENTRY>(ptr));
	mAllocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
    MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&mHeader));

    // 없으면 새로 만든다
    if (memory == nullptr)
    {
        memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(mAllocSize, MEMORY_ALLOCATION_ALIGNMENT));
    }
    else
    {
        ASSERT_CRASH(memory->mAllocSize == 0);
    }

    mAllocCount.fetch_add(1);

    return memory;
}
