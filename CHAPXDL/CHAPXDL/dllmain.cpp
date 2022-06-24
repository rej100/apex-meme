// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "VKInterface.h"
#include "VKGBOInterface.h"
#include "threads.h"
#include "globals.h"


DWORD WINAPI MainThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "CHAPXDL v7.2.1" << std::endl;

    g::screenCenter.x = g::screenWidth / 2.0f;
    g::screenCenter.y = g::screenHeight / 2.0f;

    int procID = 0;
    std::cout << "id: ";
    std::cin >> procID;

    g::procID = procID;

    std::cin.get();
    std::cin.get();

    std::cout << "procID: " << g::procID << std::endl;

    VKInterface Vibe;
    Vibe.Setup();
    VKGBO Overlay;

    g::base = (DWORD64)Vibe.GetBaseAddress(g::procID);

    if (!mouse_open())
    {
        std::cout << "mouse fail" << std::endl;
    }

    std::cout << "krnl, ov, and mousesetup complete" << std::endl;

    threads::initialize(&Vibe, &Overlay);

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//DWORD WINAPI MainThread(HMODULE hModule)
//{
//    AllocConsole();
//    FILE* f;
//    freopen_s(&f, "CONIN$", "r", stdin);
//    freopen_s(&f, "CONOUT$", "w", stdout);
//
//    std::cout << "CHAPXDL v0.7.3" << std::endl;
//
//    VKInterface Vibe;
//    Vibe.Setup();
//    std::cout << "krnl setup complete" << std::endl;
//
//    int procID = 0;
//    std::cout << "id: ";
//    std::cin >> procID;
//    std::cin.get();
//    std::cin.get();
//
//    std::cout << "procID: " << procID << std::endl;
//
//    DWORD64 baseAddy = (DWORD64)Vibe.GetBaseAddress(procID);
//    std::cout << "baseAddy: " << std::hex << baseAddy << std::dec << std::endl;
//
//    DWORD64 lpAddy = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(baseAddy + off::LocalPlayer));
//    std::cout << "lpAddy: " << std::hex << lpAddy << std::dec << std::endl;
//
//    while (true)
//    {
//        lpAddy = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(baseAddy + off::LocalPlayer));
//        if (!lpAddy)
//        {
//            std::cout << "LP continue" << std::endl;
//            continue;
//        }
//        float x = Vibe.ReadProcessMemoryF(procID, (PVOID)(lpAddy + off::m_vecAbsOrigin));
//        float y = Vibe.ReadProcessMemoryF(procID, (PVOID)(lpAddy + off::m_vecAbsOrigin + 0x4));
//        float z = Vibe.ReadProcessMemoryF(procID, (PVOID)(lpAddy + off::m_vecAbsOrigin + 0x8));
//
//        system("cls");
//
//        std::cout << "lpAddy: " << std::hex << lpAddy << std::dec << std::endl;
//        std::cout << "x: " << x << std::endl;
//        std::cout << "y: " << y << std::endl;
//        std::cout << "z: " << z << std::endl;
//
//        DWORD64 boneArrayAddy = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(lpAddy + off::m_bConstrainBetweenEndpoints));
//        float headx = Vibe.ReadProcessMemoryF(procID, (PVOID)(boneArrayAddy + 0xCC + (8 * 0x30))) + x;
//        float heady = Vibe.ReadProcessMemoryF(procID, (PVOID)(boneArrayAddy + 0xDC + (8 * 0x30))) + y;
//        float headz = Vibe.ReadProcessMemoryF(procID, (PVOID)(boneArrayAddy + 0xEC + (8 * 0x30))) + z;
//
//        std::cout << "xh: " << headx << std::endl;
//        std::cout << "yh: " << heady << std::endl;
//        std::cout << "zh: " << headz << std::endl;
//
//        DWORD64 viewRender = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(baseAddy + off::ViewRender));
//        DWORD64 viewMatrix = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(viewRender + off::ViewMatrix));
//
//        float matrix[16];
//        for (int i = 0; i < 16; i++)
//        {
//            matrix[i] = Vibe.ReadProcessMemoryF(procID, (PVOID)(viewMatrix + (4 * i)));
//        }
//
//        std::cout << "===" << std::endl;
//
//        for (int i = 0; i < 16; i++)
//        {
//            std::cout << "f" << i << ": " << matrix[i] << std::endl;
//        }
//
//        std::cout << "===" << std::endl;
//
//        std::string ident;
//        std::string ident2;
//        DWORD64 identoff = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(lpAddy + off::m_ModelName));
//        ident2 = Vibe.ReadProcessMemoryS(procID, (PVOID)(identoff), 100, true);
//
//        if (ident2 != "")
//        {
//            std::cout << "ident: " << ident2 << std::endl;
//        }
//
//        for (int i = 0; i < 32; i++)
//        {
//            DWORD64 entityListAddy = baseAddy + off::cl_entitylist;
//
//            DWORD64 baseEntity = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)entityListAddy);
//
//            if (!baseEntity)
//            {
//                std::cout << "noBase: " << ident2 << std::endl;
//                continue;
//            }
//            DWORD64 entityAddy = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(entityListAddy + (i << 5)));
//            DWORD64 entityModelNameAddy = (DWORD64)Vibe.ReadProcessMemory(procID, (PVOID)(entityAddy + off::m_ModelName));
//
//            std::string identifier;
//            identifier = Vibe.ReadProcessMemoryS(procID, (PVOID)(entityModelNameAddy), 100, true);
//
//            if (identifier != "")
//            {
//                std::cout << i << ": " << identifier << std::endl;
//            }
//        }
//
//
//        Sleep(10);
//    }
//
//    fclose(f);
//    FreeConsole();
//    FreeLibraryAndExitThread(hModule, 0);
//
//    return 0;
//}
