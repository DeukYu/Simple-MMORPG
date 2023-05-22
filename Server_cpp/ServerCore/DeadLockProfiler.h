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
	vector<int32>					mDiscoveredOrder;		// 노드가 발견된 순서를 기록하는 배열
	int32							mDiscoveredCount = 0;	// 노드가 발견된 순서
	vector<bool>					mFinished;				// Dfs(i)가 종료 되었는지 여부
	vector<int32>					mParent;
};

