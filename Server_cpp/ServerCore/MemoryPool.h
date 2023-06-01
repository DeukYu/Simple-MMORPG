#pragma once

DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 size) : mAllocSize(size) {}

	static void* AttackHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 mAllocSize;
};

DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
class MemoryPool
{
	USE_LOCK;

public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER	mHeader;
	int32			mAllocSize = 0;
	atomic<int32> mAllocCount = 0;
};