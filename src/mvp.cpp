#include <vector>
#include <string>
#include <iostream>
#include <conio.h> // Для получения нажатий клавиш
#include "cli/cliCore.h"

using namespace tk;

class CLIListManager
{
private:
	std::vector<std::vector<std::string>> lists;
	size_t currentListIndex = 0;
	size_t currentItemIndex = 0;

public:
	CLIListManager(const std::vector<std::vector<std::string>>& lists)
	: lists(lists)
	{ }

	void display()
	{
		tk::cliCore::clearScreen();

		// Отображение текущего списка
		const auto& currentList = lists[currentListIndex];
		for (size_t i = 0; i < currentList.size(); ++i)
		{
			tk::cliCore::insertStr(0, i, currentList[i]);
			if (i == currentItemIndex)
			{
				tk::cliCore::highlightLine(i);
			}
			else
			{
				tk::cliCore::resetHighlightLine(i);
			}
		}

		// Отображение подсказок для пользователя
		std::string hints = "Up/Down: Select, Left/Right: Switch lists, Q: Quit";
		tk::cliCore::insertStr(0, tk::cliCore::height() - 1, hints);

		tk::cliCore::update();
	}

	void handleInput()
	{
		if (kbhit())
		{
			char input = _getch();
			switch (input)
			{
				case 72: // Стрелка вверх
					if (currentItemIndex > 0)
					{
						--currentItemIndex;
					}
					break;
				case 80: // Стрелка вниз
					if (currentItemIndex < lists[currentListIndex].size() - 1)
					{
						++currentItemIndex;
					}
					break;
				case 75: // Стрелка влево
					if (currentListIndex > 0)
					{
						--currentListIndex;
						currentItemIndex = 0;
					}
					break;
				case 77: // Стрелка вправо
					if (currentListIndex < lists.size() - 1)
					{
						++currentListIndex;
						currentItemIndex = 0;
					}
					break;
				case 'q': // Выход
					exit(0);
					break;
			}
		}
		display();
	}

	void run()
	{
		tk::cliCore::init();
		display();
		while (true)
		{
			handleInput();
		}
	}
};

int main()
{

	std::vector<std::vector<std::string>> lists = { { "Item 1.1", "Item 1.2", "Item 1.3" }, { "Item 2.1", "Item 2.2" },
		{ "Item 3.1", "Item 3.2", "Item 3.3", "Item 3.4" } };

	CLIListManager manager(lists);
	manager.run();

	return 0;
}
