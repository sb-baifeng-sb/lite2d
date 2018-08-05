#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

#if true
#define USE_WIN32_CONSOLE
#endif // COCOS2D_DEBUG

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	// create the application instance
	AppDelegate app;
	int ret = Application::getInstance()->run();

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return ret;
}