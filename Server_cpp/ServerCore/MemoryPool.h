#pragma once

struct MemoryHeader
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

class MemoryPool
{
	USE_LOCK;

public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	int32 mAllocSize = 0;
	atomic<int32> mAllocCount = 0;
	queue<MemoryHeader*> mQueue;
};