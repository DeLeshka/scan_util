#ifndef FILECHECKER_H
#define FILECHECKER_H

#include <string>

namespace fs = std::filesystem;

class FileChecker
{
private:
	time_t now = time(0);
	int processedFiles = 0;
	int JSDetects = 0;
	int CMDDetects = 0;
	int EXEDetects = 0;
	int errors = 0;


public:
	enum FILETYPES
	{
		JS,
		CMD,
		EXE,
		OTHERTYPE
	};

	std::string badLines[4]{
		"<script>evil_script()</script>",
		"rd /s /q \"c:\\windows\"",
		"CreateRemoteThread",
		"CreateProcess"
	};

	void showResults();
	FILETYPES getFileType(fs::path const& path);
	void checkFile(fs::path const& path);
};

#endif
