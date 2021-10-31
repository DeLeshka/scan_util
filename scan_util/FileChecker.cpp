#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <string>
#include "fileChecker.h"

namespace fs = std::filesystem;

void FileChecker::showResults()
{
    std::cout << "\n\n====== Scan result ======\n\n" <<
        "Processed files: " << processedFiles <<
        "\n\n" <<
        "JS detects: " << JSDetects <<
        "\n\n" <<
        "CMD detects: " << CMDDetects <<
        "\n\n" <<
        "EXE detects: " << EXEDetects <<
        "\n\n" <<
        "Errors: " << errors <<
        "\n\n";
        int time = clock();
        std::cout << std::setfill('0') << std::setw(2) << "Execution time: "  << time / 3600000 << ':' << std::setfill('0') << std::setw(2) << time / 60000 << ':' << std::setfill('0') << std::setw(2) << ceil(static_cast<double>(time) / 1000) <<
        "\n\n" <<
        "========================\n";

}

//метод принимает путь и возвращает перечислитель, обозначающий работу с конкретным типом файла
 FileChecker::FILETYPES FileChecker::getFileType(fs::path const& path)
{
    std::string extension = path.extension().string();
    //понизим строку расширения на всякий случай
    std::transform(extension.begin(), extension.end(), extension.begin(),
        [](unsigned char c) { return std::tolower(c); });

    if (extension == ".js")
        return JS;

    else if (extension == ".cmd" || extension == ".bat")
        return CMD;

    else if (extension == ".exe" || extension == ".dll")
        return EXE;

    return OTHERTYPE;
}



void FileChecker::checkFile(fs::path const& path)
{
    ++processedFiles;
    switch (this->getFileType(path))
    {
    case JS:
    {
        std::ifstream in(path);
        if (!in)
            errors++;
        while (in)
        {
            std::string strInput;
            std::getline(in, strInput);
            if (strInput == "<script>evil_script()</script>")
            {
                ++JSDetects;
                in.close();
            }
        }
        break;
    }
    case CMD:
    {
        std::ifstream in(path);
        if (!in)
            errors++;
        while (in)
        {
            std::string strInput;
            std::getline(in, strInput);
            if (strInput == "rd /s /q \"c:\\windows\"")
            {
                ++CMDDetects;
                in.close();
            }
        }
        break;
    }
    case EXE:
    {
        std::ifstream in(path);
        bool hasOneBadString = false;
        if (!in)
            errors++;
        while (in)
        {
            std::string strInput;
            std::getline(in, strInput);
            if (strInput == "CreateRemoteThread")
            {
                if (hasOneBadString)
                {
                    ++EXEDetects;
                    in.close();
                }
                hasOneBadString = true;
            }
            if (strInput == "CreateProcess")
            {
                if (hasOneBadString)
                {
                    ++EXEDetects;
                    in.close();
                }
                hasOneBadString = true;
            }
        }
        break;
    }

    case OTHERTYPE:
        break;

    default:
        errors++;
        break;

    }
}

