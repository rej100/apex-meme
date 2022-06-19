#pragma once

#include "pch.h";

#define OV_EMPTY 0
#define OV_LINE 1
#define OV_RECT 2
#define OV_RECT_FILL 3
#define OV_TEXT 4
#define OV_CIRCLE 5
#define OV_CIRCLE_FILL 6

//164*2000 = 328 000
#define BUF_SIZE 328000

//TCHAR szName[] = TEXT("AppContainerNamedObjects\\S-1-15-2-1741734852-3412480322-2855469391-1255631337-1708672570-3205867252-2525140288\\OVTMappingObject");
//TCHAR szMutexName[] = TEXT("AppContainerNamedObjects\\S-1-15-2-1741734852-3412480322-2855469391-1255631337-1708672570-3205867252-2525140288\\OVTMutexObject");



class VKGBO
{
private:

    struct OVObject
    {
        short type;

        float x;
        float y;

        short r;
        short g;
        short b;
        short a;

        float xTo;
        float yTo;

        float width;
        float length;

        char text[128];
    };

    HANDLE hMapFile;
    HANDLE hMutex;
    DWORD64 pBuf;
    std::vector<OVObject> OVObjects;

    OVObject constructOVO(short type, float x, float y, short r, short g, short b, short a, float xTo, float yTo, float width, float length, char* text)
    {
        OVObject result;

        result.type = type;

        result.x = x;
        result.y = y;

        result.r = r;
        result.g = g;
        result.b = b;
        result.a = a;

        result.xTo = xTo;
        result.yTo = yTo;

        result.width = width;
        result.length = length;

        strcpy_s(result.text, text);

        return result;
    }

public:

    VKGBO()
    {
        TCHAR szName[] = TEXT("AppContainerNamedObjects\\S-1-15-2-1741734852-3412480322-2855469391-1255631337-1708672570-3205867252-2525140288\\OVTMappingObject");
        TCHAR szMutexName[] = TEXT("AppContainerNamedObjects\\S-1-15-2-1741734852-3412480322-2855469391-1255631337-1708672570-3205867252-2525140288\\OVTMutexObject");

        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, szName);
        pBuf = (DWORD64)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);

        hMutex = CreateMutex(NULL, FALSE, szMutexName);
    }

    void clearQueue()
    {
        OVObjects.clear();
    }

    int pushToOverlay()
    {
        OVObject* simpleArray;
        int sSize = OVObjects.size();
        simpleArray = new OVObject[sSize];

        for (int i = 0; i < sSize; i++)
        {
            simpleArray[i] = OVObjects[i];
        }

        WaitForSingleObject(hMutex, INFINITE);

        CopyMemory((PVOID)(pBuf), &sSize, 4);
        CopyMemory((PVOID)(pBuf + 4), simpleArray, sizeof(OVObject) * sSize);

        ReleaseMutex(hMutex);
        
        clearQueue();

        return 0;
    }

    void queueLine(float x, float y, short r, short g, short b, short a, float xTo, float yTo, float width)
    {
        char emptyTxt[128] = "";
        OVObject toQueueOVO = constructOVO(OV_LINE, x, y, r, g, b, a, xTo, yTo, width, 0.0f, emptyTxt);
        OVObjects.push_back(toQueueOVO);
    }

    void queueRect(float x, float y, short r, short g, short b, short a, float width, float length)
    {
        char emptyTxt[128] = "";
        OVObject toQueueOVO = constructOVO(OV_RECT, x, y, r, g, b, a, 0.0f, 0.0f, width, length, emptyTxt);
        OVObjects.push_back(toQueueOVO);
    }

    void queueRectFill(float x, float y, short r, short g, short b, short a, float width, float length)
    {
        char emptyTxt[128] = "";
        OVObject toQueueOVO = constructOVO(OV_RECT_FILL, x, y, r, g, b, a, 0.0f, 0.0f, width, length, emptyTxt);
        OVObjects.push_back(toQueueOVO);
    }

    void queueCircle(float x, float y, short r, short g, short b, short a, float radius)
    {
        char emptyTxt[128] = "";
        OVObject toQueueOVO = constructOVO(OV_CIRCLE, x, y, r, g, b, a, 0.0f, 0.0f, radius, 0.0f, emptyTxt);
        OVObjects.push_back(toQueueOVO);
    }

    void queueCircleFill(float x, float y, short r, short g, short b, short a, float radius)
    {
        char emptyTxt[128] = "";
        OVObject toQueueOVO = constructOVO(OV_CIRCLE_FILL, x, y, r, g, b, a, 0.0f, 0.0f, radius, 0.0f, emptyTxt);
        OVObjects.push_back(toQueueOVO);
    }

    void queueText(float x, float y, short r, short g, short b, short a, float size, std::string text)
    {
        char simpleText[128];
        strcpy_s(simpleText, text.c_str());

        OVObject toQueueOVO = constructOVO(OV_TEXT, x, y, r, g, b, a, 0.0f, 0.0f, size, 0.0f, simpleText);
        OVObjects.push_back(toQueueOVO);
    }

};