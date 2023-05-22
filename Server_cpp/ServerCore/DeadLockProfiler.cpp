#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(string name)
{
	LockGuard	guard(mLock);

	int32 lockId = 0;
	auto findIt = mNameToId.find(name);
	if (findIt == mNameToId.end())
	{
		lockId = static_cast<int32>(mNameToId.size());
		mNameToId[name] = lockId;
		mIdToName[lockId] = name;
	}
	else
		lockId = findIt->second;

	if (mLockStack.empty() == false)
	{
		const int32 prevId = mLockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = mLockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}
	mLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(string name)
{
	LockGuard	guard(mLock);

	if (mLockStack.empty())
		CRASH("MULTIPLE UNLOCK");

	int32 lockId = mNameToId[name];
	if (mLockStack.top() != lockId)
		CRASH("INVALID UNLOCK");
	mLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
}

void DeadLockProfiler::Dfs(int32 here)
{
}
