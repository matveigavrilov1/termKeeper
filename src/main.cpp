#include "cli/cliCore.h"
#include "utils/logger.h"

int main(int, char**) {
    // Инициализация и обновление экрана
    tk::cliCore::init();
    tk::cliCore::update();
    Sleep(1000);

    // Вставляем символ 'H' в позицию (0, 0)
    tk::cliCore::insertSymb(0, 0, 'H');
    tk::cliCore::update();
    Sleep(1000);

    // Проверяем, пуст ли символ в позиции (0, 0)
    if (!tk::cliCore::isEmptySymb(0, 0)) {
        LOG_INF("Test 1 passed: Symbol at (0, 0) is not empty.");
    } else {
        LOG_INF("Test 1 failed: Symbol at (0, 0) is empty.");
    }
	Sleep(1000);
    // Вставляем длинную строку в позицию (3, 0)
    tk::cliCore::insertStr(3, 0, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    tk::cliCore::update();
    Sleep(1000);

    // Проверяем, пуста ли строка 0
    if (!tk::cliCore::isEmptyLine(0)) {
        LOG_INF("Test 2 passed: Line 0 is not empty.");
    } else {
        LOG_INF("Test 2 failed: Line 0 is empty.");
    }
	Sleep(1000);
    // Вставляем символ 'H' в правый нижний угол
    tk::cliCore::insertSymb(tk::cliCore::width() - 1, tk::cliCore::height() - 1, 'H');
    tk::cliCore::update();
    Sleep(1000);

    // Проверяем, пуст ли символ в правом нижнем углу
    if (!tk::cliCore::isEmptySymb(tk::cliCore::width() - 1, tk::cliCore::height() - 1)) {
        LOG_INF("Test 3 passed: Symbol at bottom-right corner is not empty.");
    } else {
        LOG_INF("Test 3 failed: Symbol at bottom-right corner is empty.");
    }
	Sleep(1000);
    // Вставляем длинную строку в последнюю строку
    tk::cliCore::insertStr(0, tk::cliCore::height() - 1, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    tk::cliCore::update();
    Sleep(1000);

    // Проверяем, пуста ли последняя строка
    if (!tk::cliCore::isEmptyLine(tk::cliCore::height() - 1)) {
        LOG_INF("Test 4 passed: Last line is not empty.");
    } else {
        LOG_INF("Test 4 failed: Last line is empty.");
    }

    // Очищаем первую строку
    tk::cliCore::clearLine(0);
    tk::cliCore::update();
    Sleep(1000);

    // Проверяем, пуста ли первая строка после очистки
    if (tk::cliCore::isEmptyLine(0)) {
        LOG_INF("Test 5 passed: Line 0 is empty after clearing.");
    } else {
        LOG_INF("Test 5 failed: Line 0 is not empty after clearing.");
    }
	Sleep(1000);
    // Очищаем весь экран
    tk::cliCore::clearScreen();
    tk::cliCore::update();
    Sleep(1000);
	Sleep(1000);
    // Проверяем, пуста ли первая строка после очистки экрана
    if (tk::cliCore::isEmptyLine(0)) {
        LOG_INF("Test 6 passed: Line 0 is empty after clearing the screen.");
    } else {
        LOG_INF("Test 6 failed: Line 0 is not empty after clearing the screen.");
    }
	Sleep(1000);
    return 0;
}