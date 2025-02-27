#include "cli/cliCore.h"
#include "logger.h"


int main(int, char**)
{

	tk::cliCore::init();
	tk::cliCore::update();
	Sleep(3000);
	LOG_INF("tk::cliCore::log(\"Hello, world!\");");
	tk::cliCore::log("Hello, world!");
	LOG_DBG("tk::cliCore::update();");
	tk::cliCore::update();
	Sleep(3000);

	LOG_ERR("tk::cliCore::update();");
	tk::cliCore::update();
	Sleep(3000);
	LOG_WRN("log(big)");
	tk::cliCore::log("Hello!abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	tk::cliCore::update();
	Sleep(3000);
	return 0;
}
