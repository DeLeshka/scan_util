#ifndef FILECHECKER_H
#define FILECHECKER_H


namespace fs = std::filesystem;

class FileChecker
{
private:
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



	void showResults();
	FILETYPES getFileType(fs::path const& path);
	void checkFile(fs::path const& path);
};

#endif
