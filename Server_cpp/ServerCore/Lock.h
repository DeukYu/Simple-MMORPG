#pragma once
#include "Types.h"

// W -> R (O)
// R -> W (X)
class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> mLockFlag;
	uint16 mWriteCount = 0;
};

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : mLock(lock) { mLock.ReadLock(); }
	~ReadLockGuard() { mLock.ReadUnlock(); }
private:
	Lock& mLock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : mLock(lock) { mLock.WriteLock(); }
	~WriteLockGuard() { mLock.WriteUnlock(); }
private:
	Lock& mLock;
};