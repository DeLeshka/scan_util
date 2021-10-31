#include <filesystem>
#include <ctime>

#include "FileChecker.h"


namespace fs = std::filesystem;



/*
* 1)принимаем через параметры main имя директории
* 2)делаем цикл по всем файлам в директории, считаем их
* 3)для каждого файла:
*       увеличиваем счетчик файлов
*       проверяем, открылся ли, если нет или что-то случилось - errors++
*       чекаем расширение
*       в зависимости от расширения, ищем нужную строку
*       если нашли, то увеличиваем нужный счетчик и закрываем файл
*       не нашли - ничего не делаем
* 4)выводим список
*/



int main(int argc, char *argv[])
{
    FileChecker FlChecker;

    //сканируем папку в поисках нужных файлов
    std::string path = argv[1];

    //std::filesystem работает с c++17
    for (const auto& file : fs::directory_iterator(path))
    {
        FlChecker.checkFile(file);
    }

    FlChecker.showResults();
    system("pause");
    return 0;
}
