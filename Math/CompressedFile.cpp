#include "CompressedFile.h"

CompressedFile::CompressedFile(const char* filename, std::ios_base::openmode mode) :
	m_stream(filename, mode)
{
}
CompressedFile::~CompressedFile()
{
	m_stream.close();
}

void CompressedFile::open(const char* filename, std::ios_base::openmode mode)
{
	m_stream.open(filename, mode);
}

bool CompressedFile::isOpen() const
{
	return m_stream.is_open();
}
