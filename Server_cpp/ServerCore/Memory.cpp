#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

Memory::Memory()
{
	int32 size = 0;
	int32 tableIndex = 0;

	for (size = 32; size <= 1024; size += 32)
	{
		MemoryPool* pool = new MemoryPool(size);
		mPools.emplace_back(pool);
		while (tableIndex <= size)
		{
			mPoolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
	for (; size <= 2024; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);
		mPools.emplace_back(pool);
		while (tableIndex <= size)
		{
			mPoolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
	for (; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);
		mPools.emplace_back(pool);

		while (tableIndex <= size)
		{
			mPoolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : mPools)
		delete pool;

	mPools.clear();
}

void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);
	if (allocSize > MAX_ALLOC_SIZE)
		header = reinterpret_cast<MemoryHeader*>(::malloc(allocSize));
	else
		header = mPoolTable[allocSize]->Pop();

	return MemoryHeader::AttackHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);
	const int32 allocSize = header->mAllocSize;
	ASSERT_CRASH(allocSize > 0);

	if (allocSize > MAX_ALLOC_SIZE)
		::free(header);
	else
		mPoolTable[allocSize]->Push(header);
}
