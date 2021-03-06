#include "MainWindow.h"

#include <string>
#include <array>
#include <vector>
#include <set>
#include <unordered_map>
#include <fstream>
#include <CommCtrl.h>

#include "DataConversion.h"
#include "UsersData.h"
#include "../res/resource.h"

using namespace std;

namespace windows
{
	array<wchar_t, 256> fileNameBuffer{};
	const set<char> delimiters({ {';'}, {':'} });	//�����������
	unordered_map<wstring, string> payType
	(
		{
			make_pair(L"��������","0"),
			make_pair(L"����","1")
		}
	);

	constexpr array<wstring_view, 2> payTypesList =
	{
		L"��������",
		L"����"
	};

	void chooseFile(HWND parent);

	vector<string> parse();

	LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	string changeStrings(string&& str);

	filesystem::path MainWindow::getCurrentDirectory() const
	{
		return currentDirectory;
	}

	MainWindow::MainWindow() : currentDirectory(filesystem::current_path())
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
			L"���������� ���� �� ������",	//�������� ����
			WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE,	//^ WS_THICKFRAME ��������� ���� ������ ������
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

		list = CreateWindowExW
		(
			NULL,
			WC_COMBOBOXW,
			L"",
			CBS_DROPDOWNLIST | WS_CHILDWINDOW | WS_VISIBLE,
			(clientAreaWidth - listWidth) * 0.5, standardOffsetY,
			listWidth, listHeight,
			handle,
			nullptr,
			nullptr,
			nullptr
		);

		for (const auto& i : payTypesList)
		{
			SendMessageW(list, CB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(i.data()));
		}

		SendMessageW(list, CB_SETCURSEL, 0, NULL);

		warning = CreateWindowExW
		(
			NULL,
			L"STATIC",
			L"��������!\n��������� ��������� �������� ������ ������ � ���������� �����!",
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
			L"������� ����",
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
			L"��������� ����",
			WS_CHILDWINDOW | WS_VISIBLE,
			(clientAreaWidth - chooseButtonWidth) * 0.5, (clientAreaHeight - chooseButtonHeight) * 0.5 + chooseButtonHeight * 2,
			sendButtonWidth, sendButtonHeight,
			handle,
			HMENU(sendButton),
			nullptr,
			nullptr
		);

		deleteDBButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"�������� ���� ������",
			WS_CHILDWINDOW | WS_VISIBLE,
			(clientAreaWidth - chooseButtonWidth) * 0.5, (clientAreaHeight - chooseButtonHeight) * 0.5 + chooseButtonHeight * 3 + standardOffsetY,
			sendButtonWidth, sendButtonHeight,
			handle,
			HMENU(deleteButton),
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

	HWND MainWindow::getList()
	{
		return list;
	}

	MainWindow::~MainWindow()
	{
		DestroyWindow(list);
		DestroyWindow(warning);
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

			case deleteButton:
				system(deleteDataBase);

				break;

			case sendButton:

			{
				static wchar_t fileNameCheck[128];

				if (!GetWindowTextW(FindWindowExW(ptr->getHWND(), nullptr, L"STATIC", fileNameBuffer.data()), fileNameCheck, 128))
				{
					break;
				}

				vector<string> res = parse();
				vector<data::UsersData<char>> information;
				information.reserve(res.size() / tableColumns);

				wstring type;
				int id = SendMessageW(ptr->getList(), CB_GETCURSEL, NULL, NULL);

				type.resize(SendMessageW(ptr->getList(), CB_GETLBTEXTLEN, id, NULL));

				SendMessageW(ptr->getList(), CB_GETLBTEXT, id, reinterpret_cast<LPARAM>(type.data()));

				for (size_t i = 0; i < res.size(); i += tableColumns)
				{
					information.emplace_back(vector<string>(begin(res) + i, begin(res) + i + tableColumns));
					information.back().get().push_back(payType[type]);
				}

				string command;

				for (const auto& i : information)
				{
					const vector<string>& ref = i.get();

					for (const auto& j : ref)
					{
						string tem;

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

				filesystem::path temFile = ptr->getCurrentDirectory();
				temFile.append("tem.txt");

				ofstream out(temFile);
				out << changeStrings(toANSI(toOEM866(command)));
				out.close();

				DWORD attributes = GetFileAttributesW(temFile.generic_wstring().data());
				SetFileAttributesW(temFile.generic_wstring().data(), attributes + FILE_ATTRIBUTE_HIDDEN);

				string relativePath = ptr->getCurrentDirectory().generic_string();
				relativePath += "/";
				relativePath += applicationName;

				system(string(relativePath + temFile.generic_string()).data());
			}

			break;
			}

			return 0;

		case WM_DESTROY:	//���������� ��� ����������� ����
			PostQuitMessage(0);

			return 0;

		default:
			return DefWindowProcW(hwnd, msg, wparam, lparam);
		}
	}

	vector<string> parse()
	{
		vector<string> res;

		ifstream file(fileNameBuffer.data());
		if (file.is_open())
		{
			string garbage;

			for (size_t i = 0; i < 3; i++)	//���������� ������ 3 ������
			{
				getline(file, garbage);
			}

			string line;

			while (getline(file, line))
			{
				string tem;

				size_t commaCount = 0;

				for (size_t i = 0; i < line.size(); i++)
				{
					if (/* line[i] == ',' && line[i + 1] != ' ' ���� �������� */ commaCount < 3 && line[i] == ',' || delimiters.find(line[i]) != end(delimiters))
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

		wstring temp = fileNameBuffer.data();

		file.lStructSize = sizeof(OPENFILENAMEW);
		file.hwndOwner = parent;
		file.lpstrFile = fileNameBuffer.data();
		file.lpstrFile[0] = L'\0';
		file.nMaxFile = sizeof(fileNameBuffer);
		file.lpstrFilter = L"��������� ���������\0*.txt\0\0";
		file.nFilterIndex = 1;
		file.lpstrFileTitle = nullptr;
		file.nMaxFileTitle = 0;
		file.lpstrInitialDir = nullptr;
		file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		GetOpenFileNameW(&file);

		wstring fileName = file.lpstrFile;

		SetWindowTextW(FindWindowExW(parent, nullptr, L"STATIC", temp.data()), fileName.data());
	}

	string changeStrings(string&& str)
	{
		for (const auto& i : toChange)
		{
			size_t index = string::npos;

			while ((index = str.find(i)) != string::npos)
			{
				str.replace(str.begin() + index, str.begin() + index + i.size(), changeTo);
			}
		}

		return str;
	}
}