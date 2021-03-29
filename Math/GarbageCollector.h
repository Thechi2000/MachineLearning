#pragma once
#include <vector>
#include <iostream>
#include "malloc.h"

#ifdef NOLEAKDETECTOR
#define NEW(cls) new cls
#define NEWARR(cls, count) new[count] cls
#define DEL(ptr) delete ptr
#else
#define NEW(cls) new (gc.allocate(sizeof(cls), __FILE__, __LINE__)) cls
#define NEWARR(cls, count) (cls*)gc.allocate(sizeof(cls) * count, __FILE__, __LINE__)
#define DEL(ptr) gc.del(ptr)

class GarbageCollector
{
private:
	struct AllocatedMemoryBlock
	{
	public:
		void* ptr;
		const char* instanciationFile;
		int instanciationLine;
	};
	std::vector<AllocatedMemoryBlock> m_allocatedMemorySlots;

public:
	~GarbageCollector();

	void* allocate(int size, const char* instanciationFile, int instanciationLine);
	void del(void* ptr);
};

extern GarbageCollector gc;

#endif