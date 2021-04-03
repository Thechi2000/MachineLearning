#pragma once
#include <fstream>
#include "macro.hpp"

class CompressedFile
{
public:
	CompressedFile(const char* filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
	~CompressedFile();

	void open(const char* filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
	bool isOpen() const;

	template<class T>
	CompressedFile& operator<<(const T& t)
	{
		const char* chars = (const char*)(void*)&t;
		for_in_range(i, 0, sizeof(T))
			m_stream << chars[i];

		return *this;
	}
	template<class T>
	T& operator>>(T& t)
	{
		char* chars = (char*)(void*)&t;
		for_in_range(i, 0, sizeof(T))
			m_stream >> chars[i];

		return t;
	}

private:
	std::fstream m_stream;
};

