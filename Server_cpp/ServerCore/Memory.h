#pragma once

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