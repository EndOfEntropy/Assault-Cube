#pragma once
#include <Windows.h>
#include <iostream>


using namespace std;

class Bypass
{

private:
	HANDLE m_hProcess = { 0 };

public:
	Bypass()
	{
	}

	~Bypass()
	{
		if (m_hProcess != 0) {
			CloseHandle(m_hProcess);
		}
		cout << "Destructing Handle: " << m_hProcess << "\n";
	}

	bool Attach(DWORD pid)
	{
		m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
		if (m_hProcess == 0) // Failed to get a handle
		{
			cout << "OpenProcess failed. GetLastError = " << dec << GetLastError() << "\n";
			system("pause");
			return 1;
		}
		else
		{
			return m_hProcess;
		}
	}

	bool Read(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead)
	{
		bool rProcess = ReadProcessMemory(m_hProcess, (LPCVOID)lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
		if (rProcess == 0) // Failed to read memory
		{
			cout << "ReadProcessMemory failed. GetLastError = " << dec << GetLastError() << "\n";
			system("pause");
			return 1;
		}
		else
		{
			return rProcess;
		}
	}

	bool Write(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesWritten)
	{
		bool wProcess = WriteProcessMemory(m_hProcess, (LPVOID)lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
		if (wProcess == 0) // Failed to read memory
		{
			cout << "WriteProcessMemory failed. GetLastError = " << dec << GetLastError() << "\n";
			system("pause");
			return 1;
		}
		else
		{
			return wProcess;
		}
	}

};

class myGame
{
private:
	LPCTSTR m_gameName = {};

public:
	myGame(LPCTSTR gameName) :
		m_gameName(gameName)
	{
	}

	myGame()
	{
	}

	~myGame()
	{
	}

	LPCTSTR getGameName() { return m_gameName; }

	//Finding the game window named AssaultCube
	HWND findWindow()
	{
		HWND hGameWindow = FindWindow(0, m_gameName);
		if (!hGameWindow)
		{
			cout << m_gameName << " not found. GetLastError = " << dec << GetLastError() << "\n";
			system("pause > nul");
			return 0;
		}
		else
		{
			wcout << m_gameName << " found.\n"; //to print (char *) string use wcout
			return hGameWindow;
		}
	}

	//Finding the processID of the game window
	DWORD getPID(HWND handle)
	{
		DWORD processID = 0;
		GetWindowThreadProcessId(handle, &processID);
		if (!processID)
		{
			cout << "Getting window PID failed. GetLastError = " << dec << GetLastError() << "\n";
			system("pause > nul");
			return 1;
		}
		else
		{
			wcout << m_gameName << " PID found.\n"; //to print (char *) string use wcout
			return processID;
		}
	}

};