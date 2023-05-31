#include "pch.h"
#include "ThreadManager.h"

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	int32 _hp = 100;
	int32 _mp = false;
};

class Player
{
public:
	Player()
	{
		cout << "Player()" << endl;
	}

	~Player()
	{
		cout << "~Player()" << endl;
	}
};

int main()
{
	SYSTEM_INFO	info;
	::GetSystemInfo(&info);

	cout << info.dwPageSize << endl;
	cout << info.dwAllocationGranularity << endl;

	Knight* knight = (Knight*)CustomNew<Player>();

	knight->_hp = 100;
	CustomDelete(knight);
    return 0;
}