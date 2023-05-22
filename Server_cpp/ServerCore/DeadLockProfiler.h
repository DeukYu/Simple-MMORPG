#pragma once
class DeadLockProfiler
{
public:
	void PushLock(string name);
	void PopLock(string name);
	void CheckCycle();

private:
	void Dfs(int32 here);

private:
	unordered_map<string, int32>	mNameToId;
	unordered_map<int32, string>	mIdToName;
	stack<int32>					mLockStack;
	map<int32, set<int32>>			mLockHistory;

	Mutex	mLock;
private:
	vector<int32>					mDiscoveredOrder;
	int32							mDiscoveredCount = 0;
	vector<bool>					mFinished;
	vector<int32>					mParent;
};

