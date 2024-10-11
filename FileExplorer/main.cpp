// 26. Информация  о  файлах  на  жестких  дисках   компьютера
// записана  с  помощью  дерева.Обеспечить выполнение следующих
// операций :
// 1) загрузку дерева в память из файла;
// 2) обход дерева папок в  режиме  диалога(раскрытие папок,
// подъем на уровень и т.п.);
// 3) корректировку  дерева при создании новых папок и файлов,
// их переименовании, копировании, переносе и удалении.
// 4) сохранение дерева в файле(13).
// Автор: Нечаев Олег ПС-24
// Среда разработки: MS VisualStudio2022 Community Edition cpp ver 20


#include <iostream>
#include <conio.h>

#include "UserInterface.h"
#include "FileExplorer.h"
#include "ArgsParser.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RU");

    /*std::optional<Args> args = ParseArgs(argc, argv);
    if (!args)
    {
        return EXIT_FAILURE;
    }*/

    Args args;
    args.inputFileName = "Storage.txt";

    FileExplorer explorer(args);
    UserInterface UI(explorer);
    bool UIanswer = true;

    while (UIanswer)
    {
        if (_kbhit())
        {
            UIanswer = UI.HandleCommand(_getch());
        }
    }

    return EXIT_SUCCESS;
}