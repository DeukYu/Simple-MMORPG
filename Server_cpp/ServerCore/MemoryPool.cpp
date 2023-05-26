#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize) : mAllocSize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (mQueue.empty() == false)
	{
		MemoryHeader* header = mQueue.front();
		mQueue.pop();
		::free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	ptr->mAllocSize = 0;

	// ¸Þ¸ð¸® ¹Ý³³
	mQueue.push(ptr);
	mAllocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;
	{
		WRITE_LOCK;
		if (mQueue.empty() == false)
		{
			header = mQueue.front();
			mQueue.pop();
		}
	}

	if (header == nullptr)
		header = reinterpret_cast<MemoryHeader*>(::malloc(mAllocSize));
	else
		ASSERT_CRASH(header->mAllocSize == 0);

	mAllocCount.fetch_add(1);

	return header;
}
