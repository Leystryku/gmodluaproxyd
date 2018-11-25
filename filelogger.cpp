#include "filelogger.h"

void FileLogger::Reset()
{
	started = false;
}

int32_t FileLogger::Start(const char* savedir)
{

	if (!savedir || strlen(savedir) > 220)
	{
		return 0;
	}
	started = false;

	strncpy(directory, savedir, 200);

	return 1;
}

extern void filelogger_recursivecreatedir(const char*dir);

int32_t FileLogger::LogFile(const char* name, const char* content, size_t len)
{
	char formatteddirectory[256] = { 0 };
	strcpy(formatteddirectory, directory);

	char coolname[256] = { 0 };



	if (name[0] != '@')
	{
		strncpy(coolname, name, 256);
	}else{
		if (name[1] != 0)
		{
			strncpy(coolname, name+1, 256);
		}
	}

	for (size_t i = 0; i < strlen(coolname); i++)
	{

		if (coolname[i] == '\\') // no backslashes cuz uniform
		{
			coolname[i] = '/';
		}

		if (coolname[i] == '.' && coolname[i - 1] == '.') // no dots cuz dir traversal
		{
			coolname[i] = 'D';
		}

		if (coolname[i] == '<' || coolname[i] == '>' || coolname[i] == ':' || coolname[i] == '"' || coolname[i] == '|' || coolname[i] == '?' || coolname[i] == '*') // no invalid chars
		{
			coolname[i] = 'I';
		}

		if (32 > coolname[i]) // no control chars
		{
			coolname[i] = 'C';
		}

	}

	snprintf(formatteddirectory, sizeof(formatteddirectory), "%s/%s", directory, coolname);
	
	int32_t last = 0;

	for (size_t i = 0; i < strlen(formatteddirectory); i++)
	{
		if (formatteddirectory[i] == '/')
		{
			last = i;
		}
	}

	std::string removefilename = formatteddirectory;

	filelogger_recursivecreatedir(removefilename.substr(0, last).c_str());

	FILE*written = fopen(formatteddirectory, "wb");


	if (!written)
	{
		for (int32_t i = 0; i < 0xFFFF; i++) // up to 65535 conflict resolvingz
		{
			char resolveconflict[0xFF] = { 0 };
			snprintf(resolveconflict, 0xFF, "%s%d", formatteddirectory, i);
			written = fopen(resolveconflict, "wb");

			if (written)
				break;

		}

		if (!written)
		{
			printf("FileLogger ERROR: couldn't resolve file conflict for: %s\n", name);
			return 0;
		}
	}

	fwrite(content, sizeof(char), len, written);
	fclose(written);

	return 1;

}