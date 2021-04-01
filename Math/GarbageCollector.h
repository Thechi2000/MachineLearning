#pragma once
#include <vector>
#include <iostream>
#include "malloc.h"

//#define NOLEAKDETECTOR

#ifdef NOLEAKDETECTOR
#define NEW(cls) new cls
#define NEWARR(cls, count) new cls[count]
#define DEL(ptr) delete ptr
#define DELARR(ptr) delete[] ptr
#define GC_DEBUG
#else
#define NEW(cls) new (gc.allocate(sizeof(cls), __FILE__, __LINE__)) cls
#define NEWARR(cls, count) (cls*)gc.allocate(sizeof(cls) * count, __FILE__, __LINE__)
#define DEL(ptr) gc.del(ptr)
#define DELARR(ptr) gc.del(ptr)

#define GC_DEBUG GarbageCollector& garbage_collector = gc;

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