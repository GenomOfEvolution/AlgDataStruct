#pragma once
#include "FileExplorer.h"

#include<functional>
#include<map>
#include<iostream>
#include<windows.h> 
#include<conio.h>


class UserInterface
{
public:
	UserInterface(FileExplorer fileExplorer);
	bool HandleCommand(char buttonCode);

private:
	bool PrintHelp();
	bool CreateFolderElement();
	bool CreateFileElement();
	bool EditName();
	bool CopyElement();
	bool DeleteElement();
	bool MoveCursorUp();
	bool MoveCursorDown();
	bool EnterElement();
	bool ExitExplorer();
	bool PasteElement();
	bool ExtractElement();

	void SetColor(int textColor, int bgColor);
	void CalcBounds(Tree* elem);
	void DrawStorage();

private:
	FileExplorer storage;
	Tree* curElemParent = nullptr;
	Tree* curElem = nullptr;
	Tree* bufferElem = nullptr;

	using Handler = std::function<bool()>;
	using ActionMap = std::map<uint8_t, Handler>;
	const ActionMap actionMap;

	int cursorPos = 0;
	int boundTop = 0;
	int boundBottom = 0;

	const int fileColor = 15;
	const int folderColor = 1;
	const int selectColor = 2;
};
