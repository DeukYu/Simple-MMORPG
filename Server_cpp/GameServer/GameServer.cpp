#include "pch.h"
#include "ThreadManager.h"

class TestLock
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;

		if (mQueue.empty())
			return -1;
		return mQueue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;
		mQueue.push(rand() % 100);
	}
	void TestPop()
	{
		WRITE_LOCK;
		if (mQueue.empty() == false)
			mQueue.pop();
	}
private:
	queue<int32>	mQueue;
};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(10ms);
		testLock.TestPop();
	}
	
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(100ms);
	}
}

int main()
{
	for (int32 i = 0; i < 2; ++i)
	{
		GThreadManager->Launch(ThreadWrite);
	}
	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch(ThreadRead);
	}
	GThreadManager->Join();
}