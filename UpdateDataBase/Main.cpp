#include <Windows.h>

#include "MainWindow.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	windows::MainWindow& ref = windows::MainWindow::get();

	SendMessageW(ref.getHWND(), WM_COMMAND, MAIN_WINDOW_POINTER_INIT, reinterpret_cast<LPARAM>(&ref));

	MSG msg = {};

	while (GetMessageW(&msg, nullptr, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}