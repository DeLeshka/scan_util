#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <string>
#include <regex>
#include "fileChecker.h"

namespace fs = std::filesystem;

//вывод результатов
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
        std::cout << std::setfill('0') << std::setw(2) << "Execution time: "  << time / 3600000 << ':' <<
            std::setfill('0') << std::setw(2) << time / 60000 << ':' << std::setfill('0') <<
            std::setw(2) << ceil(static_cast<double>(time) / 1000) <<
        "\n\n" <<
        "========================\n";

}

//метод принимает путь и возвращает перечислитель, обозначающий работу с конкретным типом файла
 FileChecker::FILETYPES FileChecker::getFileType(fs::path const& path)
{
    std::string extension = path.extension().string();

    //понизим регистр расширени€ на вс€кий случай
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




 /*
 * принимаем путь до файла
 * увеличиваем счетчик файлов
 * в зависимости от расширени€ файла, ищем регул€ркой нужную подстроку
 * если нашли - закрываем файл, увеличиваем счетчик и выходим из цикла
 * в случае exe/dll делаем это только когда нашли обе "плохие" строки
 */

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
            std::getline(in, currentLine);
            std::regex badLine(".*(<script>evil_script\\(\\)<\\/script>).*");
            if (std::regex_match(currentLine, badLine))
            {
                ++JSDetects;
                in.close();
                break;
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
            std::getline(in, currentLine);
            std::regex badLine(R".*(rd \/s \/q \"c:\\windows\").*");
            if (std::regex_match(currentLine, badLine))
            {
                ++CMDDetects;
                in.close();
                break;
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
            std::getline(in, currentLine);
            std::regex badLine1(".*CreateRemoteThread.*");
            std::regex badLine2(".*CreateProcess.*");
            if (std::regex_match(currentLine, badLine1))
            {
                if (hasOneBadString)
                {
                    ++EXEDetects;
                    in.close();
                    break;
                }
                hasOneBadString = true;
            }
            if (std::regex_match(currentLine, badLine2))
            {
                if (hasOneBadString)
                {
                    ++EXEDetects;
                    in.close();
                    break;
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

