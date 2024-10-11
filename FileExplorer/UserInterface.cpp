#include "UserInterface.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_F1 59
#define KEY_F2 60
#define KEY_F3 61
#define KEY_F4 62
#define KEY_F5 63
#define KEY_F6 64
#define KEY_F7 65
#define KEY_F8 66


UserInterface::UserInterface(FileExplorer fileExplorer) : 
	storage(fileExplorer),
	actionMap({ 
		{ KEY_UP, std::bind_front(&UserInterface::MoveCursorUp, this) },
		{ KEY_DOWN, std::bind_front(&UserInterface::MoveCursorDown, this) },
		{ KEY_ENTER, std::bind_front(&UserInterface::EnterElement, this) },
		{ KEY_ESC, std::bind_front(&UserInterface::ExitExplorer, this) },

		{ KEY_F1, std::bind_front(&UserInterface::PrintHelp, this) },
		{ KEY_F2, std::bind_front(&UserInterface::CreateFolderElement, this) },
		{ KEY_F3, std::bind_front(&UserInterface::CreateFileElement, this) },
		{ KEY_F4, std::bind_front(&UserInterface::EditName, this) },
		{ KEY_F5, std::bind_front(&UserInterface::CopyElement, this) },
		{ KEY_F6, std::bind_front(&UserInterface::ExtractElement, this)},
		{ KEY_F7, std::bind_front(&UserInterface::PasteElement, this) },
		{ KEY_F8, std::bind_front(&UserInterface::DeleteElement, this) },
	})
{
	curElemParent = storage.ROOT;
	if (curElemParent->children.empty())
	{
		curElem = nullptr;
	}
	else
	{
		curElem = curElemParent->children[0];
		cursorPos = 0;
	}

	CalcBounds(curElemParent);
	DrawStorage();
}

bool UserInterface::HandleCommand(char buttonCode)
{
	auto it = actionMap.find(uint8_t(buttonCode));
	if (it != actionMap.end())
	{
		return it->second();
	}

	return true;
}

bool IsCorrectFolderName(const std::string& str)
{
	const std::string forbiddenChars = "\\/:*?\"<>|";

	for (char c : str)
	{
		if (forbiddenChars.find(c) != std::string::npos)
		{
			return false;
		}
	}

	std::string trimmedStr;
	std::remove_copy(str.begin(), str.end(), std::back_inserter(trimmedStr), ' ');
	for (char c : trimmedStr)
	{
		if (c != '.')
		{
			return true;
		}
	}
	return false;
}

void UserInterface::SetColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void UserInterface::CalcBounds(Tree* elem)
{
	boundTop = (elem->parent != nullptr || elem->parent == storage.ROOT) ? -1 : 0;
	boundBottom = (elem->children.empty()) ? -1 : (int)elem->children.size();
}

std::string NameInput()
{
	system("cls");
	std::string newName = "";
	std::cout << "Введите новое имя: ";
	std::cin >> newName;
	while (!IsCorrectFolderName(newName))
	{
		std::cout << "\nИмя файла не должно содержать символы \\/:*?\"<>|";
		std::cout << "\nВведите новое имя: ";
		std::cin >> newName;
		system("cls");
	}

	return newName;
}

void UserInterface::DrawStorage()
{
	system("cls");
	(cursorPos == -1) ? SetColor(selectColor, 0) : SetColor(fileColor, 0);
	if (curElemParent != nullptr)
	{
		if (curElemParent != storage.ROOT)
		{
			std::cout << "..\n";
		}
	}
	else
	{
		return;
	}
	
	int index = 0;
	for (auto it : curElemParent->children)
	{
		if (index == cursorPos)
		{
			SetColor(selectColor, 0);
		}
		else
		{
			(!it->isFile) ? SetColor(fileColor, 0) : SetColor(folderColor, 0);
		}
		index++;
		std::cout << it->name << "\n";
	}
	
	SetColor(fileColor, 0);
	std::string filePath;
	Tree* elem = curElemParent;
	while (elem->parent != nullptr)
	{
		if (elem->parent->parent == nullptr)
		{
			filePath = elem->name + ":\\" + filePath;
		}
		else
		{
			filePath = elem->name + "\\" + filePath;
		}
		
		elem = elem->parent;
	}
	std::cout << "\n" << filePath << "\n";
	storage.ShowBuffer();
}

bool UserInterface::PrintHelp()
{
	system("cls");
	SetColor(15, 0);
	std::cout << "\n F1 - help"
	    << "\n F2 - new folder"
	    << "\n F3 - new file"
	    << "\n F4 - edit selected folder/file name"
	    << "\n F5 - copy folder/file into buffer"
		<< "\n F6 - cut folder/file in buffer"
	    << "\n F7 - paste folder/file in current folder"
	    << "\n F8 - delete selected filde/folder"
	    << "\n Esc - exit and save to file"
	    << "\n arrow up/down to move"
	    << "\n Press any key to continue...";
	char ch = _getch();
	
	DrawStorage();

	return true;
}

bool UserInterface::CreateFolderElement()
{
	std::string name = NameInput();
	storage.AddNewElem(false, name, curElemParent);
	CalcBounds(curElemParent);
	DrawStorage();
	return true;
}

bool UserInterface::CreateFileElement()
{
	std::string name = NameInput();
	storage.AddNewElem(true, name, curElemParent);
	CalcBounds(curElemParent);
	DrawStorage();
	return true;
}

bool UserInterface::EditName()
{
	if (cursorPos == -1)
	{
		return true;
	}

	storage.ChangeName(curElem, NameInput());
	DrawStorage();
	return true;
}

bool UserInterface::CopyElement()
{
	storage.CopyElem(curElem);
	DrawStorage();

	return true;
}

bool UserInterface::DeleteElement()
{
	if (curElem == nullptr || cursorPos == -1 || curElemParent->children.empty())
	{
		return true;
	}

	system("cls");
	std::cout << "Вы точно хотите удалить " + curElem->name << "? (Y/N)";
	char answer = _getch();

	if ((answer == 'Y') || (answer == 'y'))
	{
		storage.DeleteTree(curElem);
		CalcBounds(curElemParent);

		if (cursorPos == boundBottom)
		{
			cursorPos--;
			if (cursorPos != -1)
			{
				curElem = curElemParent->children[cursorPos];
			}
			else
			{
				curElem = nullptr;
			}
		}
		else
		{
			if (curElemParent != nullptr)
			{
				curElem = curElemParent->children[curElemParent->children.size() - 1];
			}
			else
			{
				curElem = nullptr;
			}
		}
	}
	DrawStorage();

	return true;
}

bool UserInterface::MoveCursorUp()
{
	if ((cursorPos - 1) >= boundTop)
	{
		cursorPos--;
		if (cursorPos == -1)
		{
			curElem = nullptr;
		}
		else
		{
			if (curElemParent != nullptr)
			{
				curElem = curElemParent->children[cursorPos];
			}
		}
		DrawStorage();
	}

	return true;
}

bool UserInterface::MoveCursorDown()
{
	if ((cursorPos + 1) < boundBottom)
	{
		cursorPos++;
		if (curElemParent != nullptr)
		{
			curElem = curElemParent->children[cursorPos];
		}
		DrawStorage();
	}
	
	return true;
}

bool UserInterface::EnterElement()
{
	//Поднимаемся вверх по файлам
	if (cursorPos == -1)
	{
		curElem = curElemParent;
		curElemParent = curElemParent->parent;
		if (curElemParent != nullptr)
		{
			auto it = std::find(curElemParent->children.begin(), curElemParent->children.end(), curElem);
			if (it != curElemParent->children.end())
			{
				cursorPos = (int)std::distance(curElemParent->children.begin(), it);
			}
		}
		CalcBounds(curElemParent);
		DrawStorage();
	}
	else //Заходим глубже
	{
		if (curElem == nullptr)
		{
			return true;
		}

		if (!curElem->isFile)
		{
			curElemParent = curElem;
			curElem = nullptr;
			cursorPos = -1;

			CalcBounds(curElemParent);
			DrawStorage();
		}
	}

	return true;
}

bool UserInterface::ExitExplorer()
{
	system("cls");

	std::cout << "Введите имя для файла сохранения: ";
	std::string fileName;
	std::cin >> fileName;
	std::ofstream output(fileName);
	if (!output)
	{
		std::cout << "\nНевозможно открыть файл!\n";
		return false;
	}
	storage.SaveTree(output);
	std::cout << "Saving the storage...\n";

	return false;
}

bool UserInterface::PasteElement()
{
	storage.PasteNode(curElemParent);
	CalcBounds(curElemParent);
	DrawStorage();

	return true;
}

bool UserInterface::ExtractElement()
{
	if (curElem == nullptr)
	{
		return true;
	}

	storage.ExtractElem(curElem);

	if (curElemParent != nullptr)
	{
		if (!curElemParent->children.empty())
		{
			curElem = curElemParent->children[cursorPos];
		}
		else
		{
			curElem = nullptr;
		}
	}

	CalcBounds(curElemParent);
	DrawStorage();

	return true;
}
