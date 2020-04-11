#include "MainWindow.h"

#include <string>
#include <array>
#include <vector>
#include <set>
#include <fstream>

#include "UsersData.h"
#include "resource.h"

namespace windows
{
	std::array<wchar_t, 256> fileNameBuffer{};
	const std::set<char> delimiters({ {';'}, {':'} });	//Разделители

	void chooseFile(HWND parent);

	std::vector<std::string> parse();

	LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	std::filesystem::path MainWindow::getCurrentDirectory() const
	{
		return currentDirectory;
	}

	MainWindow::MainWindow(): currentDirectory(std::filesystem::current_path())
	{
		WNDCLASSEXW wndClass = {};

		wndClass.cbSize = sizeof(WNDCLASSEXW);
		wndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wndClass.hIcon = static_cast<HICON>(LoadImageW(GetModuleHandleW(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 170, 170, LR_DEFAULTCOLOR));
		wndClass.lpfnWndProc = MainWindowProcedure;
		wndClass.lpszClassName = L"MainWindow";

		RegisterClassExW(&wndClass);

		RECT sizes;

		GetClientRect(GetDesktopWindow(), &sizes);

		long desktopWidth = sizes.right - sizes.left;
		long desktopHeight = sizes.bottom - sizes.top;

		handle = CreateWindowExW
		(
			NULL,
			wndClass.lpszClassName,
			L"Обновление базы по оплате",	//Название окна
			WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE,	//^ WS_THICKFRAME Запрещает окну менять размер
			(desktopWidth - mainWindowWidth) * 0.5, (desktopHeight - mainWindowHeight) * 0.5,
			mainWindowWidth * 0.7, mainWindowHeight * 0.7,
			nullptr,
			nullptr,
			nullptr,
			nullptr
			);

		GetClientRect(handle, &sizes);

		long clientAreaWidth = sizes.right - sizes.left;
		long clientAreaHeight = sizes.bottom - sizes.top;

		warning = CreateWindowExW
		(
			NULL,
			L"STATIC",
			L"Внимание!\nПрограмма выполняет удаление старых данных и добавление новых!",
			WS_CHILDWINDOW | WS_VISIBLE | SS_CENTER,	
			22, 100,
			warningWidth, warningHeight,
			handle,
			nullptr,
			nullptr,
			nullptr
		);

		chooseFileButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Выбрать файл",
			WS_CHILDWINDOW | WS_VISIBLE,
			(clientAreaWidth - chooseButtonWidth) * 0.5, (clientAreaHeight - chooseButtonHeight) * 0.5,
			chooseButtonWidth, chooseButtonHeight,
			handle,
			HMENU(chooseButton),
			nullptr,
			nullptr
			);

		sendFileButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Отправить файл",
			WS_CHILDWINDOW | WS_VISIBLE,
			(clientAreaWidth - chooseButtonWidth) * 0.5, (clientAreaHeight - chooseButtonHeight) * 0.5 + chooseButtonHeight * 2,
			sendButtonWidth, sendButtonHeight,
			handle,
			HMENU(sendButton),
			nullptr,
			nullptr
			);

		currentFile = CreateWindowExW
		(
			NULL,
			L"STATIC",
			nullptr,
			WS_CHILDWINDOW | WS_VISIBLE | SS_CENTER,
			22.5, (clientAreaHeight - chooseButtonHeight) * 0.5 + currentFileHeight * 0.5 + chooseButtonHeight,
			currentFileWidth, currentFileHeight,
			handle,
			nullptr,
			nullptr,
			nullptr
			);
	}

	MainWindow& MainWindow::get()
	{
		static MainWindow mainWindow;
		return mainWindow;
	}

	HWND MainWindow::getHWND()
	{
		return handle;
	}

	MainWindow::~MainWindow()
	{
		DestroyWindow(chooseFileButton);
		DestroyWindow(sendFileButton);
		DestroyWindow(currentFile);
		DestroyWindow(handle);
	}

	LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		static MainWindow* ptr = nullptr;

		switch (msg)
		{
		case WM_COMMAND:
			switch (wparam)
			{
			case MAIN_WINDOW_POINTER_INIT:
				ptr = reinterpret_cast<MainWindow*>(lparam);

				break;

			case chooseButton:
				chooseFile(ptr->getHWND());

				break;

			case sendButton:

			{
				static wchar_t fileNameCheck[128];

				if (!GetWindowTextW(FindWindowExW(ptr->getHWND(), nullptr, L"STATIC", fileNameBuffer.data()), fileNameCheck, 128))
				{
					break;
				}

				std::vector<std::string> res = parse();
				std::vector<data::UsersData<char>> information;
				information.reserve(res.size() / tableColumns);

				for (size_t i = 0; i < res.size(); i += tableColumns)
				{
					information.emplace_back(std::vector<std::string>(std::begin(res) + i, std::begin(res) + i + tableColumns));
				}

				std::string command;

				for (const auto& i : information)
				{
					const std::vector<std::string>& ref = i.get();

					for (const auto& j : ref)
					{
						std::string tem;

						for (const auto& k : j)
						{
							if (k == L' ')
							{
								tem += L'_';
							}
							else
							{
								tem += k;
							}
						}

						command += tem + " ";
					}
				}
				command.pop_back();

				std::filesystem::path temFile = ptr->getCurrentDirectory();
				temFile.append("tem.txt");

				std::ofstream out(temFile);
				out.write(command.data(), command.size());
				out.close();

				DWORD attributes = GetFileAttributesW(temFile.generic_wstring().data());
				SetFileAttributesW(temFile.generic_wstring().data(), attributes + FILE_ATTRIBUTE_HIDDEN);

				std::string relativePath = ptr->getCurrentDirectory().generic_string();
				relativePath += "/";
				relativePath += applicationName;

				system(std::string(relativePath + temFile.generic_string()).data());
			}

			break;
			}

			return 0;

		case WM_DESTROY:	//Вызывается при уничтожении окна
			PostQuitMessage(0);

			return 0;

		default:
			return DefWindowProcW(hwnd, msg, wparam, lparam);
		}
	}

	std::vector<std::string> parse()
	{
		std::vector<std::string> res;

		std::ifstream file(fileNameBuffer.data());
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::string line;
				std::getline(file, line);

				if (line == "")
				{
					break;
				}

				std::string tem;

				size_t commaCount = 0;

				for (size_t i = 0; i < line.size(); i++)
				{
					if (/* line[i] == ',' && line[i + 1] != ' ' ЕСЛИ ИСПРАВЯТ */ commaCount < 3 && line[i] == ',' || delimiters.find(line[i]) != end(delimiters))
					{
						if (tem == "")
						{
							res.push_back("empty");
						}
						else
						{
							res.push_back(tem);
							tem = "";
						}

						if (line[i] == ',')
						{
							commaCount++;
						}
					}
					else
					{
						tem += line[i];
					}
				}

				res.push_back(tem);
			}
			file.close();
		}

		return res;
	}

	void chooseFile(HWND parent)
	{
		OPENFILENAMEW file = {};

		std::wstring temp = fileNameBuffer.data();

		file.lStructSize = sizeof(OPENFILENAMEW);
		file.hwndOwner = parent;
		file.lpstrFile = fileNameBuffer.data();
		file.lpstrFile[0] = L'\0';
		file.nMaxFile = sizeof(fileNameBuffer);
		file.lpstrFilter = L"Текстовые документы\0*.txt\0\0";
		file.nFilterIndex = 1;
		file.lpstrFileTitle = nullptr;
		file.nMaxFileTitle = 0;
		file.lpstrInitialDir = nullptr;
		file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		GetOpenFileNameW(&file);

		std::wstring fileName = file.lpstrFile;

		SetWindowTextW(FindWindowExW(parent, nullptr, L"STATIC", temp.data()), fileName.data());
	}
}