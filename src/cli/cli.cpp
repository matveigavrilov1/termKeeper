#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>

#include "cli/cli.h"

using namespace tk;

void overwriteLine(char * text, size_t length) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Получаем текущую информацию о консоли
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        std::cerr << "Ошибка GetConsoleScreenBufferInfo: " << GetLastError() << std::endl;
        return;
    }

    // Запоминаем текущую позицию курсора
    COORD originalCursorPosition = csbi.dwCursorPosition;

    // Перемещаем курсор в начало текущей строки
    COORD newCursorPosition = { 0, csbi.dwCursorPosition.Y };
    if (!SetConsoleCursorPosition(hConsole, newCursorPosition)) {
        std::cerr << "Ошибка SetConsoleCursorPosition: " << GetLastError() << std::endl;
        return;
    }

    // Преобразуем std::string в const char* для WriteConsoleA

    // Выводим текст
    DWORD charsWritten;
    if (!WriteConsoleA(hConsole, text, length, &charsWritten, NULL)) {
        std::cerr << "Ошибка WriteConsoleA: " << GetLastError() << std::endl;
        return;
    }

    // Заполняем остаток строки пробелами, чтобы очистить старый текст
    if (length < csbi.dwSize.X) {
        DWORD spacesToWrite = csbi.dwSize.X - length;
        std::string spaces(spacesToWrite, ' ');
        const char* spaces_cstr = spaces.c_str();  // Преобразуем в const char* и пробелы
        if (!WriteConsoleA(hConsole, spaces_cstr, spacesToWrite, &charsWritten, NULL)) {
            std::cerr << "Ошибка WriteConsoleA (очистка): " << GetLastError() << std::endl;
            return;
        }
    }

    // Возвращаем курсор на прежнее место (если нужно)
    //if (!SetConsoleCursorPosition(hConsole, originalCursorPosition)) {
    //    std::cerr << "Ошибка SetConsoleCursorPosition (восстановление): " << GetLastError() << std::endl;
    //    return;
    //}

}

void cli::show()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// Получаем хэндл консоли
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (GetConsoleScreenBufferInfo(hStdout, &csbi))
	{
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		std::cout << "Размер окна консоли: " << columns << "x" << rows << std::endl;
	}
	else
	{
		std::cerr << "Ошибка при получении информации о консоли." << std::endl;
		return;
	}

	char* screen = new char[columns * rows];
	fillScreen(screen, columns, rows);

	while (true)
	{
		handleInput();
		fillScreen(screen, columns, rows);			 // Обновляем экран
		overwriteLine(screen, columns * rows); // Выводим весь экран сразу
	}

	delete[] screen;
}

void cli::fillScreen(char* screen, int columns, int rows)
{
	// Заполняем экран пробелами
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			screen[i * columns + j] = ' ';
		}
	}

	// Рисуем текущий список
	std::vector<std::string>& currentList = isFirstListActive ? list1 : list2;
	int startY = (rows - currentList.size()) / 2; // Центрируем список по вертикали

	for (int i = 0; i < currentList.size(); ++i)
	{
		int startX = (columns - currentList[i].length()) / 2; // Центрируем текст по горизонтали

		if (i == selectedItem)
		{
			// Выделяем выбранный элемент (например, добавляем символ '>')
			std::string selectedText = "> " + currentList[i];
			for (int j = 0; j < selectedText.length(); ++j)
			{
				screen[(startY + i) * columns + startX + j] = selectedText[j];
			}
		}
		else
		{
			// Обычный элемент
			for (int j = 0; j < currentList[i].length(); ++j)
			{
				screen[(startY + i) * columns + startX + j] = currentList[i][j];
			}
		}
	}
}

void cli::handleInput()
{
	if (_kbhit())
	{
		char ch = _getch();
		if (ch == 0 || ch == 0xE0) // Проверка на специальные клавиши
		{
			ch = _getch();
			switch (ch)
			{
				case 72: // Стрелка вверх
					updateSelectedItem(-1);
					break;
				case 80: // Стрелка вниз
					updateSelectedItem(1);
					break;
				case 75: // Стрелка влево
				case 77: // Стрелка вправо
					switchList();
					break;
			}
		}
	}
}

void cli::updateSelectedItem(int delta)
{
	std::vector<std::string>& currentList = isFirstListActive ? list1 : list2;
	selectedItem += delta;
	if (selectedItem < 0)
		selectedItem = currentList.size() - 1;
	else if (selectedItem >= currentList.size())
		selectedItem = 0;
}

void cli::switchList()
{
	isFirstListActive = !isFirstListActive;
	selectedItem = 0; // Сброс выбора при переключении списка
}
