#pragma once

#include <Windows.h>
#include <filesystem>
#include "Constants.h"

namespace windows
{
	enum Buttons : unsigned char
	{
		chooseButton = 192,
		sendButton
	};

	class MainWindow
	{
	private:
		std::filesystem::path currentDirectory;

		HWND handle;

		HWND warning;

		HWND chooseFileButton;
		HWND sendFileButton;

		HWND currentFile;
		HWND list;

	private:
		MainWindow();

		MainWindow& operator =(const MainWindow&) = delete;

		MainWindow& operator =(MainWindow&&) noexcept = delete;

		~MainWindow();

	public:
		std::filesystem::path getCurrentDirectory() const;

		static MainWindow& get();

		HWND getHWND();

		HWND getList();
	};
}