#include "GarbageCollector.h"

#ifndef NOLEAKDETECTOR
#include "Console.hpp"
#include "macro.hpp"

#pragma warning(disable: 6001)
using namespace std;
using namespace console;

GarbageCollector gc;

GarbageCollector::~GarbageCollector()
{
	bool undeletedPointers = false;
	for_in_range(i, 0, m_allocatedMemorySlots.size())
		if (m_allocatedMemorySlots[i].ptr != nullptr)
		{
			undeletedPointers = true;
			break;
		}


	if (m_allocatedMemorySlots.size() != 0)
	{
		SetStdOutColor(Red);
		std::cerr << "Critical: " << m_allocatedMemorySlots.size() << " undeleted pointer" << (m_allocatedMemorySlots.size() ? "s" : "") << std::endl;

		for (auto& ptr : m_allocatedMemorySlots)
			if (ptr.ptr != nullptr)
			{
				std::cerr << ptr.instanciationFile << ':' << ptr.instanciationLine << std::endl;
				free(ptr.ptr);
			}

		SetStdOutColor(Green);
		cout << "Memory allocator has successfully freed all pointers" << endl;
		SetStdOutColor(White);
	}
	else
	{
		SetStdOutColor(Green);
		cout << "No memory leak was detected" << endl;
		SetStdOutColor(White);
	}
}


void* GarbageCollector::allocate(int size, const char* instanciationFile, int instanciationLine)
{
	void* ptr = malloc(size);
	m_allocatedMemorySlots.push_back({ ptr, instanciationFile, instanciationLine });
	return ptr;
}
void GarbageCollector::del(void* ptr)
{
	if (ptr == nullptr)
		return;

	bool hasBeenFreed = false;
	for (auto& ams : m_allocatedMemorySlots)
		if (ptr == ams.ptr)
		{
			if (!hasBeenFreed)
			{
				hasBeenFreed = true;
				free(ams.ptr);
				ams.ptr = nullptr;
			}
			else
				ams.ptr = nullptr;
		}
}

#pragma warning(default: 6001)

#endif