#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "Assault Cube Cheat.h"

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

class Game
{
private:
	LPCTSTR m_gameName = { };

public:
	Game(LPCTSTR gameName):
		m_gameName(gameName)
	{
	}

	Game()
	{
	}

	~Game()
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

int main()
{
	AcPlayer myAcPlayer;
	uintptr_t offsets[] = { myAcPlayer.hp, myAcPlayer.armor, myAcPlayer.assaultRifleLoadedAmmos, myAcPlayer.pistolLoadedAmmos, myAcPlayer.carabineLoadedAmmos, myAcPlayer.shotgunLoadedAmmos,
		myAcPlayer.smgLoadedAmmos, myAcPlayer.sniperRifleLoadedAmmos, myAcPlayer.assaultRifleLoadedAmmos, myAcPlayer.doublePistolLoadedAmmos, myAcPlayer.grenades };
	const int numOffsets = sizeof(offsets) / sizeof(offsets[0]);
	int intToWrite = { 999 };

	Game assaultCube{ TEXT("AssaultCube") };
	HWND windowHandle = { assaultCube.findWindow() };
	DWORD PID = {assaultCube.getPID(windowHandle)};

	Bypass bypass;
	bypass.Attach(PID);
	
	uintptr_t mainPtr = { 0 };
	uintptr_t tempPtr = { 0 };
	int intToRead = { 0 };

	//Find the final pointer address by RPM and adding offsets
	for (int i = 0; i < numOffsets; i++)
	{

		if (i == 0)
		{
			bypass.Read(myAcPlayer.playerBaseAddress, &mainPtr, sizeof(mainPtr), 0); //Reading the final pointer of base address
			cout << "tempPtr      (0x" << hex << uppercase << &mainPtr << ") = 0x" << mainPtr << "\n";
		}

		tempPtr = mainPtr + offsets[i];
		bypass.Read(tempPtr, &intToRead, sizeof(int), 0); //Reading the integer at the final pointer + offset
		cout << "ptrRead      (0x" << hex << uppercase << &tempPtr << ") = 0x" << tempPtr << "\n";
		cout << "intRead      (0x" << hex << uppercase << &intToRead << ") = " << dec << intToRead << "\n";

		bypass.Write(tempPtr, &intToWrite, sizeof(int), 0); //Overwrite the value at the final pointer
		cout << "intToWrite      (0x" << hex << uppercase << &intToWrite << ") = " << dec << intToWrite << "\n";
	}

	return 0;
}