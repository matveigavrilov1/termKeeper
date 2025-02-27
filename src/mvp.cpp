#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Скрываем курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void overwriteLine(const std::string& text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Получаем текущую информацию о консоли
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        std::cerr << "Ошибка GetConsoleScreenBufferInfo: " << GetLastError() << std::endl;
        return;
    }

    // Перемещаем курсор в начало текущей строки
    COORD newCursorPosition = { 0, csbi.dwCursorPosition.Y };
    if (!SetConsoleCursorPosition(hConsole, newCursorPosition)) {
        std::cerr << "Ошибка SetConsoleCursorPosition: " << GetLastError() << std::endl;
        return;
    }

    // Обрезаем или дополняем строку до ширины консоли
    std::string adjustedText = text;
    if (adjustedText.length() > csbi.dwSize.X) {
        adjustedText = adjustedText.substr(0, csbi.dwSize.X);
    } else if (adjustedText.length() < csbi.dwSize.X) {
        adjustedText.append(csbi.dwSize.X - adjustedText.length(), ' ');
    }

    // Выводим текст
    DWORD charsWritten;
    if (!WriteConsoleA(hConsole, adjustedText.c_str(), adjustedText.length(), &charsWritten, NULL)) {
        std::cerr << "Ошибка WriteConsoleA: " << GetLastError() << std::endl;
        return;
    }
}

int main() {
    hideCursor(); // Скрываем курсор

    overwriteLine("This is some initial text!");
    Sleep(1000);
    overwriteLine("This text will overwrite the previous line!");
    Sleep(1000);
    overwriteLine("A shorter line."); // Очищает остаток строки пробелами
    Sleep(1000);

    // После завершения работы overwriteLine, последующий вывод продолжится с новой строки
    std::cout << "\nThis is a new line after overwriting.";

    return 0;
}