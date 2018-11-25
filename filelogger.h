#pragma once
#include <stdint.h>
#include <string>

class FileLogger
{
public:
	void Reset();
	int32_t Start(const char *savedir);
	int32_t LogFile(const char*name, const char*content, size_t len);
private:
	bool started;
	char directory[256];

};