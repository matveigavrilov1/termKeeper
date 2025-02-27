#pragma once

#include <vector>
#include <string>

#include "model/interface.h"
#include "clipboardController/interface.h"

namespace tk
{

class cli
{
public:
	void show();

private:
	void handleInput();
	void updateSelectedItem(int delta);
	void switchList();
	void fillScreen(char* screen, int columns, int rows);

	int columns, rows;
	std::vector<std::string> list1 = { "Элемент 1", "Элемент 2", "Элемент 3" };
	std::vector<std::string> list2 = { "Элемент A", "Элемент B", "Элемент C" };
	int selectedItem = 0;
	bool isFirstListActive = true;
};
} // namespace tk