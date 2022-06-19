#pragma once

#include "pch.h"
#include "aoffsetts.h"
#include "VKInterface.h"
#include "VKGBOInterface.h"
#include "globals.h"
#include "mouse.h"

namespace threads
{

    void localLoop(VKInterface* Vibe)
    {
        while (true)
        {
            DWORD64 viewRender = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)(g::base + off::ViewRender));
            DWORD64 viewMatrixAddy = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)(viewRender + off::ViewMatrix));

            VMatrix viewMatrix;
            Vibe->ReadProcessMemoryM(g::procID, (PVOID)(viewMatrixAddy), viewMatrix.f);

            g::viewMatrix = viewMatrix;
        }
    }

    void entityLoop(VKInterface* Vibe)
    {
        std::vector<std::string> pFlags;

        pFlags.push_back("pilot");
        pFlags.push_back("ash");
        pFlags.push_back("bangalore");
        pFlags.push_back("bloodhound");
        pFlags.push_back("caustic");
        pFlags.push_back("crypto");
        pFlags.push_back("fuse");
        pFlags.push_back("gibraltar");
        pFlags.push_back("horizon");
        pFlags.push_back("lifeline");
        pFlags.push_back("loba");
        pFlags.push_back("maggie");
        pFlags.push_back("mirage");
        pFlags.push_back("newcastle");
        pFlags.push_back("octane");
        pFlags.push_back("pathfinder");
        pFlags.push_back("rampart");
        pFlags.push_back("revenant");
        pFlags.push_back("seer");
        pFlags.push_back("valkyrie");
        pFlags.push_back("wattson");
        pFlags.push_back("wraith");

        //pFlags.push_back("dummy");
        
        //pFlags.push_back("dummie");
        //pFlags.push_back("npc");

        while (true)
        {

            Player localPlayer;

            localPlayer.address = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)(g::base + off::LocalPlayer));

            if (!localPlayer.address)
            {
                continue;
            }

            localPlayer.team = (int)Vibe->ReadProcessMemory(g::procID, (PVOID)(localPlayer.address + off::m_iTeamNum));

            localPlayer.origin.x = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(localPlayer.address + off::m_vecAbsOrigin + (0x4 * 0)));
            localPlayer.origin.y = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(localPlayer.address + off::m_vecAbsOrigin + (0x4 * 1)));
            localPlayer.origin.z = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(localPlayer.address + off::m_vecAbsOrigin + (0x4 * 2)));

            g::localPlayer = localPlayer;

            std::vector<DWORD64> playerAddies;

            for (int i = 0; i < off::NUM_ENT_ENTRIES; i++)
            {
                DWORD64 entityListAddy = g::base + off::cl_entitylist;

                DWORD64 baseEntity = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)entityListAddy);

                if (!baseEntity)
                {
                    continue;
                }

                DWORD64 entityAddy = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)(entityListAddy + (i << 5)));

                DWORD64 entityModelNameAddy = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)(entityAddy + off::m_ModelName));

                std::string identifier;
                identifier = Vibe->ReadProcessMemoryS(g::procID, (PVOID)(entityModelNameAddy), 100, true);

                bool isPlayer = false;

                for (int i = 0; i < pFlags.size(); i++)
                {
                    if (identifier.find(pFlags[i]) != std::string::npos)
                    {
                        isPlayer = true;
                        break;
                    }
                }

                if (isPlayer)
                {
                    //std::cout << "p" << i << ": " << identifier << std::endl;
                    int teamNum = (int)Vibe->ReadProcessMemory(g::procID, (PVOID)(entityAddy + off::m_iTeamNum));
                    if (teamNum != g::localPlayer.team)
                    {
                        playerAddies.push_back(entityAddy);
                    }
                }
            }

            g::playerAddies = playerAddies;
            Sleep(10);
        }

    }

    void playersLoop(VKInterface* Vibe)
    {
        while (true)
        {
            std::vector<DWORD64> playerAddies = g::playerAddies;
            std::vector<Player> players;

            for (int i = 0; i < playerAddies.size(); i++)
            {
                Player tempPlayer;
                tempPlayer.address = playerAddies[i];

                tempPlayer.health = (int)Vibe->ReadProcessMemory(g::procID, (PVOID)(tempPlayer.address + off::m_iHealth));
                tempPlayer.shieldHealth = (int)Vibe->ReadProcessMemory(g::procID, (PVOID)(tempPlayer.address + off::m_shieldHealth));

                tempPlayer.origin.x = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(tempPlayer.address + off::m_vecAbsOrigin + (0x4 * 0)));
                tempPlayer.origin.y = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(tempPlayer.address + off::m_vecAbsOrigin + (0x4 * 1)));
                tempPlayer.origin.z = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(tempPlayer.address + off::m_vecAbsOrigin + (0x4 * 2)));

                DWORD64 boneArrayAddy = (DWORD64)Vibe->ReadProcessMemory(g::procID, (PVOID)(tempPlayer.address + off::m_bConstrainBetweenEndpoints));
                tempPlayer.head.x = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(boneArrayAddy + 0xCC + (8 * 0x30))) + tempPlayer.origin.x;
                tempPlayer.head.y = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(boneArrayAddy + 0xDC + (8 * 0x30))) + tempPlayer.origin.y;
                tempPlayer.head.z = Vibe->ReadProcessMemoryF(g::procID, (PVOID)(boneArrayAddy + 0xEC + (8 * 0x30))) + tempPlayer.origin.z;

                players.push_back(tempPlayer);

            }

            g::players = players;
            
        }

    }

    void renderLoop(VKGBO* Overlay)
    {
        while (true)
        {
            Overlay->queueCircle(g::screenCenter.x, g::screenCenter.y, 0, 255, 0, 255, g::aimFov);
            Overlay->queueCircle(g::screenCenter.x, g::screenCenter.y, 0, 255, 0, 255, g::aimSnap);

            std::vector<Player> players = g::players;
            VMatrix viewMatrix = g::viewMatrix;
            Player localPlayer = g::localPlayer;

            Player closestPlayer;

            if (!players.empty())
            {
                closestPlayer = players[0];
            }

            for (int i = 0; i < players.size(); i++)
            {
                Vector3 originPos = worldToScreen(players[i].origin, viewMatrix);
                Vector3 headPos = worldToScreen(players[i].head, viewMatrix);

                float yDist = headPos.y - originPos.y;
                if (yDist < 0)
                {
                    yDist *= -1.0f;
                }
                float xDist = yDist / 3.0f;

                Overlay->queueRect(headPos.x - (xDist / 2.0f), headPos.y, 255, 255, 255, 255, xDist, yDist);

                Overlay->queueLine((headPos.x - (xDist / 2.0f) - 2.0f), headPos.y + yDist, 255, 0, 0, 255, (headPos.x - (xDist / 2.0f) - 2.0f), headPos.y, 2.0f);
                Overlay->queueLine((headPos.x - (xDist / 2.0f) - 4.0f), headPos.y + yDist, 255, 0, 0, 255, (headPos.x - (xDist / 2.0f) - 4.0f), headPos.y, 2.0f);

                float healthLength = yDist * (players[i].health / 100.0f);
                float shieldLength = yDist * (players[i].shieldHealth / 125.0f);

                Overlay->queueLine((headPos.x - (xDist / 2.0f) - 2.0f), headPos.y + yDist, 0, 255, 0, 255, (headPos.x - (xDist / 2.0f) - 2.0f), headPos.y + yDist - healthLength, 2.0f);
                Overlay->queueLine((headPos.x - (xDist / 2.0f) - 4.0f), headPos.y + yDist, 0, 0, 255, 255, (headPos.x - (xDist / 2.0f) - 4.0f), headPos.y + yDist - shieldLength, 2.0f);

                if (vector3dist(localPlayer.origin, players[i].head) < vector3dist(localPlayer.origin, closestPlayer.head))
                {
                    closestPlayer = players[i];
                }
            }
            if (!players.empty())
            {
                Vector3 closestHeadPos = worldToScreen(closestPlayer.head, viewMatrix);

                Overlay->queueLine(g::screenCenter.x, g::screenCenter.y, 255, 0, 0, 255, closestHeadPos.x, closestHeadPos.y, 1.0f);
            }

            Overlay->pushToOverlay();

        }
    }

    void aimLoop()
    {
        while (true)
        {
                std::vector<Player> players = g::players;
                Player localPlayer = g::localPlayer;
                VMatrix viewMatrix = g::viewMatrix;

                Vector3 tempHead;
                Vector2 tempv2;

                Vector3 toAim;

                toAim.x = 0;
                toAim.y = 0;
                toAim.z = 9999.0f;

                POINT cPos;
                GetCursorPos(&cPos);
                Vector2 cursorPos;
                cursorPos.x = cPos.x;
                cursorPos.y = cPos.y;

                for (int i = 0; i < players.size(); i++)
                {
                    if (vector3dist(localPlayer.origin, players[i].head) < g::aimDist)
                    {
                        tempHead = worldToScreen(players[i].head, viewMatrix);

                        tempv2.x = tempHead.x;
                        tempv2.y = tempHead.y;
                        tempHead.z = vector2dist(cursorPos, tempv2);

                        if (tempHead.z < toAim.z)
                        {
                            toAim = tempHead;
                        }
                    }
                }

                GetCursorPos(&cPos);
                cursorPos.x = cPos.x;
                cursorPos.y = cPos.y;

                tempv2.x = toAim.x;
                tempv2.y = toAim.y;
                toAim.z = vector2dist(tempv2, cursorPos);

                toAim.x = (toAim.x - cursorPos.x);
                toAim.y = (toAim.y - cursorPos.y);


                if (toAim.z > g::aimSnap)
                {
                    toAim.x /= g::aSmooth;
                    toAim.y /= g::aSmooth;
                }

                if (toAim.z < g::aimFov)
                {
                    if (GetAsyncKeyState(VK_MENU)) //alt
                    {
                        mouse_move(0, (int)toAim.x, (int)toAim.y, 0);
                        Sleep(2);
                    }
                }
        }
    }

    void initialize(VKInterface* VibePtr, VKGBO* OverlayPtr)
    {
        std::thread localThread(localLoop, VibePtr);
        std::thread entityThread(entityLoop, VibePtr);
        std::thread playersThread(playersLoop, VibePtr);
        std::thread renderThread(renderLoop, OverlayPtr);
        std::thread aimThread(aimLoop);

        localThread.join();
        entityThread.join();
        playersThread.join();
        renderThread.join();
        aimThread.join();
    }
}

