#pragma once

#include "pch.h"

class VKInterface
{
private:

	enum Request
	{
		GETBASE = 0,
		READPROCESSMEMORY = 1,
		WRITEPROCESSMEMORY = 2,
		OPENHANDLE = 3,
		GETMODULEBASE = 4,
		GETMODULEBASE32 = 5,
	};

	struct Communication
	{

		Request Request;
		DWORD processID;
		DWORD Reason; // must be 0xDEADBEEF....
		PVOID Outbase; // output image base for process.

		/*
		* READ/WRITE PROCESS MEMORY.
		*/
		PVOID Address;
		size_t size;
		char moduleName[32];
		PVOID result;
	};

	struct CommunicationF
	{

		Request Request;
		DWORD processID;
		DWORD Reason; // must be 0xDEADBEEF....
		PVOID Outbase; // output image base for process.

		/*
		* READ/WRITE PROCESS MEMORY.
		*/
		PVOID Address;
		size_t size;
		char moduleName[32];
		float result;
	};

	struct CommunicationC
	{

		Request Request;
		DWORD processID;
		DWORD Reason; // must be 0xDEADBEEF....
		PVOID Outbase; // output image base for process.

		/*
		* READ/WRITE PROCESS MEMORY.
		*/
		PVOID Address;
		size_t size;
		char moduleName[32];
		char result;
	};

	struct CommunicationM
	{

		Request Request;
		DWORD processID;
		DWORD Reason; // must be 0xDEADBEEF....
		PVOID Outbase; // output image base for process.

		/*
		* READ/WRITE PROCESS MEMORY.
		*/
		PVOID Address;
		size_t size;
		char moduleName[32];
		float result[16];
	};

	void* g_DataPtr = 0;

public:
	VKInterface()
	{

	}

	BOOL Setup()
	{

		LoadLibraryA("user32.dll");
		LoadLibraryA("win32u.dll");
		LoadLibraryA("ntdll.dll");

		auto win32u = GetModuleHandle("win32u.dll");
		if (!win32u)
		{
			std::cout << "failed to load win32u.dll cichy" << std::endl;
			return FALSE;
		}

		g_DataPtr = GetProcAddress(win32u, "NtUserGetObjectInformation");
		if (!g_DataPtr)
		{
			std::cout << "failed to find NtUserGetObjectInformation" << std::endl;
			return FALSE;
		}

		return TRUE;
	}

	DWORD GetProcessID(std::string processName)
	{
		DWORD processpid = 0;
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (_stricmp(entry.szExeFile, processName.c_str()) == 0)
				{
					processpid = entry.th32ProcessID;
					break;
				}
			}
		}

		CloseHandle(snapshot);
		return processpid;
	}

	template<typename ... Arg>
	PVOID CallCommunication(const Arg ... args)
	{
		if (!g_DataPtr)
		{
			return NULL;
		}

		auto aFunc = static_cast<PVOID(_fastcall*)(Arg...)>(g_DataPtr);
		return aFunc(args ...);
	}

	PVOID GetBaseAddress(DWORD processID)
	{
		Communication request = {};
		SecureZeroMemory(&request, sizeof(Communication));

		request.Request = Request::GETBASE;
		request.Reason = 0xDEADBEEF;
		request.processID = processID;
		request.Outbase = 0;

		CallCommunication(0, 0, &request);
		return request.Outbase;
	}

	DWORD64 GetModuleAddress(DWORD processID, char moduleName[32])
	{
		Communication request = {};
		SecureZeroMemory(&request, sizeof(Communication));

		request.Request = Request::GETMODULEBASE;
		request.Reason = 0xDEADBEEF;
		request.processID = processID;
		strcpy_s(request.moduleName, moduleName);

		CallCommunication(0, 0, &request);
		return (DWORD64)request.Outbase;
	}

	PVOID ReadProcessMemory(DWORD processID, PVOID Address)
	{
		Communication request = {};
		SecureZeroMemory(&request, sizeof(Communication));

		request.Request = Request::READPROCESSMEMORY;
		request.Reason = 0xDEADBEEF;
		request.processID = processID;
		request.Address = Address;
		request.size = sizeof(PVOID);

		CallCommunication(0, 0, &request);
		return request.result;
	}

	float ReadProcessMemoryF(DWORD processID, PVOID Address)
	{
		CommunicationF request = {};
		SecureZeroMemory(&request, sizeof(CommunicationF));

		request.Request = Request::READPROCESSMEMORY;
		request.Reason = 0xDEADBEEF;
		request.processID = processID;
		request.Address = Address;
		request.size = sizeof(float);

		CallCommunication(0, 0, &request);
		return request.result;
	}

	char ReadProcessMemoryC(DWORD processID, PVOID Address)
	{
		CommunicationC request = {};
		SecureZeroMemory(&request, sizeof(CommunicationC));

		request.Request = Request::READPROCESSMEMORY;
		request.Reason = 0xDEADBEEF;
		request.processID = processID;
		request.Address = Address;
		request.size = sizeof(char);

		CallCommunication(0, 0, &request);
		return request.result;
	}

	void ReadProcessMemoryM(DWORD processID, PVOID Address, float (&out)[16])
	{
		CommunicationM request = {};
		SecureZeroMemory(&request, sizeof(CommunicationM));

		request.Request = Request::READPROCESSMEMORY;
		request.Reason = 0xDEADBEEF;
		request.processID = processID;
		request.Address = Address;
		request.size = (sizeof(float)*16);

		CallCommunication(0, 0, &request);

		std::copy(request.result, request.result + 16, out);

	}

	std::string ReadProcessMemoryS(DWORD processID, PVOID Address, int size, bool nullStop)
	{
		std::string result;
		char toAdd;
		for (int i = 0; i < size; i++)
		{
			toAdd = ReadProcessMemoryC(processID, (PVOID)((DWORD64)Address + i));
			if (nullStop && (toAdd == '\0'))
			{
				break;
			}
			result += toAdd;
		}
		return result;
	}


};