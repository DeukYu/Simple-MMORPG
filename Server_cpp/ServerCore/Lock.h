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
	void WriteLock(const string name);
	void WriteUnlock(const string name);
	void ReadLock(const string name);
	void ReadUnlock(const string name);

private:
	Atomic<uint32> mLockFlag = EMPTY_FLAG;
	uint16 mWriteCount = 0;
};

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const string name) : mLock(lock), mName(name) { mLock.ReadLock(name); }
	~ReadLockGuard() { mLock.ReadUnlock(mName); }
private:
	Lock& mLock;
	const string mName;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const string name) : mLock(lock), mName(name) { mLock.WriteLock(name); }
	~WriteLockGuard() { mLock.WriteUnlock(mName); }
private:
	Lock& mLock;
	const string mName;
};