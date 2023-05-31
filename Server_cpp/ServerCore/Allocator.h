#pragma once

class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void	 Release(void* ptr);
};

class StompAllocator
{
	enum MyEnum
	{
		PAGE_SIZE = 0x1000
	};

public:
	static void* Alloc(int32 size);
	static void  Release(void* ptr);
};