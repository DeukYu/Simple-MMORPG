#include "pch.h"
#include "Lock.h"
#include "CoreTLS.cpp"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const string_view& name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif // _DEBUG

	// 동일한 쓰레드가 소유하고 있다면 무조건 성공한다.
	const uint32 lockThreadId = (mLockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		mWriteCount++;
		return;
	}
	// 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 획득한다.
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (mLockFlag.compare_exchange_strong(OUT expected, desired))
			{
				mWriteCount++;
				return;
			}
		}
		if (const int64 endTick = ::GetTickCount64(); endTick - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");
		this_thread::yield();
	}
}

void Lock::WriteUnlock(const string_view& name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif // _DEBUG

	// ReadLock 다풀기 전에는 WriteLock 불가능
	if ((mLockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER");

	const int32 lockCount = --mWriteCount;
	if (lockCount == 0)
		mLockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const string_view& name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif // _DEBUG
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (mLockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		mLockFlag.fetch_add(1);
		return;
	}
	// 아무도 소유하고 있지 않을 때, 경합해서 공유 카운트를 올린다.
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (mLockFlag.load() & READ_COUNT_MASK);
			if (mLockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (const int64 endTick = ::GetTickCount64(); endTick - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock(const string_view& name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif // _DEBUG
	if ((mLockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
